#pragma once
#include <box2d/b2_contact.h>
#include <box2d/Box2d.h>
#include <Crystal/Core/UUID.h>
#include <Crystal/Scripting/ScriptEngine.h>
namespace Crystal {
	class PhysicsContactListener : public b2ContactListener {
	public:
		void BeginContact(b2Contact* contact) override {
			// Get the Box2D fixtures involved in the collision
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// Get the user data (entity ID) associated with each fixture
			b2BodyUserData& bodyData = bodyA->GetUserData();
			UUID* entityID_A = reinterpret_cast<UUID*>(bodyData.pointer);
			UUID* entityID_B = reinterpret_cast<UUID*>(bodyData.pointer);

			// Now, you can use the entity IDs to identify the entities involved in the collision
			// Implement your collision handling logic here
		}

		void EndContact(b2Contact* contact) override {
			// Handle collision end
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// Get the user data (entity ID) associated with each fixture
			b2BodyUserData& bodyData = bodyA->GetUserData();
			UUID& entityID_A = (UUID)bodyData.pointer;
			UUID& entityID_B = (UUID)bodyData.pointer;
		}
	};
}
