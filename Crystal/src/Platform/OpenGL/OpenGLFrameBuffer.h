#pragma once

#include "Crystal/Renderer/FrameBuffer.h"

namespace Crystal
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification spec);
		virtual ~OpenGLFrameBuffer() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual const FrameBufferSpecification& GetSpecification() const override {	return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { CRYSTAL_CORE_ASSERT(index < m_ColorAttachments.size(), "GetColorAttachMentRendererID index wrong"); return m_ColorAttachments[index]; }
	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}