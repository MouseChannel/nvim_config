/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-12 09:32:00
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-30 21:33:31
 * @FilePath: \MoChengEngine\FrameWork\Wrapper\Component\ComponentBase.hpp
 * @Description: nullptr
 * 
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved. 
 */
#pragma once
namespace MoChengEngine::FrameWork::Wrapper {
template <typename T> class ComponentBase {

  [[nodiscard]] virtual T Get_handle() const = 0;
};

} // namespace MoChengEngine::FrameWork::Wrapper