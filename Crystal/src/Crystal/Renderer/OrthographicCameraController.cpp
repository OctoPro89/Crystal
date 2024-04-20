#include "crystalpch.h"
#include "OrthographicCameraController.h"
#include "Crystal/Core/Input.h"
#include "Crystal/Core/Keycodes.h"

namespace Crystal
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		CRYSTAL_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(CRYSTAL_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(CRYSTAL_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(CRYSTAL_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(CRYSTAL_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(CRYSTAL_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(CRYSTAL_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		CRYSTAL_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CRYSTAL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CRYSTAL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}


	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Bounds = { m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top };
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		CRYSTAL_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		CRYSTAL_PROFILE_FUNCTION();
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
