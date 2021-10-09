#include "Scene.h"
#include <learnopengl/Model.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
unsigned int diffuseMap ;
unsigned int diffuseMap_gamma;
unsigned int specularMap;

unsigned int VBO, cubeVAO;
unsigned int framebuffer;
unsigned int lightCubeVAO;
unsigned int quadVAO, quadVBO;
unsigned int textureColorbuffer;

Model ourModel;
Shader nanoShader;
Shader lightingShader;
Shader lightCubeShader;
Shader screenShader;
Shader normalShader;


void Init()
{
	diffuseMap = loadTexture("resources/pic/container2.png", false);
	diffuseMap_gamma = loadTexture("resources/pic/container2.png", true);
	specularMap = loadTexture("resources/pic/container2_specular.png", false);

	ourModel =  Model("resources/model/nano/nanosuit.obj");
	nanoShader = Shader("resources/shader/nano.vs", "resources/shader/nano.fs", "resources/shader/nano.gs");
	lightingShader = Shader("resources/shader/lighting_maps.vs", "resources/shader/lighting_maps.fs");
	lightCubeShader = Shader("resources/shader/light_cube.vs", "resources/shader/light_cube.fs");
	screenShader = Shader("resources/shader/quad.vs", "resources/shader/quad.fs");
	normalShader = Shader("resources/shader/base.vs", "resources/shader/base.fs");



	glEnable(GL_DEPTH_TEST);

	
	// first, configure the cube's VAO (and VBO)

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);


	// lighting info
// -------------
	glm::vec3 lightPositions[] = {
		glm::vec3(-3.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	};
	glm::vec3 lightColors[] = {
		glm::vec3(0.25),
		glm::vec3(0.50),
		glm::vec3(0.75),
		glm::vec3(1.00)
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	// screen quad VAO

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	// framebuffer configuration
	// -------------------------

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Draw()
{
	// per-frame time logic
		// --------------------
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;



	// render
	// ------
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
	glEnable(GL_DEPTH_TEST);


	// be sure to activate shader when setting uniforms/drawing objects
	lightingShader.use();
	lightingShader.setInt("gamma", gammaEnabled);
	lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setFloat("material.shininess", 32.0f);


	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light 
	lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


	char buffer[100];
	for (int i = 0; i < 8; i++)
	{
		snprintf(buffer, 100, "pointLights[%d].position", i);
		lightingShader.setVec3(buffer, pointLightPositions[i]);

		snprintf(buffer, 100, "pointLights[%d].ambient", i);
		lightingShader.setVec3(buffer, 0.05f, 0.05f, 0.05f);

		snprintf(buffer, 100, "pointLights[%d].diffuse", i);
		lightingShader.setVec3(buffer, 0.8f, 0.8f, 0.8f);

		snprintf(buffer, 100, "pointLights[%d].specular", i);
		lightingShader.setVec3(buffer, 1.0f, 1.0f, 1.0f);

		snprintf(buffer, 100, "pointLights[%d].constant", i);
		lightingShader.setFloat(buffer, 1.0f);

		snprintf(buffer, 100, "pointLights[%d].linear", i);
		lightingShader.setFloat(buffer, 0.09);

		snprintf(buffer, 100, "pointLights[%d].quadratic", i);
		lightingShader.setFloat(buffer, 0.032);
	}

	// spotLight
	lightingShader.setVec3("spotLight.position", camera.Position);
	lightingShader.setVec3("spotLight.direction", camera.Front);
	lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightingShader.setFloat("spotLight.constant", 1.0f);
	lightingShader.setFloat("spotLight.linear", 0.09);
	lightingShader.setFloat("spotLight.quadratic", 0.032);
	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	lightingShader.setMat4("projection", projection);
	lightingShader.setMat4("view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	lightingShader.setMat4("model", model);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glBindTexture(GL_TEXTURE_2D, gammaEnabled ? diffuseMap_gamma : diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	// render containers
	glBindVertexArray(cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}





	glm::mat4 model_nano = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model_nano = glm::scale(model_nano, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 projection_nano = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	normalShader.use();
	normalShader.setMat4("projection", projection_nano);
	normalShader.setMat4("view", view);
	normalShader.setMat4("model", model_nano);
	ourModel.Draw(normalShader);

	nanoShader.use();
	nanoShader.setMat4("model_nano", model_nano);
	nanoShader.setMat4("projection_nano", projection_nano);
	nanoShader.setMat4("view_nano", view);

	nanoShader.setVec3("viewPos_nano", camera.Position);
	nanoShader.setVec3("dirLight_nano.direction", -0.2f, -1.0f, -0.3f);
	nanoShader.setVec3("dirLight_nano.ambient", 0.05f, 0.05f, 0.05f);
	nanoShader.setVec3("dirLight_nano.diffuse", 0.4f, 0.4f, 0.4f);
	nanoShader.setVec3("dirLight_nano.specular", 0.5f, 0.5f, 0.5f);
	//nanoShader.setFloat("time", glfwGetTime());

	for (int i = 0; i < 8; i++)
	{
		snprintf(buffer, 100, "pointLights_nano[%d].position", i);
		nanoShader.setVec3(buffer, pointLightPositions[i]);

		snprintf(buffer, 100, "pointLights_nano[%d].ambient", i);
		nanoShader.setVec3(buffer, 0.05f, 0.05f, 0.05f);

		snprintf(buffer, 100, "pointLights_nano[%d].diffuse", i);
		nanoShader.setVec3(buffer, 0.8f, 0.8f, 0.8f);

		snprintf(buffer, 100, "pointLights_nano[%d].specular", i);
		nanoShader.setVec3(buffer, 1.0f, 1.0f, 1.0f);

		snprintf(buffer, 100, "pointLights_nano[%d].constant", i);
		nanoShader.setFloat(buffer, 1.0f);


		snprintf(buffer, 100, "pointLights_nano[%d].linear", i);
		nanoShader.setFloat(buffer, 0.09);

		snprintf(buffer, 100, "pointLights_nano[%d].quadratic", i);
		nanoShader.setFloat(buffer, 0.032);
	}

	// spotLight
	nanoShader.setVec3("spotLight_nano.position", camera.Position);
	nanoShader.setVec3("spotLight_nano.direction", camera.Front);
	nanoShader.setVec3("spotLight_nano.ambient", 0.0f, 0.0f, 0.0f);
	nanoShader.setVec3("spotLight_nano.diffuse", 1.0f, 1.0f, 1.0f);
	nanoShader.setVec3("spotLight_nano.specular", 1.0f, 1.0f, 1.0f);
	nanoShader.setFloat("spotLight_nano.constant", 1.0f);
	nanoShader.setFloat("spotLight_nano.linear", 0.09);
	nanoShader.setFloat("spotLight_nano.quadratic", 0.032);
	nanoShader.setFloat("spotLight_nano.cutOff", glm::cos(glm::radians(12.5f)));
	nanoShader.setFloat("spotLight_nano.outerCutOff", glm::cos(glm::radians(15.0f)));


	ourModel.Draw(nanoShader);


	// also draw the lamp object(s)
	lightCubeShader.use();
	lightCubeShader.setMat4("projection", projection);
	lightCubeShader.setMat4("view", view);
	// we now draw as many light bulbs as we have point lights.
	glBindVertexArray(lightCubeVAO);
	for (unsigned int i = 0; i < 8; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		lightCubeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader.use();
	screenShader.setInt("PostPocessCmd", PostPocessCmd);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);



}

void Clean()
{
	// optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
	{
		gammaEnabled = !gammaEnabled;
		gammaKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		gammaKeyPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		PostPocessCmd = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		PostPocessCmd = 2;
	}
	else
	{
		PostPocessCmd = 0;
	}
}