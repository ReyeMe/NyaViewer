#include "NyaSmoothMesh.h"
#include "wx/glcanvas.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

NyaSmoothMesh::NyaSmoothMesh()
{
}

NyaSmoothMesh::~NyaSmoothMesh()
{
	if (this->normals != nullptr) delete[] this->normals;

	this->normals = nullptr;
}

bool NyaSmoothMesh::Open(wxFileInputStream& stream)
{
	// Load flat mesh stuff
	if (NyaMesh::Open(stream))
	{
		this->normals = new NyaVertex[this->pointCount];
		return NyaUtils::OpenAndIterate(stream, this->normals, this->pointCount);
	}

	return false;
}

void NyaSmoothMesh::RenderQuad(size_t polygon, GLuint* textures)
{
	NyaVertex normals[4] =
	{
			this->normals[this->polygons[polygon].Points[0]],
			this->normals[this->polygons[polygon].Points[1]],
			this->normals[this->polygons[polygon].Points[2]],
			this->normals[this->polygons[polygon].Points[3]]
	};

	NyaMesh::RenderQuad(polygon, textures, normals);
}
