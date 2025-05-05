#include "NyaMesh.h"
#include "wx/glcanvas.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

NyaMesh::NyaMesh()
{

}

NyaMesh::~NyaMesh()
{
	if (this->points != nullptr) delete[] this->points;
	if (this->polygons != nullptr) delete[] this->polygons;
	if (this->attributes != nullptr) delete[] this->attributes;

	this->points = nullptr;
	this->polygons = nullptr;
	this->attributes = nullptr;
}

bool NyaMesh::Open(wxFileInputStream& stream)
{
	bool result = true;

	result &= NyaUtils::GetAndIterate(stream, &this->pointCount, 1);
	result &= NyaUtils::GetAndIterate(stream, &this->polygonCount, 1);

	if (result)
	{
		this->points = new NyaVertex[this->pointCount];
		result &= NyaUtils::OpenAndIterate(stream, this->points, this->pointCount);
	}

	if (result)
	{
		this->polygons = new NyaPolygon[this->polygonCount];
		result &= NyaUtils::OpenAndIterate(stream, this->polygons, this->polygonCount);
	}

	if (result)
	{
		this->attributes = new NyaAttributes[this->polygonCount];
		result &= NyaUtils::OpenAndIterate(stream, this->attributes, this->polygonCount);
	}

	return result;
}

void NyaMesh::Render(GLuint* textures = nullptr)
{
	for (size_t polygon = 0; polygon < this->polygonCount; polygon++)
	{
		NyaVertex normals[4] =
		{
			this->polygons[polygon].Normal,
			this->polygons[polygon].Normal,
			this->polygons[polygon].Normal,
			this->polygons[polygon].Normal
		};

		this->RenderQuad(polygon, textures, normals);
	}
}

/** @brief Render single quad
 */
void NyaMesh::RenderQuad(size_t polygon, GLuint* textures, NyaVertex normals[4])
{
	static NyaVertex uv[4] =
	{
		NyaVertex(0.0f, 1.0f),
		NyaVertex(1.0f, 1.0f),
		NyaVertex(1.0f, 0.0f),
		NyaVertex(0.0f, 0.0f),
	};

	const auto flags = this->attributes[polygon].Flags;
	NyaColor color = this->attributes[polygon].BaseColor;
	bool halfAlpha = flags.HasMeshEffect | flags.HasTransparency;
	float brightMultiplier = flags.HasHalfBrightness ? 0.5f : 1.0f;

	if (flags.HasTexture)
	{
		color = NyaColor();
		glBindTexture(GL_TEXTURE_2D, textures[this->attributes[polygon].Texture]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBegin(flags.IsWireframe ? GL_LINE_LOOP : GL_QUADS);

	for (size_t point = 0; point < 4; point++)
	{
		NyaVertex vertex = this->points[this->polygons[polygon].Points[point]];

		glColor4f(color.R * brightMultiplier, color.G * brightMultiplier, color.B * brightMultiplier, halfAlpha ? 0.5f : 1.0f);
		glNormal3f(normals[point].X, normals[point].Y, normals[point].Z);
		glTexCoord2f(uv[point].X, uv[point].Y);
		glVertex3f(vertex.X, vertex.Y, vertex.Z);
	}

	glEnd();
}