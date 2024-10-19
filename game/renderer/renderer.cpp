#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <array>
#include <vector>

#include <assert.h>
#include "glHelper.h"
#include "renderer.h"
#include "../game.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//RenderState.resolutionChange = true;
	glViewport(0, 0, width, height);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	
}

void screenDimensions(GLFWwindow* window, float& width, float& height) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	width = (float)(w);
	height = (float)(h);
}

GLFWwindow* renderer::createWindow() {
	GLFWwindow* window{ nullptr };

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(800, 600, "Fydos Client", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	framebuffer_size_callback(window, 500, 500);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void renderer::render(Game& game, RenderStateObject& RenderState, GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float dimensions[] = { 0.0f, 0.0f };
	screenDimensions(window, dimensions[0], dimensions[1]);
	float scale = game.getScale();
	float now = static_cast<float>(glfwGetTime());
	game.updateScreenSize(dimensions[0], dimensions[1]);

	//global uniforms
	glm::mat3_padded perspective{};
	perspective.set(-game.cameraX() * scale + dimensions[0] / 2, -game.cameraY() * scale + dimensions[1] / 2, 0, scale, scale);

	glBindBuffer(GL_UNIFORM_BUFFER, RenderState.uniformData);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 12, &perspective);
	if (RenderState.resolutionChange) {
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 12, sizeof(float) * 2, &dimensions);
		//RenderState.resolutionChange = false;
	}
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 14, sizeof(float) * 1, &now); //optimization: move to program that only need this
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//end setting static sutff or whatever

	RenderState.shader("default");
	unsigned int colorLoc = glGetUniformLocation(RenderState.programs["default"], "color");
	

	//tiles
	{
		glBindVertexArray(RenderState.geometry["basic"].position);
		for (const auto& [key, value] : game.chunks) {
			glm::mat3 tilePosition{};
			Chunk& chunk = game.chunks.getChunk(key);

			for (int i = 0; i < Chunk::chunkLength; i++) {
				float color[]{ 1.0, 1.0, 0.0, 1.0 };
				if (chunk[i].id() == 0) { color[0] = 0.2f; color[1] = 0.31f; color[2] = 0.51f; }
				if (chunk[i].id() == 1) { color[0] = 0.5f; color[1] = 0.5f; color[2] = 0.5f; }

				tilePosition.set(chunk.leftBorder() + chunk[i].x() * 100.0f + Constants::tileWidth / 2, chunk.topBorder() + chunk[i].y() * 100.0f + Constants::tileWidth / 2, -chunk[i].getRotation(), 100.0f, 100.0f);
				RenderState.setTransform(tilePosition);

				int type = chunk[i].geometryType();
				glUniform4fv(colorLoc, 1, color);
				//RenderState.basicGeometry[type].start, RenderState.basicGeometry[type].length
				DrawArraysIndirectCommand* cmd = new DrawArraysIndirectCommand{ (unsigned int)RenderState.basicGeometry[type].length, 1, (unsigned int)RenderState.basicGeometry[type].start, 0 };
				//glDrawArraysIndirect(GL_TRIANGLES, cmd);
				//glDrawArraysInstancedBaseInstance(GL_TRIANGLES, cmd->first, cmd->count, cmd->instanceCount, cmd->baseInstance);
				glDrawArrays(GL_TRIANGLES, cmd->first, cmd->count);
				delete cmd;
			}
		}
	}

	//players
	{
		glBindVertexArray(RenderState.geometry["box"].position);

		glm::mat3 playerTransform{};
		for (const auto& [key, value] : game.players) {
			auto position = &game.players[key]->body->position;
			playerTransform.set(position->x, position->y, 0.0f, 100.0f, 100.0f);
			RenderState.setTransform(playerTransform);

			float color[]{ 1.0, 1.0, 0.0, 1.0 };
			glUniform4fv(colorLoc, 1, color);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box"].size);

			auto bruh = game.players[key]->body->getAABB();
			float color2[]{ 1.0, 0.0, 0.0, 0.4 };
			glUniform4fv(colorLoc, 1, color2);
			playerTransform.set(bruh.min.x, bruh.min.y, 0.0f, 5.0f, 5.0f);
			RenderState.setTransform(playerTransform);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box"].size);

			playerTransform.set(bruh.max.x, bruh.max.y, 0.0f, 5.0f, 5.0f);
			RenderState.setTransform(playerTransform);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box"].size);
		}
	}

	{
		glm::mat3 cursorMatrix{};
		cursorMatrix.set(std::floor((float)game.cursorX() / 100.0f) * 100.0f, std::floor((float)game.cursorY() / 100.0f) * 100.0f, 0, 100, 100);
		RenderState.setTransform(cursorMatrix);
		float color[]{ 1.0, 0.0, 0.0, 0.3 };
		glUniform4fv(colorLoc, 1, color);
		glBindVertexArray(RenderState.geometry["box2"].position);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, RenderState.geometry["box2"].size);
	}


	glfwSwapBuffers(window);
}

//create geometry - TODO: move to another file
void initGeometry(RenderStateObject& RenderState) {
	float vertices[] = {
	  -0.5f, -0.5f,
	  -0.5f,  0.5f,
	   0.5f, -0.5f,
	   0.5f,  0.5f,
	};
	AttributeBuffer box;
	box.position = positionVAO(vertices, sizeof(vertices));
	box.size = sizeof(vertices) / (2 * sizeof(float));

	RenderState.geometry["box"] = box;

	float vertices2[] = {
	   0.0f,  0.0f,
	   0.0f,  1.0f,
	   1.0f,  0.0f,
	   1.0f,  1.0f,
	};
	AttributeBuffer box2;
	box2.position = positionVAO(vertices2, sizeof(vertices2));
	box2.size = sizeof(vertices2) / (2 * sizeof(float));

	RenderState.geometry["box2"] = box2;

	//basic geometry
	std::vector<std::vector<int>> indices{};
	indices.push_back(std::vector<int>{0, 1, 2, 0, 2, 3});
	indices.push_back(std::vector<int>{0, 1, 2});
	indices.push_back(std::vector<int>{0, 1, 2, 0, 2, 3});
	indices.push_back(std::vector<int>{0, 1, 2, 0, 2, 3});
	indices.push_back(std::vector<int>{0, 1, 2, 0, 2, 3});
	indices.push_back(std::vector<int>{0, 1, 2, 0, 2, 3});

	int totalSize = 0;
	for (std::size_t i = 0; i < indices.size(); i++) {
		RenderState.addBasicGeometry(totalSize, (int)indices[i].size());
		totalSize += (int)indices[i].size();
	}
	float* batchedVertices = new float[totalSize * 2] {};

	int location = 0;
	for (int i = 0; i < indices.size(); i++) {
		for (int j = 0; j < indices[i].size(); j++) {
			int index = indices[i][j];
			batchedVertices[2 * location] = TileGeometry::index[i][index].x;
			batchedVertices[2 * location + 1] = TileGeometry::index[i][index].y;
			location++;
		}
		std::cout << "\n\n";
	}

	AttributeBuffer basicGeometry;
	basicGeometry.position = positionVAO(batchedVertices, sizeof(float) * totalSize * 2);
	basicGeometry.size = totalSize;

	RenderState.geometry["basic"] = basicGeometry;

	delete[] batchedVertices;
}

void renderer::setupRenderer(RenderStateObject& RenderState) {
	RenderState.createProgram("default", "../data/shaders/default.vert", "../data/shaders/default.frag");

	//perspective matrix - resolution vec2 - current time uniforms
	unsigned int uboInfo;
	glGenBuffers(1, &uboInfo);
	glBindBuffer(GL_UNIFORM_BUFFER, uboInfo);
	glBufferData(GL_UNIFORM_BUFFER, (15) * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboInfo, 0, (15) * sizeof(float));

	RenderState.uniformData = uboInfo;

	initGeometry(RenderState);
}

void renderer::createRenderer(GLFWwindow* window) {

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//renderer::setupRenderer();

	while (!glfwWindowShouldClose(window)) {
		//renderer::render(window);
	}

}