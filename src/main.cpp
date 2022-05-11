#include <OpenGL.hpp>

#include <Setup.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Model.hpp>

#include <iostream>

float constexpr vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 constexpr cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f), 
	glm::vec3( 2.0f,  5.0f, -15.0f), 
	glm::vec3(-1.5f, -2.2f, -2.5f),  
	glm::vec3(-3.8f, -2.0f, -12.3f),  
	glm::vec3( 2.4f, -0.4f, -3.5f),  
	glm::vec3(-1.7f,  3.0f, -7.5f),  
	glm::vec3( 1.3f, -2.0f, -2.5f),  
	glm::vec3( 1.5f,  2.0f, -2.5f), 
	glm::vec3( 1.5f,  0.2f, -1.5f), 
	glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 constexpr lightColor(1.0f, 1.0f, 1.0f);
#define NR_POINT_LIGHTS 4
glm::vec3 constexpr pointLightPositions[NR_POINT_LIGHTS] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};

int WIN_WIDTH = 800, WIN_HEIGHT = 600;

float cameraSpeed = 5.0f;
bool wire_frame = false;

// create camera (see Camera.hpp)
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f);
Camera *current_camera = &camera;

int main()
{
	GLFWwindow *window = setup();
		if (window == nullptr) return -1;
	
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	glm::vec3 lightColor(1.0f);
	
	// create shader program for box
	Shader shader("res/shaders/complete_lighting.vert", "res/shaders/complete_lighting.frag");
		// set values for uniforms
		shader.setFloat("material.shininess", 32.0f);

		shader.setVec3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shader.setVec3f("dirLight.ambient", lightColor * glm::vec3(0.02f));
		shader.setVec3f("dirLight.diffuse", lightColor * glm::vec3(0.5f));
		shader.setVec3f("dirLight.specular", glm::vec3(1.0f));
		
		for (int i = 0; i < NR_POINT_LIGHTS; i++){
			std::string name = "pointLights[" + std::to_string(i) + "].";
			shader.setVec3f(name + "position", pointLightPositions[i]);
			shader.setVec3f(name + "ambient", lightColor * glm::vec3(0.02f));
			shader.setVec3f(name + "diffuse", lightColor * glm::vec3(0.5f));
			shader.setVec3f(name + "specular", glm::vec3(1.0f));
			shader.setVec3f(name + "attenuation", glm::vec3(1.0f, 0.09f, 0.32f));
		}

		shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));
		shader.setVec3f("spotLight.ambient", lightColor * glm::vec3(0.02f));
		shader.setVec3f("spotLight.diffuse", lightColor * glm::vec3(0.5f));
		shader.setVec3f("spotLight.specular", glm::vec3(1.0f));
		shader.setVec3f("spotLight.attenuation", glm::vec3(1.0f, 0.09f, 0.032f));
	
	// create shader program for light
	Shader lightbox_shader("res/shaders/light_cube.vert", "res/shaders/light_cube.frag");
		// set values for uniforms
		lightbox_shader.setVec3f("lightColor", lightColor);

	// set up textures
	Texture diffuseMap("res/textures/container2.png");
		diffuseMap.bind(shader);
	Texture specularMap("res/textures/container2_specular.png");
		specularMap.bind(shader);
	
	// set up models
	Model nanosuit("res/models/nanosuit/nanosuit.obj");
	
	// set up vertex buffer object for a cube
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	// set up vertex array object for box
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. then set the vertex attribute pointers and ENABLE THEN FOR GOD'S SAKE!
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
		glEnableVertexAttribArray(2);		
		// 4. unbind Vertex Array Object
		glBindVertexArray(0);

	// set up vertex array object for light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
		// 1. bind Vertex Array Object
		glBindVertexArray(lightVAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. then set the vertex attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
		glEnableVertexAttribArray(0);
		// 4. unbind Vertex Array Object
		glBindVertexArray(0);

	// print version of OpenGL being used
	std::cout << glGetString(GL_VERSION) << '\n';
	start_fps();
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)){
	// clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// render box
		// activate the box shader
		shader.use();
		// do calculations and update uniforms and render
		glm::mat4 view = current_camera->view();
		glm::mat4 projection = glm::perspective(glm::radians(current_camera->fov), ((float) WIN_WIDTH) / WIN_HEIGHT, 0.1f, 100.0f);
		
		shader.setVec3f("spotLight.position", current_camera->position);
		shader.setVec3f("spotLight.direction", current_camera->cameraFront);
		
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3f("viewPos", current_camera->position);
		diffuseMap.setUniform(shader, "material.texture_diffuse0");
		specularMap.setUniform(shader, "material.texture_specular0");
		
		// now render
		for (int i = 0; i < 10; i++){
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			
			glm::mat4 normal_matrix(glm::mat3(glm::transpose(glm::inverse(model))));
			shader.setMat3("normal_matrix", normal_matrix);
			
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
	// render model
		// activate the shader
		shader.use();
		glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)), glm::vec3(0.0f, 5.0f, 0.0f));
		shader.setMat4("model", model);
		
		nanosuit.Draw(shader);
		
	// render lightboxes
		// activate the light shader
		lightbox_shader.use();
		// do calculations and update uniforms
		lightbox_shader.setMat4("view", view);
		lightbox_shader.setMat4("projection", projection);
		for (int i = 0; i < NR_POINT_LIGHTS; i++){
			model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightbox_shader.setMat4("model", model);
			// now render
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);	
		}

		
	// process input
		processInput(window);
		
	// check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	// update deltaTime
		update_fps();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	
	glfwTerminate();
	
	std::cout << avg_fps() << '\n';
	return 0;
}
