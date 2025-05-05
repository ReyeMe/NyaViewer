#pragma once
#include "NyaVertex.h"

/** @brief Polygon geometry
 */
class NyaPolygon : public NyaLoadable
{
public:
	/** @brief Polygon normal
	 */
	NyaVertex Normal;

	/** @brief Polygon points
	 */
	uint16_t Points[4];

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;
};