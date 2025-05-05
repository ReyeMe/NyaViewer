#include "App.h"

wxIMPLEMENT_APP(Application);

Application::Application()
{

}

Application::~Application()
{

}

bool Application::OnInit()
{
	this->AppWindow = new MainWindow();
	this->AppWindow->Show();

	return true;
}