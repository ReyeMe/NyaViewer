#include "NyaPolygon.h"

bool NyaPolygon::Open(wxFileInputStream& stream)
{
	bool result = true;

	result &= NyaUtils::OpenAndIterate(stream, &this->Normal, 1);
	result &= NyaUtils::GetAndIterate(stream, this->Points, _countof(this->Points));

	return result;
}
