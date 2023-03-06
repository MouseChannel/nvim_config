/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-11 15:14:03
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-19 21:54:38
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Glfw_Window.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include "FrameWork/Base/baseHeader.h"
#include "FrameWork/Wrapper/Base/WrapperBase.hpp"
namespace MoChengEngine::FrameWork::Wrapper {
class Glfw_Window_my {
private:
  GLFWwindow *m_handle{NULL};
  int m_Width{800};
  int m_Height{800};

public:
  using Ptr = std::shared_ptr<Glfw_Window_my>;
  static Ptr Create() { return std::make_shared<Glfw_Window_my>(); }
  Glfw_Window_my();
  ~Glfw_Window_my();
  bool m_WindowResized{false};
  bool ShouldClose();
  void PollEvent();

  static void WindowResized(GLFWwindow *window, int width, int height);
  [[nodiscard]] GLFWwindow *Get_handle() const { return m_handle; }
};

} // namespace MoChengEngine::FrameWork::Wrapper