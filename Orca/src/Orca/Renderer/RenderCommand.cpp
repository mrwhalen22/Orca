#include "oapch.h"
#include "RenderCommand.h"

#include "Orca/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orca
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;




}