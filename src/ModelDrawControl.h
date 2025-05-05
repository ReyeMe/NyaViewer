#pragma once
#include <wx/wx.h>
#include <wx/glcanvas.h>

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

// Nya structures
#include "nya/NyaModel.h"

class ModelDrawControl : public wxGLCanvas
{
	struct Camera
	{
		Camera()
		{
			this->Position = NyaVertex(0.0f, 0.0f, -50.0f);
			this->Direction = NyaVertex(0.0f, 0.0f, 1.0f);
			this->Up = NyaVertex(0.0f, 1.0f, 0.0f);
		}

		NyaVertex Position;
		NyaVertex Direction;
		NyaVertex Up;
	};

	bool IsMouseDrag = false;

	wxGLContext* glContext = nullptr;

	Camera modelCamera;
	NyaModel* model = nullptr;

	void Prepare2DViewport(int x, int y, int width, int height);
	void Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

	void MouseLookAt(int dx, int dy);

	void RenderCursor();
	void RenderAxis();

	void MouseMoved(wxMouseEvent& event);
	void MouseDown(wxMouseEvent& event);
	void MouseWheelMoved(wxMouseEvent& event);
	void MouseReleased(wxMouseEvent& event);
	void RightClick(wxMouseEvent& event);
	void MouseLeftWindow(wxMouseEvent& event);
	void KeyPressed(wxKeyEvent& event);
	void KeyReleased(wxKeyEvent& event);


public:
	ModelDrawControl(wxFrame * parent, int* args);
	virtual ~ModelDrawControl();
	
	void Resized(wxSizeEvent & evt);
	
	const int GetWidth();
	const int GetHeight();
	
	void Render(wxPaintEvent & evt);
	
	void LoadModel(NyaModel* model);

	DECLARE_EVENT_TABLE()
};

