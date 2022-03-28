#!/bin/zsh
g++ -std=c++17 -o bin/main src/*.cpp src/io/*.cpp src/rendering/*.cpp src/entities/*.cpp -I/opt/homebrew/include -I/Users/emirhanyilmaz/Development/VulkanSDK/1.2.198.1/macOS/include -I/Users/emirhanyilmaz/Development/stb -L/opt/homebrew/lib -L/Users/emirhanyilmaz/Development/VulkanSDK/1.2.198.1/macOS/lib -lglfw.3.3 -lvulkan.1 -lvulkan.1.2.198
