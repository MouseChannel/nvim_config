/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-14 21:37:39
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-09 15:43:35
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\FrameBuffer.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#include "FrameBuffer.h"
#include "FrameWork/Wrapper/Command/CommandQueue.h"

namespace MoChengEngine::FrameWork::Wrapper
{
    FrameBuffer::FrameBuffer(Device::Ptr device, VkExtent2D extent,
                             std::vector<VkImageView> attachments,
                             RenderPass::Ptr renderPass)
        : Component<VkFrameBuffer, FrameBuffer>{device}
    {

        VkFramebufferCreateInfo frameBufferCreateInfo{};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.renderPass = renderPass->Get_handle();
        frameBufferCreateInfo.attachmentCount =
            static_cast<uint32_t>(attachments.size());
        frameBufferCreateInfo.pAttachments = attachments.data();
        frameBufferCreateInfo.width = extent.width;
        frameBufferCreateInfo.height = extent.height;
        frameBufferCreateInfo.layers = 1;
        VK_CHECK_SUCCESS(vkCreateFramebuffer(device->Get_handle(), &frameBufferCreateInfo, nullptr,
                                             &m_handle),
                         "create frameBuffer failed");
    }
    FrameBuffer::~FrameBuffer()
    {
        vkDestroyFramebuffer(m_device->Get_handle(), m_handle, nullptr);
    }
} // namespace MoChengEngine::FrameWork::Wrapper