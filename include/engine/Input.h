#pragma once
#include <vector>
#include "cg/cgut.h"

class Input
{
public:
	Input();

	static void poll();

	static bool getKey(int keyCode);
	static bool getKeyDown(int keyCode);
	static bool getKeyUp(int keyCode);

	static bool getMouseButton(int buttonCode);
	static bool getMouseButtonDown(int buttonCode);
	static bool getMouseButtonUp(int buttonCode);

	static void processKeyEvent(int key, int scancode, int action, int mods);
	static void processMouseClickEvent(int button, int action, int mods);
	static void processMouseMoveEvent(dvec2 pos, ivec2 windowSize);

	static dvec2 getCursorPoint();
	static dvec2 getCursorAxis();

private:
	enum KeyState {
		RELEASED = 0,
		RELEASED_ONCE,
		PRESSED,
		PRESSED_ONCE,
	};

	static const int KEY_COUNT;
	static std::vector<bool> currentKeys;
	static std::vector<KeyState> keys;

	static const int MOUSE_COUNT;
	static std::vector<bool> currentMouse;
	static std::vector<KeyState> mouse;

	static dvec2 cursorPos;
	static bool mouseMoved;
	static dvec2 currentCursorAxis;
	static dvec2 cursorAxis;
};