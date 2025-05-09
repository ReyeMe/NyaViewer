#pragma once
#include "NyaMesh.h"

/** @brief Smooth mesh geometry
 */
class NyaSmoothMesh : public NyaMesh
{
public:
	/** @brief Point normal vectors
	 */
	NyaVertex* normals = nullptr;

	NyaSmoothMesh();
	~NyaSmoothMesh();

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;

	/** @brief Render single quad
	 * @param polygonIdx Index of the polygon
	 * @param textures Quad textures
	 */
	void RenderQuad(size_t polygonIdx, GLuint* textures);
};
