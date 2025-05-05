#pragma once

#include <wx/wx.h>
#include <wx/ribbon/bar.h>

#include "ModelDrawControl.h"

class MainWindow : public wxFrame
{
private:

	ModelDrawControl* modelDraw = nullptr;

	void OpenFile(wxCommandEvent &e);

public:
	MainWindow();
	~MainWindow();
};

