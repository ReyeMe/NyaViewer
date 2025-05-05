#pragma once

#include <wx/wx.h>
#include "MainWindow.h"

class Application : public wxApp
{
public:
	wxFrame* AppWindow = nullptr;

	Application();
	~Application();

	bool OnInit();
};

