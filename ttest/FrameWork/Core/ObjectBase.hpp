/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-15 09:20:44
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-28 10:46:59
 * @FilePath: \MoChengEngine\FrameWork\Core\ObjectBase.hpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once
#include <memory>
namespace MoChengEngine::FrameWork::Core {
template <typename T> class ObjectBase {
public:
  using Ptr = std::shared_ptr<T>;

  static Ptr Create(auto ...args) {

    return std::make_shared<T>(std::forward<decltype(args)>(args)...);
  }
};
} // namespace MoChengEngine::FrameWork::Core