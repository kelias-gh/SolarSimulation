#pragma once
#include <cmath>
#include <iostream>

namespace Math {
#define PI 3.14159265359
		struct Vector4 {
			union {
				struct {
					float x, y, z, w;
				};
				float v[4];
			};
			Vector4() : x(0), y(0), z(0), w(0) {}
			Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

			inline Vector4 operator = (Vector4 const& Vector);
			inline Vector4 operator * (Vector4 const& Vector);
			inline Vector4 operator + (Vector4 const& Vector);
			inline Vector4 operator - (Vector4 const& Vector);
		};

		struct Vector3 {
			union {
				struct {
					float x, y, z;
				};
				float v[3];
			};

			Vector3() : x(0), y(0), z(0) {}
			Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

			const inline float magnitude();
			const inline float sqrMagnitude();

			inline Vector3 operator = (Vector3 const& Vector);
			inline Vector3 operator * (Vector3 const& Vector);
			inline Vector3 operator * (float const& Value);
			inline Vector3 operator / (float const& Value);
			inline Vector3 operator + (Vector3 const& Vector);
			inline Vector3 operator += (Vector3 const& Vector);
			inline Vector3 operator - (Vector3 const& Vector);
			inline Vector3 operator-() const;
			inline Vector3 operator -= (Vector3 const& Vector);
			inline bool operator > (Vector3 const& Vector);
			inline bool operator < (Vector3 const& Vector);
		};

		struct Vector2 {
			union {
				struct {
					float x, y;
				};
				float v[2];
			};

			Vector2() : x(0), y(0) {}
			Vector2(float x, float y) : x(x), y(y) {}

			inline Vector2 operator = (Vector2 const& Vector);
			inline Vector2 operator * (Vector2 const& Vector);
			inline Vector2 operator + (Vector2 const& Vector);
			inline Vector2 operator - (Vector2 const& Vector);
			inline bool operator > (Vector2 const& Vector);
			inline bool operator < (Vector2 const& Vector);
		};

		inline Vector3 CrossProduct(Vector3 a, Vector3 b);
		inline Vector3 normalize(Vector3 vector);
		inline float DotProduct(Vector4 a, Vector4 b);
		inline float DotProduct(Vector3 a, Vector3 b);
		inline float DotProduct(Vector2 a, Vector2 b);

		inline Vector3 absVector(Vector3 vector);

		const Vector3 forward = Vector3(0, 0, 1);
		const Vector3 up = Vector3(0, 1, 0);
		const Vector3 right = Vector3(1, 0, 0);

		struct Matrix4x4 {
			Vector4 v[4];

			inline Matrix4x4 operator = (Matrix4x4 const& matrix);
			inline Matrix4x4 operator * (Matrix4x4 const& matrix);
			friend std::ostream& operator<<(std::ostream& out, const Matrix4x4& matrix)
			{
				for (size_t i = 0; i < _countof(matrix.v); i++)
				{
					out << matrix.v[i].x << ", " << matrix.v[i].y << ", " << matrix.v[i].z << ", " << matrix.v[i].w << std::endl;
				}
				out << std::endl;
				return out;
			}
		};

		inline Matrix4x4 Transpose(Matrix4x4 Matrix);

		inline Matrix4x4 Translation(float X, float Y, float Z);
		inline Matrix4x4 Translation(Vector3 translationVector);
		inline Matrix4x4 TranslationX(float X);
		inline Matrix4x4 TranslationY(float Y);
		inline Matrix4x4 TranslationZ(float Z);

		inline Matrix4x4 RotationRollPitchYaw(float Roll, float Pitch, float Yaw);
		inline Matrix4x4 RotationRollPitchYaw(Vector3 RollPitchYawVector);
		inline Matrix4x4 RotationRoll(float degrees);
		inline Matrix4x4 RotationPitch(float degrees);
		inline Matrix4x4 RotationYaw(float degrees);

		inline Matrix4x4 Scaling(float X, float Y, float Z);
		inline Matrix4x4 Scaling(Vector3 scalingVector);
		inline Matrix4x4 ScalingX(float X);
		inline Matrix4x4 ScalingY(float Y);
		inline Matrix4x4 ScalingZ(float Z);

		inline Matrix4x4 PerspectiveProjection(float FOV, float WindowWidth, float WindowHeight, float ZClipNear, float ZClipFar);
		inline Matrix4x4 OrthogonalProjection();

		inline Matrix4x4 LookAtView(Vector3 Position, Vector3 LookAt, Vector3 UpDir);

	inline float radians(float degrees) {
		return (degrees * PI) / 180;
	}
}

#include "AOMMathMatrices.inl"
#include "AOMMathVectors.inl"