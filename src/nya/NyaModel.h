#pragma once

#include "NyaSmoothMesh.h"

/** @brief Model group
 */
class NyaModel : public NyaLoadable, NyaRenderable
{
public:

	/** @brief Type of a mesh stored within model
	 */
	enum class MeshType : size_t
	{
		/** Flat shaded mesh
		 */
		Flat = 0,

		/** Smooth shaded mesh
		 */
		Smooth = 1
	};

	/** @brief Number of meshes in group
	 */
	size_t meshCount = 0;

	/** @brief Number of textures in group
	 */
	size_t textureCount = 0;

	/** @brief array of loaded textures
	 */
	GLuint* textures = nullptr;

	/** @brief Mesh geometry
	 */
	NyaMesh** meshes = nullptr;

	/** @brief Type of stored mesh geometry
	 */
	MeshType meshType = MeshType::Flat;

	NyaModel();
	~NyaModel();

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;

	/** @brief Render structure
	 */
	void Render(GLuint* tex) override;

private:

	/** @brief Delete mesh geometry
	 * @param target Target buffer
	 * @param count Number of stored geometry
	 */
	template<typename T>
	static bool DeleteMeshes(T** target, size_t count);

	/** @brief Render mesh geometry
	 * @param target Target buffer
	 * @param count Number of stored geometry
	 */
	template<typename T>
	static void RenderMeshes(T** target, GLuint* textures, size_t count);
};
