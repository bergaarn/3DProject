#pragma once

#include "Window.h"
#include <iostream>

LRESULT CALLBACK windowProc(HWND handler, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGUI

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	default:
		break;
	}

	return DefWindowProc(handler, message, wParam, lParam);
}

Window::Window()
	: width(0), height(0), title(""), windowHandler(NULL), windowClass{}, running(false)
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

	const wchar_t CLASS_NAME[] = L"WindowClass";

	ZeroMemory(&this->windowClass, sizeof(WNDCLASS));
	this->windowClass.lpfnWndProc = &windowProc;
	this->windowClass.style = CS_HREDRAW | CS_VREDRAW;
	this->windowClass.hInstance = NULL;
	this->windowClass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&this->windowClass))
	{
		// Failed to register class
		std::cerr << "windowClass failed to register, last error: " << GetLastError() << std::endl;
		return false;
	}

	// Create Window
	this->windowHandler = CreateWindowEx(0, CLASS_NAME, L"3D Project | Joel Berg | 2022", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		0, this->width, this->height, nullptr, nullptr, NULL, nullptr);

	if (this->windowHandler == nullptr)
	{
		// Failed to create window
		std::cerr << "windowHandler was nullptr, last error: " << GetLastError() << std::endl;
		return false;
	}

	ShowWindow(this->windowHandler, SW_SHOW);
	SetForegroundWindow(this->windowHandler);
	SetFocus(this->windowHandler);
	UpdateWindow(this->windowHandler);

	this->running = true;

	return true;
}

const bool& Window::isRunning()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Handle messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Receive quit message
		if (msg.message == WM_QUIT)
			this->running = false;
	}

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