#pragma once
#include "Orca/Layer.h"
#include "Orca/Events/MouseEvent.h"
#include "Orca/Events/KeyEvent.h"
#include "Orca/Events/ApplicationEvent.h"


namespace Orca {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
		
	private:
		float m_Time = 0.0f;

	};

}

