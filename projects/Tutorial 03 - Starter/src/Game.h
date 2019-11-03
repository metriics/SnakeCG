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
	void Initialize(); // init glfw, set key callback
	void Shutdown(); // terminate glfw

	void LoadContent(); // load in all mesh types and link shader
	void UnloadContent(); // null

	void InitImGui(); // unused for this project
	void ShutdownImGui(); // unused for this project

	void ImGuiNewFrame(); // unused for this project
	void ImGuiEndFrame(); // unused for this project

	void Update(float deltaTime); // check collision, change snek dir & update pos, fruit and obstacle spawn timer
	void CollisionCheck(); // collision check logic, called by Update()
	void Draw(float deltaTime); // set clear color and clear, bind shader & draw meshes 
	void addSnekPart(); // called when adding a snek part when score increases
	void addScoreDot(); // called when score increases
	void DrawGui(float deltaTime); // unused for this project

	void resetGame(); // called upon death (run into yourself or an obstacle)
	void newFruitPos(Object* obj); // set position of passed obj to new random position and update verts in mesh, used for fruits and obstacles

private:
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	Collision collisionManager;

	std::vector<Object*> snek; // vector of snek parts, head at i = 1. i = 0 unused and not drawn
	Object* fruit; // red, increases score by 1 when collided with
	Object* bigFruit; // yellow, increases score by two when collided with
	std::vector<Object*> dead; // green, resets game andscore when collided with
	std::vector<ScoreDot*> scoreDot; // white, represents score

	float timer = 0; // update timer
	float obTimer = 0; // obstacle update timer
	float count = 0; // amount of times timer has hit 0.1
	float obCount = 0; // amount of times obTimer has hit 10. Should be equal to amount of obstacles on screen
	int whichFruit = 1; //1 reg fruit, 2 big fruit
	int score = 0; // player score

	// shared pointers to our meshes
	std::vector<Mesh_sptr> snekMeshes;
	Mesh_sptr fruitMesh;
	Mesh_sptr bigFruitMesh;
	std::vector<Mesh_sptr> deadMesh;
	std::vector<Mesh_sptr> scoreDotMesh;
	

	// A shared pointer to our shader
	Shader_sptr myShader;

	bool w, a, s, d = false; // whether or not w a s d keys are held
};