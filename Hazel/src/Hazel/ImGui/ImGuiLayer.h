#pragma once
#include "Hazel/Layer.h"


namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate();
		void OnEvent(Event& e);
	private:
		float m_Time = 0.0f;

	};

}

