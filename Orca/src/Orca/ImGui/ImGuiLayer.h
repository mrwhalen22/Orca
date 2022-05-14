#pragma once
#include "Orca/Core/Layer.h"
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
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetBlockEvents(bool flag) { m_BlockEvents = flag; }
		bool IsBlockingEvents() { return m_BlockEvents; }
		
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};

}

