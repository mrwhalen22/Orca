#pragma once
#include "Orca/Core.h"

namespace Orca {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};

}