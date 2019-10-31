#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLM/glm.hpp"

#include "Mesh.h"
#include "Shader.h"
#include <vector>
#include "Object.h"
#include "Collision.h"

class Game {
public:
	Game();
	~Game();

	void Run();

	// called when a key has been pressed
	void KeyPressed(GLFWwindow* window, int key);

	// called when a key is being held down
	void KeyHeld(GLFWwindow* window, int key);

	// called when a key has been released
	void KeyReleased(GLFWwindow* window, int key);

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
	void CollisionCheck();
	void Draw(float deltaTime);
	void addSnekPart();
	void DrawGui(float deltaTime);

private:
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	Collision collisionManager;

	std::vector<Object*> snek;
	Object* fruit; //red
	Object* bigFruit; //yellow
	Object* dead; //green

	float timer = 0;

	// A shared pointer to our mesh
	Mesh_sptr   myMesh;
	std::vector<Mesh_sptr> snekMeshes;
	Mesh_sptr fruitMesh;
	Mesh_sptr bigFruitMesh;
	Mesh_sptr deadMesh;
	

	// A shared pointer to our shader
	Shader_sptr myShader;

	bool w, a, s, d = false;
};