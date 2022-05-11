#include <OpenGL.hpp>

#include <Camera.hpp>

#include <iostream>

extern int WIN_WIDTH, WIN_HEIGHT;

// deltaTime and fps
static float deltaTime = 0.0f;
static float firstFrame;
static float lastFrame;
static float currentFrame;
static int no_of_frames = 0;

// cameras
extern Camera *current_camera;
extern bool wire_frame;
extern float cameraSpeed;

// mouse input
static float lastX = WIN_WIDTH / 2, lastY = WIN_HEIGHT / 2;
static bool firstMouse = true;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		current_camera->forward(cameraSpeed * deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		current_camera->forward(-cameraSpeed * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		current_camera->strafeRight(-cameraSpeed * deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		current_camera->strafeRight(cameraSpeed * deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int p_width, int p_height)
{
	glViewport(0, 0, p_width, p_height);
	WIN_WIDTH = p_width, WIN_HEIGHT = p_height;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float const sensitivity_x = (360.0f / WIN_WIDTH), sensitivity_y = (180.0 / WIN_HEIGHT); // change these to your liking
	float xoffset = (xpos  - lastX) * sensitivity_x;
	float yoffset = (lastY - ypos) * sensitivity_y;
	lastX = xpos, lastY = ypos;

	current_camera->updateOrientation(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	current_camera->updateFov(-(float)yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// exiting program
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	// switching to line rendering mode
	else if (key == GLFW_KEY_T && action == GLFW_PRESS){
		wire_frame = !wire_frame;
		if (wire_frame){
			glDisable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

GLFWwindow * setup()
{
	if (!glfwInit()){
		std::cout << "GLFW failed to initialize\n";
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a windowed mode window and its OpenGL context
	GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello World", NULL, NULL);
	if (window == nullptr){
		std::cout << "GLFW failed to create a window\n";
		glfwTerminate();
		return nullptr;
	}
	
	// make the window's context current
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "GLAD failed to initialize\n";
		glfwTerminate();
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
//	glfwSwapInterval(1);
	
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	
	return window;
}

void start_fps()
{
	firstFrame = glfwGetTime();
	lastFrame = firstFrame;
}

void update_fps()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	no_of_frames += 1;
}

float avg_fps()
{
	return no_of_frames / (lastFrame - firstFrame);
}
