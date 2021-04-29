#include "Input.h"

const int Input::KEY_COUNT = GLFW_KEY_LAST + 1;
std::vector<bool> Input::current_keys(KEY_COUNT);
std::vector<Input::KeyState> Input::keys(KEY_COUNT);

Input::Input() {
	for (int i = 0; i < KEY_COUNT; i++) {
		current_keys[i] = false;

		keys[i] = KeyState::RELEASED;
	}
}

void Input::poll() {
	for (int i = 0; i < KEY_COUNT; i++) {
		if (current_keys[i]) {
			if (keys[i] == KeyState::RELEASED) {
				keys[i] = KeyState::PRESSED_ONCE;
			}
			else {
				keys[i] = KeyState::PRESSED;
			}
		}
		else {
			if (keys[i] == KeyState::PRESSED) {
				keys[i] = KeyState::RELEASED_ONCE;
			}
			else {
				keys[i] = KeyState::RELEASED;
			}
		}
	}
}

bool Input::getKey(int key_code) {
	return keys[key_code] == KeyState::PRESSED_ONCE || keys[key_code] == KeyState::PRESSED;
}

bool Input::getKeyDown(int key_code) {
	return keys[key_code] == KeyState::PRESSED_ONCE;
}

bool Input::getKeyUp(int key_code) {
	return keys[key_code] == KeyState::RELEASED_ONCE;
}

void Input::processKeyEvent(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		current_keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		current_keys[key] = false;
	}
}