#include "Game.h"

#include "Math.h"

#include "Blocks.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Callback Definitions
void KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
void CursorEnterCallback(GLFWwindow *window, int entered);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

Game *Game::m_instance = nullptr;
Game::Game()
{
	spdlog::info("Created Game Instance.");
}
Game::~Game()
{
	spdlog::info("Destroyed Game Instance.");
}

void Game::Init()
{
	spdlog::trace("Hello, World!");

	// Initialize GLFW
	if (!glfwInit())
		throw std::runtime_error("Could not initialize GLFW!");
	spdlog::info("Initialized GLFW.");

	// Create Window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MC_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Minecraft", nullptr, nullptr);
	if (!m_window)
	{
		Cleanup();
		throw std::runtime_error("Failed to create GLFW window!");
	}
	spdlog::info("Created GLFW window.");

	// Create Context and Load OpenGL
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Cleanup();
		throw std::runtime_error("Failed to load Glad!");
	}
	spdlog::info("Loaded Glad.");

	// Setup Input & Callbacks
	if (glfwRawMouseMotionSupported()) // Set Raw Input
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	m_cursorOnScreen = false;
	m_mouseLock = false;
	m_currentMousePos = { 0, 0 };
	m_prevMousePos = { 0, 0 };
	m_currentMouseWheel = { 0, 0 };
	m_prevMouseWheel = { 0, 0 };

	m_keyStates = std::make_unique<int[]>(MAX_KEYBOARD_INPUT);
	m_prevKeyStates = std::make_unique<int[]>(MAX_KEYBOARD_INPUT);
	m_mouseButtonStates = std::make_unique<int[]>(MAX_MOUSE_INPUT);
	m_prevMouseButtonStates = std::make_unique<int[]>(MAX_MOUSE_INPUT);

	glfwSetKeyCallback(m_window, KeyboardCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	glfwSetCursorPosCallback(m_window, CursorPositionCallback);
	glfwSetCursorEnterCallback(m_window, CursorEnterCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);

	// Setup Renderer & Viewport
	glfwSwapInterval(1); // V-Sync

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	int viewportWidth, viewportHeight;
	glfwGetFramebufferSize(m_window, &viewportWidth, &viewportHeight);
	glViewport(0, 0, viewportWidth, viewportHeight);

	// Initialize Game
	m_shaderManager = std::make_unique<ShaderManager>();
	m_shaderManager->AddShader("BASIC_SHADER", new Shader("./assets/shaders/v_basic.glsl", "./assets/shaders/f_basic.glsl"));

	m_textureManager = std::make_unique<TextureManager>();
	m_textureManager->AddTexture("TERRAIN", new Texture("./assets/textures/terrain.png"));

	m_camera = std::make_unique<Camera>(90.0f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);
	m_camera->position = { -8.0f, 32.0f, 8.0f };

	m_chunk = std::make_unique<ChunkBuilder>(m_shaderManager->GetShader("BASIC_SHADER"), m_textureManager->GetTexture("TERRAIN"));
	m_chunk->Create();
	m_chunk->CreateMesh();

	//
	spdlog::info("Initialized Game.");
}

float lastFrame = 0.0f;
void Game::MainLoop()
{
	spdlog::info("Starting Game Loop.");
	while (!(glfwWindowShouldClose(m_window) || m_quitGame))
	{
		float currentFrame = (float)glfwGetTime();
		m_deltaTime = currentFrame - lastFrame;

		glClearColor(0.72f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			if (IsKeyPressed(GLFW_KEY_ESCAPE) && !m_mouseLock)
				QuitGame();

			// Camera Lock
			if (m_cursorOnScreen && IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				m_mouseLock = true;
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			if (IsKeyPressed(GLFW_KEY_ESCAPE) && m_mouseLock)
			{
				m_mouseLock = false;
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			// Camera Movement
			if (m_mouseLock)
			{
				int horizontal = (int)IsKeyDown(GLFW_KEY_D) - (int)IsKeyDown(GLFW_KEY_A);
				int vertical = (int)IsKeyDown(GLFW_KEY_W) - (int)IsKeyDown(GLFW_KEY_S);

				if (IsKeyDown(GLFW_KEY_SPACE))
					m_camera->position += m_cameraSpeed * m_camera->up * m_deltaTime;
				if (IsKeyDown(GLFW_KEY_LEFT_SHIFT))
					m_camera->position -= m_cameraSpeed * m_camera->up * m_deltaTime;
				m_camera->position += (m_cameraSpeed * vertical) * m_camera->front * m_deltaTime;
				m_camera->position += (m_cameraSpeed * horizontal) * m_camera->right * m_deltaTime;

				m_camera->yaw += GetMousePos().x * m_mouseSensitivity;
				m_camera->pitch += GetMousePos().y * m_mouseSensitivity;

				if (m_camera->pitch > 89.0f)
					m_camera->pitch = 89.0f;
				if (m_camera->pitch < -89.0f)
					m_camera->pitch = -89.0f;
			}

			// Update Scene
			m_camera->Update();
			m_shaderManager->Update(*m_camera);

			m_chunk->Update(m_deltaTime);
			m_chunk->Draw();
		}
		glfwSwapBuffers(m_window);
		PollInput();
		glfwPollEvents();

		lastFrame = currentFrame;
	}
}
void Game::PollInput()
{
	for (int i = 0; i < MAX_KEYBOARD_INPUT; i++) m_prevKeyStates[i] = m_keyStates[i];
	for (int i = 0; i < MAX_MOUSE_INPUT; i++) m_prevMouseButtonStates[i] = m_mouseButtonStates[i];
	m_prevMouseWheel = m_currentMouseWheel;
	m_currentMouseWheel = { 0.0f, 0.0f };
	m_prevMousePos = m_currentMousePos;
}

void Game::Cleanup()
{
	spdlog::info("Cleaning up.");

	if (m_window)
		glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Game::IsKeyPressed(int key)
{
	bool pressed = false;
	if ((m_prevKeyStates[key] == GLFW_RELEASE) && (m_keyStates[key] == GLFW_PRESS)) pressed = true;
	return pressed;
}
bool Game::IsKeyReleased(int key)
{
	bool released = false;
	if ((m_prevKeyStates[key] == GLFW_PRESS) && (m_keyStates[key] == GLFW_RELEASE)) released = true;
	return released;
}
bool Game::IsKeyDown(int key)
{
	if (m_keyStates[key] == GLFW_PRESS) return true;
	else return false;
}
bool Game::IsKeyUp(int key)
{
	if (m_keyStates[key] == GLFW_RELEASE) return true;
	else return false;
}

bool Game::IsMouseButtonPressed(int button)
{
	bool pressed = false;
	if ((m_mouseButtonStates[button] == GLFW_PRESS) && (m_prevMouseButtonStates[button] == GLFW_RELEASE)) pressed = true;
	return pressed;
}
bool Game::IsMouseButtonReleased(int button)
{
	bool released = false;
	if ((m_mouseButtonStates[button] == GLFW_RELEASE) && (m_prevMouseButtonStates[button] == GLFW_PRESS)) released = true;
	return released;
}
bool Game::IsMouseButtonDown(int button)
{
	if (m_mouseButtonStates[button] == GLFW_PRESS) return true;
	else return false;
}
bool Game::IsMouseButtonUp(int button)
{
	if (m_mouseButtonStates[button] == GLFW_RELEASE) return true;
	else return false;
}

void KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Game *game = Game::Get();
	int *keyStates = game->GetKeyStates();

	if (action == GLFW_PRESS)
		keyStates[key] = GLFW_PRESS;
	else if (action == GLFW_RELEASE)
		keyStates[key] = GLFW_RELEASE;
}
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	Game *game = Game::Get();
	int *buttonStates = game->GetMouseButtonStates();

	buttonStates[button] = action;
}
void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	Game *game = Game::Get();
	Vector2F *mousePos = game->GetRawMousePos();
	mousePos->x = (float)xpos;
	mousePos->y = (float)ypos;
}
void CursorEnterCallback(GLFWwindow *window, int entered)
{
	Game *game = Game::Get();
	bool &mouseOnScreen = *game->GetCursorOnScreen();
	if (entered) mouseOnScreen = true;
	else mouseOnScreen = false;
}
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	Game *game = Game::Get();
	Vector2F *mouseWheel = game->GetRawMouseWheel();
	mouseWheel->x = (float)xoffset;
	mouseWheel->y = (float)yoffset;
}
