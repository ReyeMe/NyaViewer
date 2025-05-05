#pragma once
#include <wx/wx.h>
#include <wx/wfstream.h>

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

/** @brief Renderable structure base
 */
class NyaRenderable
{
public:

    /** @brief Render structure
     * @param textures Texture array
     */
    virtual void Render(GLuint * textures = nullptr) = 0;
};

/** @brief Loadable structure base
 */
class NyaLoadable
{
public:

    /** @brief Read structure from stream
     * @param stream Data stream
     */
    virtual bool Open(wxFileInputStream& stream) = 0;
};

/** @param Base utilities
 */
class NyaUtils
{
	NyaUtils() = delete;
	~NyaUtils() = delete;
public:

    /** @brief Get type from stream and advance access pointer
     * @param stream Data stream
     * @param target Target data buffer
     * @param count Number of data type to read
     */
	template<typename T>
	static bool GetAndIterate(wxFileInputStream& stream, T* target, size_t count)
    {
        if constexpr (
            !std::is_base_of<int8_t, T>::value &&
            !std::is_base_of<uint8_t, T>::value &&
            !std::is_base_of<int16_t, T>::value &&
            !std::is_base_of<uint16_t, T>::value &&
            !std::is_base_of<int32_t, T>::value &&
            !std::is_base_of<uint32_t, T>::value &&
            !std::is_base_of<int64_t, T>::value &&
            !std::is_base_of<uint64_t, T>::value)
        {
            static_assert("Type not supported");
        }

        for (size_t item = 0; item < count; item++)
        {
            for (int32_t byte = sizeof(T) - 1; byte >= 0; byte--)
            {
                if (!stream.CanRead())
                {
                    return false;
                }

                stream.Read(((uint8_t*)target) + (size_t)byte + (sizeof(T) * item), 1);
            }
        }

        return true;
    }

    /** @brief Open structure from stream and advance access pointer
     * @param stream Data stream
     * @param target Target data buffer
     * @param count Number of data type to read
     */
    template<typename T>
    static bool OpenAndIterate(wxFileInputStream& stream, T* target, size_t count)
    {
        static_assert(std::is_base_of<NyaLoadable, T>::value, "Type not supported");

        for (size_t item = 0; item < count; item++)
        {
            if (!target[item].Open(stream))
            {
                return false;
            }
        }

        return true;
    }

    /** @brief Open structure reference from stream and advance access pointer
     * @param stream Data stream
     * @param target Target data buffer
     * @param count Number of data type to read
     */
    template<typename T>
    static bool OpenAndIterate(wxFileInputStream& stream, T** target, size_t count)
    {
        static_assert(std::is_base_of<NyaLoadable, T>::value, "Type not supported");

        for (size_t item = 0; item < count; item++)
        {
            target[item] = new T();

            if (!NyaUtils::OpenAndIterate(stream, target[item], 1))
            {
                return false;
            }
        }

        return true;
    }
};