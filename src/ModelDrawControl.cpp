#include "ModelDrawControl.h"

#if defined(__WXGTK__)
#define HIDE_CURSOR wxSetCursor(wxCURSOR_BLANK)
#define SHOW_CURSOR wxSetCursor(*wxSTANDARD_CURSOR)
#elif defined(__WXMSW__)
#define HIDE_CURSOR ShowCursor(0)
#define SHOW_CURSOR ShowCursor(1)
#endif

BEGIN_EVENT_TABLE(ModelDrawControl, wxGLCanvas)
EVT_MOTION(ModelDrawControl::MouseMoved)
EVT_LEFT_DOWN(ModelDrawControl::MouseDown)
EVT_LEFT_UP(ModelDrawControl::MouseReleased)
EVT_RIGHT_DOWN(ModelDrawControl::RightClick)
EVT_LEAVE_WINDOW(ModelDrawControl::MouseLeftWindow)
EVT_SIZE(ModelDrawControl::Resized)
EVT_KEY_DOWN(ModelDrawControl::KeyPressed)
EVT_KEY_UP(ModelDrawControl::KeyReleased)
EVT_MOUSEWHEEL(ModelDrawControl::MouseWheelMoved)
EVT_PAINT(ModelDrawControl::Render)
END_EVENT_TABLE();

// some useful events to use
void ModelDrawControl::MouseMoved(wxMouseEvent& event)
{
    if (this->IsMouseDrag)
    {
        const auto halfWidth = this->GetWidth() / 2;
        const auto halfHeight = this->GetHeight() / 2;
        const auto mouse = event.GetPosition();
        this->MouseLookAt(mouse.x - halfWidth, mouse.y - halfHeight);
        this->WarpPointer(halfWidth, halfHeight);
        this->Refresh();
        event.Skip();
    }
}

void ModelDrawControl::MouseDown(wxMouseEvent& event)
{
    if (event.LeftIsDown() && !this->IsMouseDrag)
    {
        this->CaptureMouse();

        const auto halfWidth = this->GetWidth() / 2;
        const auto halfHeight = this->GetHeight() / 2;
        this->WarpPointer(halfWidth, halfHeight);
        HIDE_CURSOR;
        this->IsMouseDrag = true;
        this->Refresh();
        event.Skip();
    }
}

void ModelDrawControl::MouseReleased(wxMouseEvent& event)
{
    if (!event.LeftIsDown() && this->IsMouseDrag)
    {
        SHOW_CURSOR;
        this->ReleaseMouse();
        this->IsMouseDrag = false;
        this->Refresh();
        event.Skip();
    }
}

void ModelDrawControl::MouseWheelMoved(wxMouseEvent& event)
{
    if (event.GetWheelDelta() > 0)
    {
        this->modelCamera.Position += this->modelCamera.Direction * (event.GetWheelRotation() / (float)event.GetWheelDelta());
        this->Refresh();
        event.Skip();
    }
}

void ModelDrawControl::RightClick(wxMouseEvent& event) {}
void ModelDrawControl::MouseLeftWindow(wxMouseEvent& event) {}
void ModelDrawControl::KeyPressed(wxKeyEvent& event) {}
void ModelDrawControl::KeyReleased(wxKeyEvent& event) {}

ModelDrawControl::ModelDrawControl(wxFrame* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    this->glContext = new wxGLContext(this);
    this->modelCamera = Camera();

    // To avoid flashing on MSW
    this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

ModelDrawControl::~ModelDrawControl()
{
    if (this->model != nullptr) delete this->model;
    delete this->glContext;
}

void ModelDrawControl::Resized(wxSizeEvent& evt)
{
    this->Refresh();
}

void ModelDrawControl::LoadModel(NyaModel* model)
{
    if (this->model != nullptr)
    {
        delete this->model;
    }

    this->model = model;

    NyaVertex max(FLT_MIN, FLT_MIN, FLT_MIN);
    NyaVertex min(FLT_MAX, FLT_MAX, FLT_MAX);
    size_t pointCount = 0;

    for (size_t meshIdx = 0; meshIdx < this->model->meshCount; meshIdx++)
    {
        for (size_t pointIdx = 0; pointIdx < this->model->meshes[meshIdx]->pointCount; pointIdx++)
        {
            const auto point = this->model->meshes[meshIdx]->points[pointIdx];
            max = max.Max(point);
            min = min.Min(point);
            pointCount++;
        }
    }

    if (pointCount > 0)
    {
        const auto center = (max + min) / 2.0f;
        const auto radius = min.DistanceTo(max);

        this->modelCamera = Camera();
        this->modelCamera.Position -= this->modelCamera.Direction * radius;
    }
    else
    {
        this->modelCamera = Camera();
        this->modelCamera.Position.Z = -50.0f;
    }

    this->Refresh();
}

void ModelDrawControl::Prepare2DViewport(int x, int y, int width, int height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_LINE_WIDTH);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(x, y, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 0.0, width, height, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ModelDrawControl::Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio_w_h = (float)(bottomrigth_x - topleft_x) / (float)(bottomrigth_y - topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200.0 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void ModelDrawControl::MouseLookAt(int dx, int dy)
{
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat mouseX, mouseY;

    this->Prepare3DViewport(0, 0, this->GetWidth(), this->GetHeight());

    //Setup the camera (referencing angles from the openGL class)
    auto eye = this->modelCamera.Direction + this->modelCamera.Position;

    gluLookAt(
        0.0f,
        0.0f,
        0.0f,
        this->modelCamera.Direction.X,
        this->modelCamera.Direction.Y,
        this->modelCamera.Direction.Z,
        this->modelCamera.Up.X,
        this->modelCamera.Up.Y,
        this->modelCamera.Up.Z);

    //Get the matrices and viewport details
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Mouse cursor pos correction (opengl’s upside down)
    mouseX = (float)dx;
    mouseY = (float)dy;

    // Unproject
    GLdouble x, y, z;
    NyaVertex vert;

    gluUnProject(mouseX + (this->GetWidth() / 2), (this->GetHeight() / 2) - mouseY, -0.1f, modelview, projection, viewport, &x, &y, &z);
    vert.X = (float)x;
    vert.Y = (float)y;
    vert.Z = (float)z;
    vert.Normalize();

    // Make camera look at point
    auto side = this->modelCamera.Up.Cross(this->modelCamera.Direction).GetNormal();
    this->modelCamera.Direction = vert;
    this->modelCamera.Up = vert.Cross(side).GetNormal();
}

void ModelDrawControl::RenderCursor()
{
    if (this->IsMouseDrag)
    {
        const auto halfWidth = this->GetWidth() / 2.0f;
        const auto halfHeight = this->GetHeight() / 2.0f;
        glClear(GL_DEPTH_BUFFER_BIT);
        this->Prepare2DViewport(halfWidth - 16, halfHeight - 16, 32, 32);

        glBegin(GL_LINES);

        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);

        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);

        glEnd();
    }
}

void ModelDrawControl::RenderAxis()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    this->Prepare2DViewport(0, 0, 64, 64);

    const auto x = this->modelCamera.Direction.Cross(this->modelCamera.Up) * 0.8f;
    const auto y = this->modelCamera.Up * 0.8f;
    const auto z = this->modelCamera.Direction * 0.8f;

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(x.X, x.Y, x.Z);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(y.X, y.Y, y.Z);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(z.X, z.Y, z.Z);

    glEnd();
}

const int ModelDrawControl::GetWidth()
{
    return this->GetSize().x;
}

const int ModelDrawControl::GetHeight()
{
    return this->GetSize().y;
}

void ModelDrawControl::Render(wxPaintEvent& evt)
{
    if (!this->IsShown()) return;

    wxGLCanvas::SetCurrent(*this->glContext);
    wxPaintDC paint(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    // Draw background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->Prepare2DViewport(0, 0, this->GetWidth(), this->GetHeight());

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.9f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    // Draw model
    glClear(GL_DEPTH_BUFFER_BIT);
    this->Prepare3DViewport(0, 0, this->GetWidth(), this->GetHeight());

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto eye = this->modelCamera.Direction + this->modelCamera.Position;
    gluLookAt(
        this->modelCamera.Position.X,
        this->modelCamera.Position.Y,
        this->modelCamera.Position.Z,
        eye.X,
        eye.Y,
        eye.Z,
        this->modelCamera.Up.X,
        this->modelCamera.Up.Y,
        this->modelCamera.Up.Z);

    if (this->model != nullptr)
    {
        this->model->Render(nullptr);
    }

    // Render cursor
    this->RenderCursor();

    // Render axis
    this->RenderAxis();

    glFlush();
    SwapBuffers();
}
