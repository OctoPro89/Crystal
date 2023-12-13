#include "crystalpch.h"
#include "VulkanBuffer.h"

namespace Crystal
{
	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size, VulkanDevice& device)
		: m_Device(device)
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size, VulkanDevice& device)
		: m_Device(device)
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		CRYSTAL_PROFILE_FUNCTION();
		vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
		vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
	}
	void VulkanVertexBuffer::Bind() const
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	void VulkanVertexBuffer::Unbind() const
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	void VulkanIndexBuffer::Bind() const
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	void VulkanIndexBuffer::Unbind() const
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
}
