#pragma once
#include <vector>
#include "cg/cgut.h"

class Input
{
public:
	Input();

	static void poll();

	static bool getKey(int key_code);
	static bool getKeyDown(int key_code);
	static bool getKeyUp(int key_code);

	static bool getMouseButton(int button_code);
	static bool getMouseButtonDown(int button_code);
	static bool getMouseButtonUp(int button_code);

	static void processKeyEvent(int key, int scancode, int action, int mods);
	static void processMouseEvent(int button, int action, int mods, dvec2 pos);

	static dvec2 getCursorPoint();

private:
	enum KeyState {
		RELEASED = 0,
		RELEASED_ONCE,
		PRESSED,
		PRESSED_ONCE,
	};

	static const int KEY_COUNT;
	static std::vector<bool> current_keys;
	static std::vector<KeyState> keys;

	static const int MOUSE_COUNT;
	static std::vector<bool> current_mouse;
	static std::vector<KeyState> mouse;

	static dvec2 cursor_pos;
};