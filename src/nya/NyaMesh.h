#pragma once
#include <wx/wfstream.h>
#include "NyaPolygon.h"
#include "NyaAttributes.h"

/** @brief Flat mesh geometry
 */
class NyaMesh : public NyaLoadable
{
public:
	NyaMesh();
	~NyaMesh();

	/** @brief Mesh point count
	 */
	size_t pointCount = 0;

	/** @brief Mesh polygon count
	 */
	size_t polygonCount = 0;

	/** @brief Mesh points
	 */
	NyaVertex* points = nullptr;

	/** @brief Mesh polygons
	 */
	NyaPolygon* polygons = nullptr;

	/** @brief Mesh polygon attributes
	 */
	NyaAttributes* attributes = nullptr;

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;

	/** @brief Render single quad
	 * @param polygonIdx Index of the polygon
	 * @param textures Quad textures
	 */
	void RenderQuad(size_t polygonIdx, GLuint* textures);

	/** Get depth of a quad
	 * @param polygonIdx Index of the polygon
	 */
	float GetQuadDepth(size_t polygonIdx);

protected:

	/** @brief Render single quad
	 * @param polygonIdx Index of the polygon
	 * @param textures Quad textures
	 * @param normals Vertex normal vectors
	 */
	void RenderQuad(size_t polygonIdx, GLuint* textures, NyaVertex normals[4]);
};