#include "NyaMesh.h"
#include "wx/glcanvas.h"
#include "../RenderOptions.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

/** @brief Mesh transparency pattern
 */
const GLubyte singlePixelStippleMask[] =
{
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
	0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
};

/** @brief Mesh transparency pattern
 */
const GLubyte doublePixelStippleMask[] =
{
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
};

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

float NyaMesh::GetQuadDepth(size_t polygonIdx)
{
	float min = FLT_MAX;
	float max = FLT_MIN;

	for (int point = 0; point < 4; point++)
	{
		NyaVertex vertex = this->points[this->polygons[polygonIdx].Points[point]];

		GLdouble model_view[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

		GLdouble projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		GLdouble tmp;
		GLdouble depth;
		gluProject(vertex.X, vertex.Y, vertex.Z,
			model_view, projection, viewport,
			&tmp, &tmp, &depth);


		min = std::min(min, (float)depth);
		max = std::max(max, (float)depth);
	}

	switch (this->attributes[polygonIdx].Flags.SortMode)
	{
	case 2:
		return min;

	case 1:
		return max;

	default:
		return (min + max) / 2.0f;
	}
}

void NyaMesh::RenderQuad(size_t polygon, GLuint* textures)
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

void NyaMesh::RenderQuad(size_t polygon, GLuint* textures, NyaVertex normals[4])
{
	static NyaVertex uv[4] =
	{
		NyaVertex(0.0f, 0.0f),
		NyaVertex(1.0f, 0.0f),
		NyaVertex(1.0f, 1.0f),
		NyaVertex(0.0f, 1.0f),
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

	if (flags.HasMeshEffect)
	{
		glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(doublePixelStippleMask);
	}

	NyaVertex flatNormal = this->polygons[polygon].Normal;

	glBegin(flags.IsWireframe ? GL_LINE_LOOP : GL_QUADS);

	for (size_t point = 0; point < 4; point++)
	{
		NyaVertex vertex = this->points[this->polygons[polygon].Points[point]];
		NyaColor baseColor = color * brightMultiplier;

		if (RenderOptions::Shaded)
		{
			// TODO: Add directional light math here!
		}

		glColor4f(baseColor.R, baseColor.G, baseColor.B, halfAlpha ? 0.5f : 1.0f);

		if (flags.HasFlatShading)
		{
			glNormal3f(flatNormal.X, flatNormal.Y, flatNormal.Z);
		}
		else
		{
			glNormal3f(normals[point].X, normals[point].Y, normals[point].Z);
		}

		glTexCoord2f(uv[point].X, uv[point].Y);
		glVertex3f(vertex.X, vertex.Y, vertex.Z);
	}

	glEnd();

	if (flags.HasMeshEffect)
	{
		glDisable(GL_POLYGON_STIPPLE);
	}

	if (flags.HasTexture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (RenderOptions::ShowWires)
	{
		NyaColor complementaryColor = color.GetComplementary();

		glBegin(GL_LINE_LOOP);

		for (size_t point = 0; point < 4; point++)
		{
			NyaVertex vertex = this->points[this->polygons[polygon].Points[point]];

			glColor4f(complementaryColor.R, complementaryColor.G, complementaryColor.B, 1.0f);
			glVertex3f(vertex.X, vertex.Y, vertex.Z);
		}

		glEnd();
	}
}