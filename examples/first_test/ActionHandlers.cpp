#include "ActionHandlers.hpp"

std::vector<std::function<void(int, int)>> ActionHandlers::keyboardObservers;
std::vector<std::function<void(double)>> ActionHandlers::scrollObservers;
std::vector<std::function<void(double, double)>> ActionHandlers::cursorObservers;

void ActionHandlers::keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods) {
	for(const auto& obs : keyboardObservers) {
		obs(key, action);
	}
}

void ActionHandlers::registerKeyboardObserver(const std::function<void(int, int)>& fun) {
	keyboardObservers.emplace_back(fun);
}
void ActionHandlers::registerScrollObserver(const std::function<void(double)>& fun) {
	scrollObservers.emplace_back(fun);
}
void ActionHandlers::registerCursorObserver(const std::function<void(double, double)>& fun) {
	cursorObservers.emplace_back(fun);
}
