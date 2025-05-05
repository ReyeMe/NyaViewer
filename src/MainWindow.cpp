#include "MainWindow.h"
#include "nya/NyaModel.h"
#include <wx/wfstream.h>
#include <wx/msgdlg.h>

MainWindow::MainWindow() :wxFrame(nullptr, wxID_ANY, "Nya Viewer")
{
	// Set aplication icon
	wxIcon appIcon;
	appIcon.LoadFile("MAINICON", wxBITMAP_TYPE_ICO_RESOURCE);

	if (appIcon.IsOk())
	{
		this->SetIcon(appIcon);
	}

	wxBitmap bitmap;
	bitmap.LoadFile("OPENICON", wxBITMAP_TYPE_BMP_RESOURCE);
	wxMask* mask = new wxMask(bitmap, wxColor(0xFFFFFF));
	bitmap.SetMask(mask);

	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(1, "&Open\tCTRL+O", "Open model file")->SetBitmap(bitmap);
	
	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	this->SetMenuBar(menuBar);

	fileMenu->Bind(wxEVT_MENU, [this](wxCommandEvent& e) { this->OpenFile(e); }, 1);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	this->modelDraw = new ModelDrawControl(this, args);
}

MainWindow::~MainWindow()
{

}

void MainWindow::OpenFile(wxCommandEvent &e)
{
	wxFileDialog openFileDialog(this, "Open model file", wxEmptyString, wxEmptyString, "Utenyaa model file (*.NYA)|*.NYA", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxFileInputStream modelStream(openFileDialog.GetPath());

		if (!modelStream.IsOk())
		{
			wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
			return;
		}

		NyaModel* modelFile = new NyaModel();
		
		if (modelFile->Open(modelStream))
		{
			this->modelDraw->LoadModel(modelFile);
		}
		else
		{
			wxLogError("Cannot load file '%s'.", openFileDialog.GetPath());
		}
	}
}