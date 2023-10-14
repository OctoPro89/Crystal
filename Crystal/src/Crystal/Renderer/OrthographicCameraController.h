#pragma once
#include "crystalpch.h"
#include "OrthographicCameraController.h"

#include "Crystal/Renderer/OrthographicCamera.h"
#include "Crystal/Core/Timestep.h"
#include "Crystal/Events/ApplicationEvent.h"
#include "Crystal/Events/MouseEvent.h"

namespace Crystal
{
	struct OrthographicCameraBounds
	{
		float Left, Right, Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 90.0f;
	};
}