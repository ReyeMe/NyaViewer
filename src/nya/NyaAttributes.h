#pragma once
#include "NyaUtils.h"

struct NyaColor
{
    NyaColor() : R(1.0f), G(1.0f), B(1.0f) {}
    NyaColor(float r, float g, float b) : R(r), G(g), B(b) {}

    /** @brief Red component
     */
    float R;

    /** @brief Green component
     */
    float G;

    /** @brief Blue component
     */
    float B;
};

#pragma pack(push, 1)
struct NyaAttributeFlags
{
    /** @brief Sort mode for face (0 = center)
     */
    uint8_t SortMode : 2;

    /** @brief Render face using half the brightness
     */
    uint8_t HasHalfBrightness : 1;

    /** @brief Face does not use gouraud shading
     */
    uint8_t HasFlatShading : 1;

    /** @brief Half transparency effect
     */
    uint8_t HasTransparency : 1;

    /** @brief Indicates whether this polygon has a mesh effect applied to it
     */
    uint8_t IsDoubleSided : 1;

    /** @brief Indicates whether this polygon has a mesh effect applied to it
     */
    uint8_t HasMeshEffect : 1;

    /** @brief Indicates whether a texture is applied to this polygon
     */
    uint8_t HasTexture : 1;

    /** @brief Reserved for future use
     */
    uint8_t Reserved : 7;

    /** @brief Render faces as wireframe
     */
    uint8_t IsWireframe : 1;
};
#pragma pack(pop)

/** @brief Polygon attributes
 */
class NyaAttributes : public NyaLoadable
{
public:
    /** @brief Polygon attribute flags
     */
    NyaAttributeFlags Flags;

    /** @brief This field is set if HasTexture field is false
     */
    NyaColor BaseColor;

    /** @brief Index of a texture to use if HasTexture field is true
     */
    size_t Texture;

    /** @brief Read structure from stream
     * @param stream Data stream
     */
    bool Open(wxFileInputStream& stream) override;
};