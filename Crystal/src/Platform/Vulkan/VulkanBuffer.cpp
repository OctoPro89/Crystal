#include "crystalpch.h"
#include "VulkanBuffer.h"
#include "VulkanRendererAPI.h"
#include <glm/glm.hpp>

namespace Crystal
{
	static std::vector<VulkanVertexBuffer> buffersThatNeedBinding; 
	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size, VulkanDevice& device)
		: m_Device(device)
	{
		CRYSTAL_PROFILE_FUNCTION();
		VkDeviceSize bufferSize = size;
		m_Device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VertexBuffer, m_VertexBufferMemory);
		void* data;
		vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices, static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size, VulkanDevice& device)
		: m_Device(device)
	{
		CRYSTAL_PROFILE_FUNCTION();
		VkDeviceSize bufferSize = size;
		m_Device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VertexBuffer, m_VertexBufferMemory);
		void* data;
		vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, nullptr, static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		CRYSTAL_PROFILE_FUNCTION();
		vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
		vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
	}

	void VulkanVertexBuffer::Bind() const
	{
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(VulkanRendererAPI::GetInstance().GetCommandBuffers()[0], 0, 1, buffers, offsets);
	}

	void VulkanVertexBuffer::Unbind() const
	{
		CRYSTAL_PROFILE_FUNCTION();
	}
	std::vector<VkVertexInputBindingDescription> VulkanVertexBuffer::GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = ;
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}
	std::vector<VkVertexInputAttributeDescription> VulkanVertexBuffer::GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescription(1);
		attributeDescription[0].binding = 0;
		attributeDescription[0].location = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription[0].offset = 0;
		return attributeDescription;
	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		CRYSTAL_PROFILE_FUNCTION();
		void* dat;
		vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, size, 0, &dat);
		memcpy(dat, data, size);
		vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
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
