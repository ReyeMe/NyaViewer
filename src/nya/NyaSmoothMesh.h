#pragma once
#include "NyaMesh.h"

/** @brief Smooth mesh geometry
 */
class NyaSmoothMesh : public NyaMesh
{
public:
	NyaSmoothMesh();
	~NyaSmoothMesh();

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;

	/** @brief Render structure
	 */
	void Render(GLuint* textures) override;

private:

	/** @brief Point normal vectors
	 */
	NyaVertex* normals = nullptr;
};
