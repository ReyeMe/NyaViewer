#pragma once
#include "NyaUtils.h"

class NyaVertex : public NyaLoadable
{
public:

	NyaVertex() : X(0.0f), Y(0.0f), Z(0.0f) {}
	NyaVertex(float x, float y, float z = 0.0f) : X(x), Y(y), Z(z) {}

	/** @brief X coordinate
	 */
	float X;

	/** @brief Y coordinate
	 */
	float Y;

	/** @brief Z coordinate
	 */
	float Z;

	/** @brief Read structure from stream
	 * @param stream Data stream
	 */
	bool Open(wxFileInputStream& stream) override;

	/** @brief Indexing operator
	 * @param index Component index
	 */
	float operator[](int index)
	{
		switch (index)
		{
		case 0:
			return this->X;

		case 1:
			return this->Y;

		case 2:
			return this->Z;

		default:
			return 1.0f;
			break;
		}

		return this->X;
	}

	/** @brief Get vector length
	 */
	float Length()
	{
		return std::sqrtf(std::powf(this->X, 2.0f) + std::powf(this->Y, 2.0f) + std::powf(this->Z, 2.0f));
	}

	/** @brief get vector cross product
	 * @param other Other vector
	 */
	NyaVertex Cross(const NyaVertex& other)
	{
		return NyaVertex(
			(this->Y * other.Z) - (this->Z * other.Y),
			(this->Z * other.X) - (this->X * other.Z),
			(this->X * other.Y) - (this->Y * other.X));
	}

	/** @brief get vector cross product
	 * @param other Other vector
	 */
	float Dot(NyaVertex& other)
	{
		float accumulator = 0.0f;

		for (int component = 0; component < 3; component++)
		{
			accumulator += (*this)[component] * other[component];
		}

		return accumulator;
	}

	/** @brief Normalize current vector
	 */
	void Normalize()
	{
		float length = this->Length();

		if (length > 0.0f)
		{
			this->X /= length;
			this->Y /= length;
			this->Z /= length;
		}
	}

	/** @brief Get normalized vector
	 */
	NyaVertex GetNormal()
	{
		float length = this->Length();

		if (length > 0.0f)
		{
			return (*this) / length;
		}
		else
		{
			return *this;
		}
	}

	/** @brief Distance to other point
	 * @param other Point distance is calculated to
	 */
	float DistanceTo(const NyaVertex& other)
	{
		return std::sqrtf(std::powf(other.X - this->X, 2.0f) + std::powf(other.Y - this->Y, 2.0f) + std::powf(other.Z - this->Z, 2.0f));
	}

	/** @brief Get maximum of two points
	 * @param other Point to compare with
	 */
	NyaVertex Max(const NyaVertex& other)
	{
		return NyaVertex(std::max(this->X, other.X), std::max(this->Y, other.Y), std::max(this->Z, other.Z));
	}

	/** @brief Get minimum of two points
	 * @param other Point to compare with
	 */
	NyaVertex Min(const NyaVertex& other)
	{
		return NyaVertex(std::min(this->X, other.X), std::min(this->Y, other.Y), std::min(this->Z, other.Z));
	}

	/** @brief Addition operator
	 * @param other Other vector
	 */
	NyaVertex operator +(const NyaVertex& other)
	{
		return NyaVertex(this->X + other.X, this->Y + other.Y, this->Z + other.Z);
	}

	/** @brief Addition operator
	 * @param scaler Constant number
	 */
	NyaVertex operator +(const float scaler)
	{
		return NyaVertex(this->X + scaler, this->Y + scaler, this->Z + scaler);
	}

	/** @brief Substraction operator
	 * @param other Other vector
	 */
	NyaVertex operator -(const NyaVertex& other)
	{
		return NyaVertex(this->X - other.X, this->Y - other.Y, this->Z - other.Z);
	}

	/** @brief Substraction operator
	 * @param scaler Constant number
	 */
	NyaVertex operator -(const float scaler)
	{
		return NyaVertex(this->X - scaler, this->Y - scaler, this->Z - scaler);
	}

	/** @brief Multiplication operator
	 * @param other Other vector
	 */
	NyaVertex operator *(const NyaVertex& other)
	{
		return NyaVertex(this->X * other.X, this->Y * other.Y, this->Z * other.Z);
	}

	/** @brief Multiplication operator
	 * @param scaler Constant number
	 */
	NyaVertex operator *(const float scaler)
	{
		return NyaVertex(this->X * scaler, this->Y * scaler, this->Z * scaler);
	}

	/** @brief Division operator
	 * @param other Other vector
	 */
	NyaVertex operator /(const NyaVertex& other)
	{
		return NyaVertex(this->X / other.X, this->Y / other.Y, this->Z / other.Z);
	}

	/** @brief Division operator
	 * @param scaler Constant number
	 */
	NyaVertex operator /(const float scaler)
	{
		return NyaVertex(this->X / scaler, this->Y / scaler, this->Z / scaler);
	}

	/** @brief Addition assign operator
	 * @param other Other vector
	 */
	NyaVertex& operator +=(const NyaVertex& other)
	{
		this->X += other.X;
		this->Y += other.Y;
		this->Z += other.Z;
		return *this;
	}

	/** @brief Addition assign operator
	 * @param scaler Constant number
	 */
	NyaVertex& operator +=(const float scaler)
	{
		this->X += scaler;
		this->Y += scaler;
		this->Z += scaler;
		return *this;
	}

	/** @brief Substraction assign operator
	 * @param other Other vector
	 */
	NyaVertex& operator -=(const NyaVertex& other)
	{
		this->X -= other.X;
		this->Y -= other.Y;
		this->Z -= other.Z;
		return *this;
	}

	/** @brief Substraction assign operator
	 * @param scaler Constant number
	 */
	NyaVertex& operator -=(const float scaler)
	{
		this->X -= scaler;
		this->Y -= scaler;
		this->Z -= scaler;
		return *this;
	}

	/** @brief Multiplication assing operator
	 * @param other Other vector
	 */
	NyaVertex& operator *=(const NyaVertex& other)
	{
		this->X *= other.X;
		this->Y *= other.Y;
		this->Z *= other.Z;
		return *this;
	}

	/** @brief Multiplication assing operator
	 * @param scaler Constant number
	 */
	NyaVertex& operator *=(const float scaler)
	{
		this->X *= scaler;
		this->Y *= scaler;
		this->Z *= scaler;
		return *this;
	}

	/** @brief Division assign operator
	 * @param other Other vector
	 */
	NyaVertex& operator /=(const NyaVertex& other)
	{
		this->X /= other.X;
		this->Y /= other.Y;
		this->Z /= other.Z;
		return *this;
	}

	/** @brief Division assign operator
	 * @param scaler Constant number
	 */
	NyaVertex& operator /=(const float scaler)
	{
		this->X /= scaler;
		this->Y /= scaler;
		this->Z /= scaler;
		return *this;
	}
};