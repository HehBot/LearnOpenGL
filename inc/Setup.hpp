#pragma once

#include <OpenGL.hpp>

#include <Camera.hpp>

#include <iostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int p_width, int p_height);

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

GLFWwindow * setup();

void start_fps();
void update_fps();
float avg_fps();
