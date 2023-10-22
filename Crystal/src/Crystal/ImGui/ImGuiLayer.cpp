#include "crystalpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Crystal/Core/Application.h"

//TEMPORARY
#include <GLFW/glfw3.h>
#include <Glad/glad.h>

namespace Crystal {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}
	ImGuiLayer::~ImGuiLayer()
	{

	}
	void ImGuiLayer::OnAttach()
	{
		CRYSTAL_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		float fontSize = 18.0f;
		
		io.Fonts->AddFontFromFileTTF("assets/fonts/Kalam/Kalam-Bold.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("assets/fonts/Kalam/Kalam-Regular.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Bold.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", fontSize);
		io.FontDefault = io.Fonts->Fonts[5];

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
		CRYSTAL_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		CRYSTAL_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	void ImGuiLayer::End()
	{
		CRYSTAL_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
// 		colors[ImGuiCol_Header] = ImVec4{ 0.027, 0.141, 0.259, 1.0f };
// 		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.04327, 0.43141, 0.43259, 1.0f };
// 		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.03627, 0.26141, 0.3659, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.047f, 0.431f, .839f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.06547f, 0.5431f, .9839, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.037f, 0.331f, .739f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.047f, 0.431f, .839f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.0547f, 0.5431f, .9839f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.04547f, 0.54431f, .9339f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.047f, 0.431f, .839, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.047f, 0.431f, .839f, 1.0f };
	}

	void ImGuiLayer::SetMonochromeTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	void ImGuiLayer::SetDefaultThemeColors()
	{
		ImGui::StyleColorsClassic();
	}

	void ImGuiLayer::SetDefaultDarkColors()
	{
		ImGui::StyleColorsDark();
	}

	void ImGuiLayer::SetDefaultLightColors()
	{
		ImGui::StyleColorsLight();
	}

}
