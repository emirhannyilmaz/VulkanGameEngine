cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VulkanGameEngine VERSION 1.0)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(VulkanGameEngine ${SOURCE_FILES})
target_include_directories(VulkanGameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Vulkan REQUIRED)
target_link_libraries(VulkanGameEngine ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a Vulkan::Vulkan "-framework Cocoa -framework IOKit -framework AppKit -framework UniformTypeIdentifiers")
