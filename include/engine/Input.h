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
	static void processKeyEvent(int key, int scancode, int action, int mods);

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
};