#include "NyaModel.h"
#include "NyaUtils.h"

NyaModel::NyaModel()
{
}

NyaModel::~NyaModel()
{
	if (this->meshType == MeshType::Smooth)
	{
		NyaModel::DeleteMeshes((NyaSmoothMesh**)this->meshes, this->meshCount);
	}
	else
	{
		NyaModel::DeleteMeshes(this->meshes, this->meshCount);
	}

	if (this->textures != nullptr)
	{
		glDeleteTextures(this->textureCount, this->textures);
		delete[] this->textures;
	}
}

bool NyaModel::Open(wxFileInputStream& stream)
{
	bool result = true;

	// Load header
	result &= NyaUtils::GetAndIterate(stream, &this->meshType, 1);
	result &= NyaUtils::GetAndIterate(stream, &this->meshCount, 1);
	result &= NyaUtils::GetAndIterate(stream, &this->textureCount, 1);

	// Start loading objects
	if (result)
	{
		if (this->meshType == MeshType::Smooth)
		{
			this->meshes = (NyaMesh**)new NyaSmoothMesh * [this->meshCount];
			result &= NyaUtils::OpenAndIterate(stream, (NyaSmoothMesh**)this->meshes, this->meshCount);
		}
		else
		{
			this->meshes = new NyaMesh * [this->meshCount];
			result &= NyaUtils::OpenAndIterate(stream, (NyaMesh**)this->meshes, this->meshCount);
		}

		// Load textures
		if (result && this->textureCount > 0)
		{
			this->textures = new GLuint[this->textureCount];

			for (size_t textureIdx = 0; result && textureIdx < this->textureCount; textureIdx++)
			{
				uint16_t width;
				uint16_t height;
				result &= NyaUtils::GetAndIterate(stream, &width, 1);
				result &= NyaUtils::GetAndIterate(stream, &height, 1);

				uint8_t* rgbaData = new uint8_t[width * height * 4];

				if (rgbaData != nullptr)
				{
					for (size_t pixel = 0; pixel < (size_t)(width * height); pixel++)
					{
						uint16_t colorData;
						NyaUtils::GetAndIterate(stream, &colorData, 1);

						rgbaData[pixel * 4] = (colorData & 0x1F) << 3;
						rgbaData[(pixel * 4) + 1] = ((colorData >> 5) & 0x1F) << 3;
						rgbaData[(pixel * 4) + 2] = ((colorData >> 10) & 0x1F) << 3;
						rgbaData[(pixel * 4) + 3] = (colorData & 0x8000) > 0 ? 0xff : 0;
					}

					// Create Textures
					glEnable(GL_TEXTURE_2D);
					GLuint texture;
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
					this->textures[textureIdx] = texture;
					glBindTexture(GL_TEXTURE_2D, 0);

					delete[] rgbaData;
				}
				else
				{
					return false;
				}
			}
		}
	}

	return result;
}

void NyaModel::Render(GLuint* tex = nullptr)
{
	if (this->meshType == MeshType::Flat)
	{
		NyaModel::RenderMeshes(this->meshes, this->textures, this->meshCount);
	}
	else
	{
		NyaModel::RenderMeshes((NyaSmoothMesh**)this->meshes, this->textures, this->meshCount);
	}
}

template<typename T>
static bool NyaModel::DeleteMeshes(T** target, size_t count)
{
	if (target != nullptr)
	{
		for (size_t mesh = 0; mesh < count; mesh++)
		{
			delete target[mesh];
		}

		delete target;
		return true;
	}

	return false;
}

template<typename T>
static void NyaModel::RenderMeshes(T** target, GLuint* textures, size_t count)
{
	static_assert(std::is_base_of<NyaRenderable, T>::value, "Type not supported");

	if (target != nullptr)
	{
		for (size_t mesh = 0; mesh < count; mesh++)
		{
			if (target[mesh] != nullptr)
			{
				target[mesh]->Render(textures);
			}
		}
	}
}