#include "Window.h"

LRESULT CALLBACK windowProc(HWND handler, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGUI

	switch (message)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
		
		break;
	default:
		break;
	}

	return DefWindowProc(handler, message, wParam, lParam);
}


Window::Window()
	:width(0), height(0), title(""), windowHandler(NULL), windowClass{}, running(false)
{
}

Window::~Window()
{
	DestroyWindow(this->windowHandler);
	UnregisterClass(this->windowClass.lpszClassName, this->windowClass.hInstance);
	this->windowHandler = 0;
}

bool Window::init(unsigned int width, unsigned int height, const std::string title)
{
	this->width = width;
	this->height = height;
	this->title = title;

	const char CLASS_NAME[] = "WindowClass";

	ZeroMemory(&this->windowClass, sizeof(WNDCLASS));
	this->windowClass.lpfnWndProc = &windowProc;
	this->windowClass.style = CS_HREDRAW | CS_VREDRAW;
	this->windowClass.hInstance = NULL;
	this->windowClass.lpszClassName = CLASS_NAME;



	return true;
}

const bool& Window::isRunning()
{
	return this->running;
}

void Window::setTitle(const std::string& title)
{
	SetWindowTextA(this->windowHandler, title.c_str());
}

void Window::quit()
{
	this->running = false;
}