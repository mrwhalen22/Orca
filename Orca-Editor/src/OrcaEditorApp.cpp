

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "EditorLayer.h"

#include <Orca.h>
#include <Orca/Core/EntryPoint.h>

namespace Orca {
	class OrcaEditor : public Application {
	public:
		OrcaEditor() 
			: Application("Orca Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~OrcaEditor() {

		}
	};

	Orca::Application* Orca::CreateApplication() {
		return new OrcaEditor();
	}
}


