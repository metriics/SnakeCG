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
	case GLFW_KEY_SPACE:
		addSnekPart();
		break;
	case GLFW_KEY_W:
		game->direction = 0;
		break;
	case GLFW_KEY_S:
		game->direction = 1;
		break;
	case GLFW_KEY_A:
		game->direction = 2;
		break;
	case GLFW_KEY_D:
		game->direction = 3;
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
	snek.push_back(new Object(glm::vec3(0, 0, 0), glm::vec4(0, 0, 1, 1))); // head
	fruit = new Object(glm::vec3(0.5, 0, 0), glm::vec4(1, 0, 0, 1));

	// Create a new mesh from the data
	snekMeshes.push_back(snek[0]->getMesh());
	fruitMesh = fruit->getMesh();
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

	if (timer >= 10) {
		
		for (int i = snek.size() - 1; i > 0; i--) {
			snek[i]->setPosition(snek[i - 1]->getPosition());
			snekMeshes[i] = snek[i]->getMesh();
		}

		switch (direction) {
		case 0:
			snek[0]->setPosition(0, 0.05);
			break;
		case 1:
			snek[0]->setPosition(0, -0.05);
			break;
		case 2:
			snek[0]->setPosition(-0.05, 0);
			break;
		case 3:
			snek[0]->setPosition(0.05, 0);
			break;
		}

		snekMeshes[0] = snek[0]->getMesh();
		timer = 0;
	}
	else {
		timer++;
	}
}

void Game::addSnekPart() {
	glm::vec3 temp = glm::vec3(0, 0, 0);

	switch (direction) {
	case 0:
		temp = glm::vec3(0, 0.05, 0);
		break;
	case 1:
		temp = glm::vec3(0, -0.05, 0);
		break;
	case 2:
		temp = glm::vec3(-0.05, 0, 0);
		break;
	case 3:
		temp = glm::vec3(0.05, 0, 0);
		break;
	}

	snek.push_back(new Object(glm::vec3(snek[snek.size() - 1]->getPosition().x, snek[snek.size() - 1]->getPosition().y, 0) + temp, glm::vec4(0, 0, 1, 1)));
	snekMeshes.push_back(snek[snek.size()-1]->getMesh());
}
void Game::Draw(float deltaTime) {
	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

	myShader->Bind();
	for (int i = 0; i < snekMeshes.size(); i++) {
		snekMeshes[i]->Draw();
	}
	fruitMesh->Draw();
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

