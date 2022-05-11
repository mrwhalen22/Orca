#pragma once
#include "Orca/Renderer/FrameBuffer.h"

namespace Orca {
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const FramebufferSpecification& GetSpecification() const override;
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		void Invalidate();
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}

