#pragma once
#include <wx/wx.h>

/** Rendering options
 */
class RenderOptions final
{
private:
	RenderOptions() = delete;
	~RenderOptions() = delete;

public:
	/** @brief Show wireframe on top
	 */
	inline static bool ShowWires = false;

	/** @brief Indicates whether model is shaded
	 */
	inline static bool Shaded = true;
};