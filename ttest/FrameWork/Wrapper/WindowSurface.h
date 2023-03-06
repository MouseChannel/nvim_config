/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 20:10:22
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-16 13:11:42
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\WindowSurface.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "./Instance/Instance.h"
#include "FrameWork/Base/baseHeader.h"
#include "Glfw_Window.h"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"

namespace MoChengEngine::FrameWork::Wrapper {
class WindowSurface : public WrapperBase<VkSurfaceKHR, WindowSurface> {
private:
  //   VkSurfaceKHR m_Handle{VK_NULL_HANDLE};
  Instance::Ptr m_instance;
  Glfw_Window_my::Ptr m_window;

public:
  WindowSurface(Instance::Ptr instance, Glfw_Window_my ::Ptr window);
  ~WindowSurface();
  //   [[nodiscard]] VkSurfaceKHR Get_handle() const { return m_Handle; }
  [[nodiscard]] auto Get_window() const { return m_window; }
};

} // namespace MoChengEngine::FrameWork::Wrapper