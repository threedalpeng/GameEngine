#include "Input.h"

const int Input::KEY_COUNT = GLFW_KEY_LAST + 1;
std::vector<bool> Input::currentKeys(KEY_COUNT);
std::vector<Input::KeyState> Input::keys(KEY_COUNT);

const int Input::MOUSE_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;
std::vector<bool> Input::currentMouse(MOUSE_COUNT);
std::vector<Input::KeyState> Input::mouse(MOUSE_COUNT);

dvec2 Input::cursorPos = dvec2();
bool Input::mouseMoved = false;
dvec2 Input::currentCursorAxis = dvec2();
dvec2 Input::cursorAxis = dvec2();

Input::Input() {
	for (int i = 0; i < KEY_COUNT; i++) {
		currentKeys[i] = false;
		keys[i] = KeyState::RELEASED;
	}
	for (int i = 0; i < MOUSE_COUNT; i++) {
		currentMouse[i] = false;
		mouse[i] = KeyState::RELEASED;
	}
}

void Input::poll() {
	for (int i = 0; i < KEY_COUNT; i++) {
		if (currentKeys[i]) {
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
		if (currentMouse[i]) {
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

	if (mouseMoved) {
		cursorAxis = currentCursorAxis;
		mouseMoved = false;
	}
	else {
		cursorAxis = dvec2(0);
	}
}

bool Input::getKey(int keyCode) {
	return keys[keyCode] == KeyState::PRESSED_ONCE || keys[keyCode] == KeyState::PRESSED;
}

bool Input::getKeyDown(int keyCode) {
	return keys[keyCode] == KeyState::PRESSED_ONCE;
}

bool Input::getKeyUp(int keyCode) {
	return keys[keyCode] == KeyState::RELEASED_ONCE;
}

bool Input::getMouseButton(int buttonCode) {
	return mouse[buttonCode] == KeyState::PRESSED_ONCE || mouse[buttonCode] == KeyState::PRESSED;
}

bool Input::getMouseButtonDown(int buttonCode) {
	return mouse[buttonCode] == KeyState::PRESSED_ONCE;
}

bool Input::getMouseButtonUp(int buttonCode) {
	return mouse[buttonCode] == KeyState::RELEASED_ONCE;
}

void Input::processKeyEvent(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		currentKeys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		currentKeys[key] = false;
	}
}
void Input::processMouseClickEvent(int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		currentMouse[button] = true;
	}
	else if (action == GLFW_RELEASE) {
		currentMouse[button] = false;
	}
}

#include <iostream>
void Input::processMouseMoveEvent(dvec2 pos, ivec2 windowSize) {
	mouseMoved = true;
	dvec2 cursorChange = pos - cursorPos;
	currentCursorAxis = dvec2(
		(cursorChange.x / (static_cast<double>(windowSize.x) - 1.0)) * 2.0,
		-(cursorChange.y / (static_cast<double>(windowSize.y) - 1.0)) * 2.0
	);
	cursorPos = pos;
}

dvec2 Input::getCursorPoint() {
	return cursorPos;
}

dvec2 Input::getCursorAxis() {
	return cursorAxis;
}