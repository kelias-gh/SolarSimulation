#include "AOMMath.h"

namespace Math {
		/*
		
		Vector4 operators
		
		*/

		inline Vector4 Vector4::operator=(Vector4 const& Vector) {
			x = Vector.x;
			y = Vector.y;
			z = Vector.z;
			w = Vector.w;

			return Vector;
		}

		inline Vector4 Vector4::operator*(Vector4 const& Vector) {
			Vector4 v;
			v.x = x * Vector.x;
			v.y = y * Vector.y;
			v.z = z * Vector.z;
			v.w = w * Vector.w;

			return v;
		}
		
		inline Vector4 Vector4::operator+(Vector4 const& Vector) {
			Vector4 v;
			v.x = x + Vector.x;
			v.y = y + Vector.y;
			v.z = z + Vector.z;
			v.w = w + Vector.w;

			return v;
		}
		
		inline Vector4 Vector4::operator-(Vector4 const& Vector) {
			Vector4 v;
			v.x = x - Vector.x;
			v.y = y - Vector.y;
			v.z = z - Vector.z;
			v.w = w - Vector.w;

			return v;
		}

		/*
		
		Vector3 operators
		
		*/

		inline Vector3 Vector3::operator=(Vector3 const& Vector) {
			x = Vector.x;
			y = Vector.y;
			z = Vector.z;

			return Vector;
		}

		inline Vector3 Vector3::operator*(Vector3 const& Vector) {
			Vector3 v;
			v.x = x * Vector.x;
			v.y = y * Vector.y;
			v.z = z * Vector.z;

			return v;
		} 

		inline Vector3 Vector3::operator*(float const& Value) {
			x *= Value;
			y *= Value;
			z *= Value;
			
			return Vector3{ x, y, z };
		}

		inline Vector3 Vector3::operator/(float const& Value)
		{
			return Vector3(x / Value, y / Value, z / Value);
		}
		
		inline Vector3 Vector3::operator+(Vector3 const& Vector) {
			Vector3 v;
			v.x = x + Vector.x;
			v.y = y + Vector.y;
			v.z = z + Vector.z;

			return v;
		}

		inline Vector3 Vector3::operator+=(Vector3 const& Vector)
		{
			x += Vector.x;
			y += Vector.y;
			z += Vector.z;

			return *this;
		}
		
		inline Vector3 Vector3::operator-(Vector3 const& Vector) {
			Vector3 v;
			v.x = x - Vector.x;
			v.y = y - Vector.y;
			v.z = z - Vector.z;

			return v;
		}

		inline Vector3 Vector3::operator-() const {
			return Vector3(-x, -y, -z);
		}

		inline Vector3 Vector3::operator-=(Vector3 const& Vector)
		{
			x -= Vector.x;
			y -= Vector.y;
			z -= Vector.z;
			
			return *this;
		}

		inline bool Vector3::operator>(Vector3 const& Vector)
		{
			if (x > Vector.x &&
				y > Vector.y &&
				z > Vector.z) {
				return true;
			}
			return false;
		}

		inline bool Vector3::operator<(Vector3 const& Vector)
		{

			if (x < Vector.x &&
				y < Vector.y &&
				z < Vector.z) {
				return true;
			}
			return false;
		}

		const float Vector3::magnitude() {
			return sqrt(x * x + y * y + z * z);
		}

		Vector3 normalize(Vector3 vector) {
			if (vector.magnitude() == 0)
			{
				return Vector3{ 0,0,0 };
			}
			Vector3 vec;

			float magnitude = vector.magnitude();

			vec.x = vector.x / magnitude;
			vec.y = vector.y / magnitude;
			vec.z = vector.z / magnitude;

			return vec;
		}

		/*
		
		Vector2 operators
		
		*/
		inline Vector2 Vector2::operator=(Vector2 const& Vector) {
			x = Vector.x;
			y = Vector.y;
			return Vector;
		}

		inline Vector2 Vector2::operator*(Vector2 const& Vector) {
			Vector2 v;
			v.x = x * Vector.x;
			v.y = y * Vector.y;
			return v;
		}
		
		inline Vector2 Vector2::operator+(Vector2 const& Vector) {
			Vector2 v;
			v.x = x + Vector.x;
			v.y = y + Vector.y;
			return v;
		}
		
		inline Vector2 Vector2::operator-(Vector2 const& Vector) {
			Vector2 v;
			v.x = x - Vector.x;
			v.y = y - Vector.y;

			return v;
		}

		inline bool Vector2::operator>(Vector2 const& Vector)
		{
			if (x > Vector.x &&
				y > Vector.y) {
				return true;
			}
			return false;
		}

		inline bool Vector2::operator<(Vector2 const& Vector)
		{
			if (x < Vector.x &&
				y < Vector.y) {
				return true;
			}
			return false;
		}

		inline Vector3 CrossProduct(Vector3 a, Vector3 b) {
			Vector3 crossProduct;
			crossProduct.x = (a.y * b.z) - (a.z * b.y);
			crossProduct.y = (a.z * b.x) - (a.x * b.z);
			crossProduct.z = (a.x * b.y) - (a.y * b.x);
			return crossProduct;
		}			
		
		inline float DotProduct(Vector4 a, Vector4 b) {
			float dotProduct = 0;
			dotProduct = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
			return dotProduct;
		}

		inline float DotProduct(Vector3 a, Vector3 b) {
			float dotProduct = 0;
			dotProduct = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
			return dotProduct;
		}

		inline float DotProduct(Vector2 a, Vector2 b) {
			float dotProduct = 0;
			dotProduct = (a.x * b.x) + (a.y * b.y);
			return dotProduct;
		}

		inline Vector3 absVector(Vector3 vector) {
			vector.x = std::abs(vector.x);
			vector.y = std::abs(vector.y);
			vector.z = std::abs(vector.z);
			return vector;
		}
}
