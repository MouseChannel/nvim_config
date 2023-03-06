/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-07 21:45:47
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2023-03-06 00:11:26
 * @FilePath: \MoChengEngine\main.cpp
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */

// #include "FrameWork/Base/Singleton.hpp"
#include "G_Example/app.h"

// #include "G_Example/app.cpp"
#include "FrameWork/Base/baseHeader.h"
// using PTR = MoChengEngine::FrameWork::Wrapper::VInstance::Ptr;
#include <vector>
int main(int, char **) {
  //  VK_SUCCESS

 
//   std::vector<int> a = std::vector<int>();
//   int saa = 0;
//   return 1;

  MoChengEngine::App app;
  app.Run();
  
//   app.InitVulkan();
  std::cout << "Hello, world!\n";
}
