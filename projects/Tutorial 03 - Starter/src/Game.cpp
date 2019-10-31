#include "Game.h"
#include "Logging.h"

#include <stdexcept>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_glfw.cpp"

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/

void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO(message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN(message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR(message); break;
#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO(message); break;
#endif
	default: break;
	}
}

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Game::KeyPressed(GLFWwindow* window, int key) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		exit(0);
		break;
	case GLFW_KEY_W:
		if(snek[0]->getDirection() != 1)
			game->snek[0]->setDirection(0);
		break;
	case GLFW_KEY_S:
		if (snek[0]->getDirection() != 0)
			game->snek[0]->setDirection(1);
		break;
	case GLFW_KEY_A:
		if (snek[0]->getDirection() != 3)
			game->snek[0]->setDirection(2);
		break;
	case GLFW_KEY_D:
		if (snek[0]->getDirection() != 2)
			game->snek[0]->setDirection(3);
		break;
	}
}

void Game::KeyHeld(GLFWwindow* window, int key) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;
}

void Game::KeyReleased(GLFWwindow* window, int key) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	// checks key value.
	switch (key)
	{
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game != nullptr)
	{
		// checks for what type of button press happened.
		switch (action)
		{
		case GLFW_PRESS: // key has been pressed
			game->KeyPressed(window, key);
			break;

		case GLFW_REPEAT: // key is held down
			game->KeyHeld(window, key);
			break;

		case GLFW_RELEASE: // key has been released
			game->KeyReleased(window, key);
			break;
		}
	}
}


Game::Game() :
	myWindow(nullptr),
	myWindowTitle("Game"),
	myClearColor(glm::vec4(0, 0, 0, 1))
{ }

Game::~Game() { }

void Game::Run()
{
	Initialize();
	InitImGui();

	LoadContent();

	static float prevFrame = glfwGetTime();
	
	// Run as long as the window is open
	while (!glfwWindowShouldClose(myWindow)) {

		float thisFrame = glfwGetTime();
		float deltaTime = thisFrame - prevFrame;

		Update(deltaTime);
		Draw(deltaTime);

		ImGuiNewFrame();
		DrawGui(deltaTime);
		ImGuiEndFrame();

		// Present our image to windows
		glfwSwapBuffers(myWindow);

		// Poll for events from windows (clicks, keypressed, closing, all that)
		glfwPollEvents();
	}

	LOG_INFO("Shutting down...");

	UnloadContent();

	ShutdownImGui();
	Shutdown();
}

void Game::Initialize() {

	srand(time(NULL));
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}
	
	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	myWindow = glfwCreateWindow(800, 800, myWindowTitle, nullptr, nullptr);

	// Tie our game to our window, so we can access it via callbacks
	glfwSetWindowUserPointer(myWindow, this);

	// Set our window resized callback
	glfwSetWindowSizeCallback(myWindow, GlfwWindowResizedCallback);

	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(myWindow);

	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// Log our renderer and OpenGL version
	LOG_INFO(glGetString(GL_RENDERER));
	LOG_INFO(glGetString(GL_VERSION));

	// Enable debugging, and route messages to our callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, this); 
	glfwSetKeyCallback(myWindow, KeyCallback);

}

void Game::Shutdown() {
	glfwTerminate();
}

void Game::LoadContent() {
	snek.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 0, 1, 1), 0)); // head
	snekMeshes.push_back(snek[0]->getMesh());
	addSnekPart();

	fruit = new Object(glm::vec3(0, 0, 0), glm::vec4(1, 0, 0, 1), -1);
	newFruitPos(fruit);
	fruitMesh = fruit->getMesh();

	bigFruit = new Object(glm::vec3(0, 0, 0), glm::vec4(1, 1, 0, 1), -1);
	newFruitPos(bigFruit);
	bigFruitMesh = bigFruit->getMesh();

	dead.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1), -1));
	newFruitPos(dead[0]);
	deadMesh.push_back(dead[0]->getMesh());

	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("passthrough.vs", "passthrough.fs");
}

void Game::UnloadContent() {

}

void Game::InitImGui() {
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output 
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(myWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();

	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void Game::ShutdownImGui() {
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void Game::ImGuiNewFrame() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void Game::ImGuiEndFrame() {
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(myWindow, &width, &height);
	io.DisplaySize = ImVec2(width, height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(myWindow);
	}
}

void Game::Update(float deltaTime) {

	CollisionCheck();
	timer = glfwGetTime() - 0.1 * count;

	if (timer >= 0.1) {
		
		for (int i = snek.size() - 1; i >= 0; i--) {
			if (i != 0) {
				snek[i]->setDirection(snek[i - 1]->getDirection());
			}

			switch (snek[i]->getDirection()) {
			case 0:
				snek[i]->addToPosition(0, 0.05, 0);
				break;
			case 1:
				snek[i]->addToPosition(0, -0.05, 0);
				break;
			case 2:
				snek[i]->addToPosition(-0.05, 0, 0);
				break;
			case 3:
				snek[i]->addToPosition(0.05, 0, 0);
				break;
			}
			snekMeshes[i] = snek[i]->getMesh();
		}

		count++;
	}

	obTimer = glfwGetTime() - 10 * obCount;

	if (obTimer >= 10.0f) {
		dead.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1), -1));
		newFruitPos(dead[dead.size() - 1]);
		deadMesh.push_back(dead[dead.size() - 1]->getMesh());
		obCount++;
	}
}

void Game::CollisionCheck() {
	for (int i = 1; i <= snek.size() - 1; i++) {
		if (snek[i]->getPosition().x < -1.05) {
			snek[i]->setPosition(glm::vec3(1.05, snek[i]->getPosition().y, snek[i]->getPosition().z));
		}
		else if (snek[i]->getPosition().x > 1.05) {
			snek[i]->setPosition(glm::vec3(-1.05, snek[i]->getPosition().y, snek[i]->getPosition().z));
		}
		else if (snek[i]->getPosition().y < -1.05) {
			snek[i]->setPosition(glm::vec3(snek[i]->getPosition().x, 1.05, snek[i]->getPosition().z));
		}
		else if (snek[i]->getPosition().y > 1.05) {
			snek[i]->setPosition(glm::vec3(snek[i]->getPosition().x, -1.05, snek[i]->getPosition().z));
		}
	}

	for (int i = 2; i < snek.size() - 1; i++) {
		if (collisionManager.isColliding(snek[1], snek[i])) {
			resetGame();
		}
	}

	if (collisionManager.isColliding(snek[1], fruit) && whichFruit == 1) {
		addSnekPart();
		newFruitPos(fruit);
		fruitMesh = fruit->getMesh();
		whichFruit = (rand() % 2) + 1;
	}

	if (collisionManager.isColliding(snek[1], bigFruit) && whichFruit == 2) {
		addSnekPart();
		addSnekPart();
		newFruitPos(bigFruit);
		bigFruitMesh = bigFruit->getMesh();
		whichFruit = (rand() % 2) + 1;
	}

	for (int i = 0; i <= dead.size() - 1; i++) {
		if (collisionManager.isColliding(snek[1], dead[i])) {
			resetGame();
		}
	}
}

void Game::addSnekPart() {
	glm::vec3 temp = glm::vec3(0, 0, 0);

	switch (snek[snek.size() - 1]->getDirection()) {
	case 0:
		temp = glm::vec3(0, -0.05, 0);
		break;
	case 1:
		temp = glm::vec3(0, 0.05, 0);
		break;
	case 2:
		temp = glm::vec3(0.05, 0, 0);
		break;
	case 3:
		temp = glm::vec3(-0.05, 0, 0);
		break;
	}

 	snek.push_back(new Object(glm::vec3(snek[snek.size() - 1]->getPosition().x, snek[snek.size() - 1]->getPosition().y, 0) + temp, glm::vec4(0, 0, 1, 1), snek[snek.size() - 1]->getDirection()));
	snekMeshes.push_back(snek[snek.size()-1]->getMesh());
}
void Game::Draw(float deltaTime) {
	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

	myShader->Bind();
	for (int i = 1; i < snekMeshes.size(); i++) {
		snekMeshes[i]->Draw();
	}

	if (whichFruit == 1) {
		fruitMesh->Draw();
	}
	else if (whichFruit == 2) {
		bigFruitMesh->Draw();
	}

	for (int i = 0; i <= dead.size() - 1; i++) {
		deadMesh[i]->Draw();
	}
}

void Game::DrawGui(float deltaTime) {
	// Open a new ImGui window
	ImGui::Begin("Test");
	// Draw a color editor
	ImGui::ColorEdit4("Clear Color", &myClearColor[0]);
	// Check if a textbox has changed, and update our window title if it has
	if (ImGui::InputText("Window Title", myWindowTitle, 32)) {
		glfwSetWindowTitle(myWindow, myWindowTitle);
	}
	ImGui::End();

	// Open a second ImGui window
	ImGui::Begin("Debug");
	// Draw a formatted text line
	ImGui::Text("Time: %f", glfwGetTime());
	ImGui::End();
}

void Game::resetGame()
{
	snek.clear();
	snekMeshes.clear();
	snek.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 0, 1, 1), 0)); // head
	snekMeshes.push_back(snek[0]->getMesh());
	addSnekPart();

	newFruitPos(fruit);
	fruitMesh = fruit->getMesh();

	newFruitPos(bigFruit);
	bigFruitMesh = bigFruit->getMesh();

	dead.clear();
	deadMesh.clear();
	dead.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1), -1));
	newFruitPos(dead[0]);
	deadMesh.push_back(dead[0]->getMesh());
}

void Game::newFruitPos(Object* obj)
{
	float x = 0.4, y = 0.4, z = 0;
	float possibleNumbers[39] = {
								 //Negative -1 - 0
								 -0.95, -0.9, -0.85, -0.8, -0.75, -0.7, -0.65, -0.6, -0.55, -0.5,
								 -0.45, -0.4, -0.35, -0.3, -0.25, -0.2, -0.15, -0.1, -0.05, 0.0,
								 //Positive 0.05 - 1
								 0.95, 0.9, 0.85, 0.8, 0.75, 0.7, 0.65, 0.6, 0.55, 0.5,
								 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05 
								};
	//while (fmod(x, 0.05f) == 0) {
		//x = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
		int temp = rand() % 38;
		x = possibleNumbers[temp];
	//}

	//while (fmod(y, 0.05f) == 0) {
		//y = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
		temp = rand() % 38;
		y = possibleNumbers[temp];
	//}

	obj->setPosition(glm::vec3(x, y, z));
	obj->updateMesh();
}

