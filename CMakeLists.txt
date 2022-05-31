cmake_minimum_required(VERSION 3.21)

project(RenderGraph VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)

add_subdirectory(src)




