#pragma once

namespace Orca {

	struct FramebufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};


	class Framebuffer {
	public:
		//virtual FramebufferSpecification& GetSpecification() = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	};


}