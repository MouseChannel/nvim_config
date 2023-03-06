/*
 * @Author: mousechannel mochenghh@gmail.com
 * @Date: 2022-11-08 12:26:03
 * @LastEditors: mousechannel mochenghh@gmail.com
 * @LastEditTime: 2022-12-23 08:15:51
 * @FilePath: \MoChengEngine\FrameWork\Base\baseHeader.h
 * @Description: nullptr
 *
 * Copyright (c) 2022 by mousechannel mochenghh@gmail.com, All Rights Reserved.
 */
#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <iostream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "error.h"
#include <GLFW/glfw3.h>
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};
const static int Width = 800;

const static int Height = 800;


// #define Using_VMA