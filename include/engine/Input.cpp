#include "Input.h"

const int Input::KEY_COUNT = GLFW_KEY_LAST + 1;
std::vector<bool> Input::current_keys(KEY_COUNT);
std::vector<Input::KeyState> Input::keys(KEY_COUNT);

const int Input::MOUSE_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;
std::vector<bool> Input::current_mouse(MOUSE_COUNT);
std::vector<Input::KeyState> Input::mouse(MOUSE_COUNT);

dvec2 Input::cursor_pos = dvec2();

Input::Input() {
	for (int i = 0; i < KEY_COUNT; i++) {
		current_keys[i] = false;
		keys[i] = KeyState::RELEASED;
	}
	for (int i = 0; i < MOUSE_COUNT; i++) {
		current_mouse[i] = false;
		mouse[i] = KeyState::RELEASED;
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
	for (int i = 0; i < MOUSE_COUNT; i++) {
		if (current_mouse[i]) {
			if (mouse[i] == KeyState::RELEASED) {
				mouse[i] = KeyState::PRESSED_ONCE;
			}
			else {
				mouse[i] = KeyState::PRESSED;
			}
		}
		else {
			if (mouse[i] == KeyState::PRESSED) {
				mouse[i] = KeyState::RELEASED_ONCE;
			}
			else {
				mouse[i] = KeyState::RELEASED;
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

bool Input::getMouseButton(int button_code) {
	return mouse[button_code] == KeyState::PRESSED_ONCE || keys[button_code] == KeyState::PRESSED;
}

bool Input::getMouseButtonDown(int button_code) {
	return mouse[button_code] == KeyState::PRESSED_ONCE;
}

bool Input::getMouseButtonUp(int button_code) {
	return mouse[button_code] == KeyState::RELEASED_ONCE;
}

void Input::processKeyEvent(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		current_keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		current_keys[key] = false;
	}
}

void Input::processMouseEvent(int button, int action, int mods, dvec2 pos) {
	cursor_pos = pos;
	if (action == GLFW_PRESS) {
		current_mouse[button] = true;
	}
	else if (action == GLFW_RELEASE) {
		current_mouse[button] = false;
	}
}

dvec2 Input::getCursorPoint() {
	return cursor_pos;
}