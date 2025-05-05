#include "NyaAttributes.h"

bool NyaAttributes::Open(wxFileInputStream& stream)
{
    bool result = true;

    NyaAttributeFlags flags;
    result &= NyaUtils::GetAndIterate(stream, (uint8_t*)&flags, 2);
    this->Flags = flags;

    uint16_t rgb555;
    result &= NyaUtils::GetAndIterate(stream, &rgb555, 1);
    this->BaseColor.R = (rgb555 & 0x1F) / 31.0f;
    this->BaseColor.G = ((rgb555 >> 5) & 0x1F) / 31.0f;
    this->BaseColor.B = ((rgb555 >> 10) & 0x1F) / 31.0f;

    result &= NyaUtils::GetAndIterate(stream, &this->Texture, 1);

    return result;
}
