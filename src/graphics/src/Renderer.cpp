#include "piston/graphics/Renderer.hpp"

#include <array>
#include <stdexcept>

piston::Renderer::Renderer(Window& a_window, Device& a_device): m_window(a_window),
                            m_device(a_device)
{
	o_recreateSwapChain();
	o_createCommandBuffers();
}

piston::Renderer::~Renderer()
{
	o_freeCommandBuffers();
}

VkCommandBuffer piston::Renderer::beginFrame()
{
	assert(!isFrameStarted && "Can't call begin frame while already in progress");
	auto result = m_swapChain->acquireNextImage(&currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		o_recreateSwapChain();
		return nullptr;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failes to acquire swap chain image");
	}

	isFrameStarted = true;
	auto l_commandBuffer = o_getCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(l_commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer");
	}

	return l_commandBuffer;
}

void piston::Renderer::endFrame()
{
	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
	auto l_commandBuffer = o_getCurrentCommandBuffer();

	if (vkEndCommandBuffer(l_commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer");
	}

	auto result = m_swapChain->submitCommandBuffers(&l_commandBuffer, &currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.o_wasWindowResized())
	{
		m_window.o_resetWindowResizedFlag();
		o_recreateSwapChain();
	}

	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image");
	}

	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void piston::Renderer::beginSwapChainRenderPass(VkCommandBuffer a_commandBuffer)
{
	assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress");
	assert(a_commandBuffer == o_getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_swapChain->getRenderPass();
	renderPassInfo.framebuffer = m_swapChain->getFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = m_swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(a_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(m_swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0, 0}, m_swapChain->getSwapChainExtent()};
	vkCmdSetViewport(a_commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(a_commandBuffer, 0, 1, &scissor);
}

void piston::Renderer::endSwapChainRenderPass(VkCommandBuffer a_commandBuffer)
{
	assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress");
	assert(a_commandBuffer == o_getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

	vkCmdEndRenderPass(a_commandBuffer);

}

void piston::Renderer::o_createCommandBuffers()
{
	m_commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers");
	}
}

void piston::Renderer::o_freeCommandBuffers()
{
	vkFreeCommandBuffers(m_device.device(), m_device.getCommandPool(), m_commandBuffers.size(),
	                     m_commandBuffers.data());
	m_commandBuffers.clear();
}

void piston::Renderer::o_recreateSwapChain()
{
	auto l_extent = m_window.o_getExtent();
	while (l_extent.width == 0 || l_extent.height == 0)
	{
		l_extent = m_window.o_getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_device.device());

	m_swapChain = nullptr; // do not delete, or for some reason it crashes when resizing the window
	m_swapChain = std::make_unique<SwapChain>(m_device, l_extent);

	if (m_swapChain == nullptr)
	{
		m_swapChain = std::make_unique<SwapChain>(m_device, l_extent);
	}
	else
	{
		std::shared_ptr<SwapChain> l_oldSwapChain = std::move(m_swapChain);
		m_swapChain = std::make_unique<SwapChain>(m_device, l_extent, l_oldSwapChain);

		if (!l_oldSwapChain->compareSwapFormat(*m_swapChain))
		{
			throw std::runtime_error("Swap chain image format has changed");
		}
	}

	//
}