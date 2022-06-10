#include <string>
#include <Windows.h>

class Window
{
private:
	unsigned int width;
	unsigned int height;
	bool running;
	std::string title;

	HWND windowHandler;
	WNDCLASS windowClass;

public:
	Window();
	~Window();

	bool init(unsigned int w, unsigned int h, const std::string t);
	const bool& isRunning();
	void setTitle(const std::string& title);
	void quit();

	inline const unsigned int getWidth() { return this->width; }
	inline const unsigned int getHeight() { return this->height; }
	inline HWND& getWindowHandler() { return this->windowHandler; }
};