#pragma once

#include "Common.h"

#include "Camera.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "ChunkBuilder.h"

#define MAX_KEYBOARD_INPUT 512
#define MAX_MOUSE_INPUT 8

struct GLFWwindow;

class Game
{
public:
	Game();
	~Game();

	inline void Run() 
	{ 
		Init(); 
		MainLoop(); 
		Cleanup(); 
	}

	bool IsKeyPressed(int key);
	bool IsKeyReleased(int key);
	bool IsKeyDown(int key);
	bool IsKeyUp(int key);

	bool IsMouseButtonPressed(int button);
	bool IsMouseButtonReleased(int button);
	bool IsMouseButtonDown(int button);
	bool IsMouseButtonUp(int button);

	inline Vector2F GetMousePos() { return { m_currentMousePos.x - m_prevMousePos.x, m_prevMousePos.y - m_currentMousePos.y }; }

	int *GetKeyStates() { return m_keyStates.get(); }
	int *GetMouseButtonStates() { return m_mouseButtonStates.get(); }
	Vector2F *GetRawMousePos() { return &m_currentMousePos; }
	Vector2F *GetRawMouseWheel() { return &m_currentMouseWheel; }
	bool *GetCursorOnScreen() { return &m_cursorOnScreen; }
	bool *GetMouseLock() { return &m_mouseLock; }

	inline int GetWindowWidth() { return m_windowWidth; }
	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowHalfWidth() { return m_windowHalfWidth; }
	inline int GetWindowHalfHeight() { return m_windowHalfHeight; }

	inline void QuitGame() { m_quitGame = true; }
	inline bool IsQuitting() { return m_quitGame; }

	// Singleton
	inline static Game *Get() { if (!m_instance) m_instance = new Game(); return m_instance; }

private:
	void Init();
	void MainLoop();
	void Cleanup();

	void PollInput();

private:
	static Game *m_instance;

	bool m_quitGame = false;

	float m_deltaTime = 0.0f;

	GLFWwindow *m_window;
	const int m_windowWidth = 1280;
	const int m_windowHeight = 720;
	const int m_windowHalfWidth = m_windowWidth / 2;
	const int m_windowHalfHeight = m_windowHeight / 2;

	const float m_mouseSensitivity = 0.1f;
	const float m_cameraSpeed = 8.0f;

	std::unique_ptr<int[]> m_keyStates;
	std::unique_ptr<int[]> m_prevKeyStates;
	std::unique_ptr<int[]> m_mouseButtonStates;
	std::unique_ptr<int[]> m_prevMouseButtonStates;

	bool m_mouseLock = false;
	bool m_cursorOnScreen;
	Vector2F m_currentMousePos;
	Vector2F m_prevMousePos;
	Vector2F m_currentMouseWheel;
	Vector2F m_prevMouseWheel;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<ShaderManager> m_shaderManager;
	std::unique_ptr<TextureManager> m_textureManager;

	std::unique_ptr<ChunkBuilder> m_chunk;

};
