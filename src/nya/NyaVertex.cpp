#include "NyaVertex.h"

bool NyaVertex::Open(wxFileInputStream& stream)
{
    int32_t fxp[3];
    if (NyaUtils::GetAndIterate(stream, fxp, 3))
    {
        this->X = fxp[0] / 65536.0f;
        this->Y = fxp[1] / 65536.0f;
        this->Z = fxp[2] / 65536.0f;
        return true;
    }

    return false;
}
