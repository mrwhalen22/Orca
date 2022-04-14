#pragma once
#include "Orca/Core/Window.h"
#include "Orca/Core/Log.h"
#include "Orca/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace Orca {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline virtual unsigned int GetWidth() const override { return m_Data.Width; }
		inline virtual unsigned int GetHeight() const override { return m_Data.Height; }
		inline virtual void* GetNativeWindow() const override { return m_Window; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;


		

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width = 0, Height = 0;
			bool VSync = false;

			EventCallbackFn EventCallback;

		};

		WindowData m_Data;
		GraphicsContext* m_Context;

	};

}

