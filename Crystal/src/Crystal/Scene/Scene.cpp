#include "crystalpch.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "Crystal/Scripting/ScriptEngine.h"
#include "Crystal/Renderer/Renderer2D.h"
#include "Crystal/Physics/Physics2D.h"

#include <glm/glm.hpp>

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_joint.h"
#include "box2d/b2_distance_joint.h"
#include "box2d/b2_revolute_joint.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Crystal {
	class PhysicsContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override {
			// Get the Box2D fixtures involved in the collision
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// Get the user data (entity ID) associated with each fixture
			b2BodyUserData& bodyData = bodyA->GetUserData();
			b2BodyUserData& bodyData2 = bodyB->GetUserData();
			UUID entityID_A = (UUID)bodyData.pointer;
			UUID entityID_B = (UUID)bodyData2.pointer;

			// Now, you can use the entity IDs to identify the entities involved in the collision
			Scene* scene = ScriptEngine::GetSceneContext();
			CRYSTAL_CORE_ASSERT(scene, "No Scene");
			Entity entity = scene->GetEntityByUUID(entityID_A);
			Entity entity2 = scene->GetEntityByUUID(entityID_B);
			CRYSTAL_CORE_ASSERT(entity, "No Entity!");
			CRYSTAL_CORE_ASSERT(entity2, "No Entity!");
			// entity1 is the thing we hit, entity2 is the thing with the script
			ScriptEngine::OnCollisionEnter(entity2, entity, contact);
		}

		void EndContact(b2Contact* contact) override {
			// Handle collision end
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// Get the user data (entity ID) associated with each fixture
			b2BodyUserData& bodyData = bodyA->GetUserData();
			b2BodyUserData& bodyData2 = bodyB->GetUserData();
			UUID& entityID_A = (UUID)bodyData.pointer;
			UUID& entityID_B = (UUID)bodyData2.pointer;

			Scene* scene = ScriptEngine::GetSceneContext();
			CRYSTAL_CORE_ASSERT(scene, "No Scene");
			Entity entity = scene->GetEntityByUUID(entityID_A);
			Entity entity2 = scene->GetEntityByUUID(entityID_B);
			CRYSTAL_CORE_ASSERT(entity, "No Entity!");
			CRYSTAL_CORE_ASSERT(entity2, "No Entity!");
			// entity1 is the thing we exit, entity2 is the thing with the script
			ScriptEngine::OnCollisionExit(entity2, entity, contact);
		}

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
		{

		}
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
		{

		}
	};

	PhysicsContactListener m_ContactListener;

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
		OnPhysics2DStart();
		OnPhysics2DJoints();
		m_PhysicsWorld->SetContactListener(&m_ContactListener);
		// Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			// Instantiate all script entities

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;

		OnPhysics2DStop();

		ScriptEngine::OnRuntimeStop();
		SoundSystem::Stop(); /* Stop all the sounds that were playing before */
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
		OnPhysics2DJoints();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (!m_IsPaused || m_StepFrames-- > 0) {
			// Update scripts
			{
				// C# Entity OnUpdate
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };

					auto name = entity.GetName();

					CRYSTAL_CORE_ASSERT(entity, "No Entity!");

					ScriptEngine::OnUpdateEntity(entity, ts);
				}

				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
			}

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;

					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			// Draw sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}
			
			// Draw Lines
			{
				auto view = m_Registry.view<TransformComponent, LineRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, line] = view.get<TransformComponent, LineRendererComponent>(entity);

					Renderer2D::DrawLine(line.Pos1, line.Pos2, line.Color, (int)entity);
				}
			}

			// Draw Text
			{
				auto view = m_Registry.view<TransformComponent, TextRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, text] = view.get<TransformComponent, TextRendererComponent>(entity);

					Renderer2D::DrawString(transform.GetTransform(), text, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}

		// Render
		RenderScene(camera);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// Render
		RenderScene(camera);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		/* Copy name because we're going to modify component data structure 
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		*/


		/* USE FOR " COPY" NAME */
		Entity newEntity = CreateEntity(entity.GetName() + " Copy");
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		CRYSTAL_CORE_ASSERT(m_EntityMap.find(uuid) != m_EntityMap.end(), "Failed to retrieve entity!");
			
		return { m_EntityMap.at(uuid), this };
	}

	void Scene::OnPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });
		auto view = m_Registry.view<Rigidbody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
				body->SetGravityScale(bc2d.GravityScale);
				b2BodyUserData& bodyData = body->GetUserData();
				//Set user data to the uuid for collisions
				bodyData.pointer = (uintptr_t)entity.GetUUID();
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
				body->SetGravityScale(cc2d.GravityScale);
				b2BodyUserData& bodyData = body->GetUserData();
				//Set user data to the uuid for collisions
				bodyData.pointer = (uintptr_t)entity.GetUUID();
			}
		}
	}

	void Scene::OnPhysics2DJoints()
	{
		auto viewHinge = m_Registry.view<HingeJoint2DComponent>();
		auto viewDistance = m_Registry.view<DistanceJoint2DComponent>();


		for (auto e : viewHinge)
		{
			Entity This = { e, this };
			auto& hj2d = This.GetComponent<HingeJoint2DComponent>();
			Entity Attached = { this->m_EntityMap.at(hj2d.Attached), this };

			b2RevoluteJointDef rjd;
			rjd.type = e_revoluteJoint;
			rjd.bodyA = (b2Body*)This.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			rjd.bodyB = (b2Body*)Attached.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			rjd.collideConnected = hj2d.ShouldBodiesCollide;
			rjd.enableLimit = hj2d.EnableLimit;
			rjd.enableMotor = hj2d.EnableMotor;
			rjd.localAnchorA = { hj2d.AnchorOffset1.x, hj2d.AnchorOffset1.y };
			rjd.localAnchorB = { hj2d.AnchorOffset2.x, hj2d.AnchorOffset2.y };
			rjd.maxMotorTorque = hj2d.MaxMotorTorque;
			rjd.motorSpeed = hj2d.MotorSpeed;
			rjd.upperAngle = glm::radians(hj2d.UpperLimitAngle);
			rjd.lowerAngle = glm::radians(hj2d.LowerLimitAngle);
			b2RevoluteJoint* dj = (b2RevoluteJoint*)m_PhysicsWorld->CreateJoint(&rjd);
		}

		for (auto e : viewDistance)
		{
			Entity This = { e, this };
			auto& dj2d = This.GetComponent<DistanceJoint2DComponent>();
			Entity Attached = { this->m_EntityMap.at(dj2d.Attached), this };

			b2DistanceJointDef djd;
			djd.type = e_distanceJoint;
			djd.bodyA = (b2Body*)This.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			djd.bodyB = (b2Body*)Attached.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			djd.localAnchorA = { dj2d.AnchorOffset1.x, dj2d.AnchorOffset1.y };
			djd.localAnchorB = { dj2d.AnchorOffset2.x, dj2d.AnchorOffset2.y };
			djd.collideConnected = dj2d.ShouldBodiesCollide;
			djd.damping = dj2d.Damping;
			djd.length = dj2d.RestLength;
			djd.minLength = dj2d.MinDistance;
			djd.maxLength = dj2d.MaxDistance;
			djd.stiffness = dj2d.Stiffness;
			b2DistanceJoint* dj = (b2DistanceJoint*)m_PhysicsWorld->CreateJoint(&djd);
		}		
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
		}

		// Draw Lines
		{
			auto view = m_Registry.view<TransformComponent, LineRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, line] = view.get<TransformComponent, LineRendererComponent>(entity);

				Renderer2D::DrawLine(line.Pos1, line.Pos2, line.Color, (int)entity);
			}
		}

		// Draw Text
		{
			auto view = m_Registry.view<TransformComponent, TextRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, text] = view.get<TransformComponent, TextRendererComponent>(entity);

				Renderer2D::DrawString(transform.GetTransform(), text, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<LineRendererComponent>(Entity entity, LineRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TextRendererComponent>(Entity entity, TextRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<DistanceJoint2DComponent>(Entity entity, DistanceJoint2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<HingeJoint2DComponent>(Entity entity, HingeJoint2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<AudioComponent>(Entity entity, AudioComponent& component)
	{
	}
}
