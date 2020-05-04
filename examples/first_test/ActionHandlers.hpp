#ifndef MAGMATIC_ACTIONHANDLERS_HPP
#define MAGMATIC_ACTIONHANDLERS_HPP


#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

class ActionHandlers {
	static std::vector<std::function<void(int, int)>> keyboardObservers;
	static std::vector<std::function<void(double)>> scrollObservers;
	static std::vector<std::function<void(double, double)>> cursorObservers;

public:
	static ActionHandlers& getInstance() {
		static ActionHandlers instance;
		return instance;
	}

	static void registerKeyboardObserver(const std::function<void(int, int)>& fun);
	static void registerScrollObserver(const std::function<void(double)>& fun);
	static void registerCursorObserver(const std::function<void(double, double)>& fun);

	static void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif //MAGMATIC_ACTIONHANDLERS_HPP
