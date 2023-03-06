/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-08 15:00:38
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-11-09 11:23:51
 * @FilePath: \MoChengEngine\FrameWork\Base\Component.hpp
 * @Description: nullptr
 * 
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved. 
 */
#pragma once
#include <memory>
#include <utility>
template <typename T> class Component {
private:
protected:
public:
  using Ptr = std::shared_ptr<T>;
  static Ptr Create(auto&... args) {
    return std::make_shared<T>(std::forward<decltype(args)>(args)...);
  }
};

;