#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLM/glm.hpp"

#include "Mesh.h"
#include "Shader.h"

class Game {
public:
	Game();
	~Game();

	void Run();

protected:
	void Initialize();
	void Shutdown();

	void LoadContent();
	void UnloadContent();

	void InitImGui();
	void ShutdownImGui();

	void ImGuiNewFrame();
	void ImGuiEndFrame();

	void Update(float deltaTime);
	void Draw(float deltaTime);
	void DrawGui(float deltaTime);

private:
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	// A shared pointer to our mesh
	Mesh_sptr   myMesh;
	// A shared pointer to our shader
	Shader_sptr myShader;
};