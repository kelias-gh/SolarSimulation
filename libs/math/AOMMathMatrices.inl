#include "AOMMath.h"

namespace Math {
		inline Matrix4x4 Matrix4x4::operator=(Matrix4x4 const& matrix) {

			v[0] = matrix.v[0];
			v[1] = matrix.v[1];
			v[2] = matrix.v[2];
			v[3] = matrix.v[3];
			return matrix;
		}

		inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& matrix) {
			Matrix4x4 m;
			float x = v[0].x;
			float y = v[0].y;
			float z = v[0].z;
			float w = v[0].w;
			// Perform the operation on the first row
			m.v[0].x = (matrix.v[0].x * x) + (matrix.v[1].x * y) + (matrix.v[2].x * z) + (matrix.v[3].x * w);
			m.v[0].y = (matrix.v[0].y * x) + (matrix.v[1].y * y) + (matrix.v[2].y * z) + (matrix.v[3].y * w);
			m.v[0].z = (matrix.v[0].z * x) + (matrix.v[1].z * y) + (matrix.v[2].z * z) + (matrix.v[3].z * w);
			m.v[0].w = (matrix.v[0].w * x) + (matrix.v[1].w * y) + (matrix.v[2].w * z) + (matrix.v[3].w * w);

			// Repeat for all the other rows
			x = v[1].x;
			y = v[1].y;
			z = v[1].z;
			w = v[1].w;
			m.v[1].x = (matrix.v[0].x * x) + (matrix.v[1].x * y) + (matrix.v[2].x * z) + (matrix.v[3].x * w);
			m.v[1].y = (matrix.v[0].y * x) + (matrix.v[1].y * y) + (matrix.v[2].y * z) + (matrix.v[3].y * w);
			m.v[1].z = (matrix.v[0].z * x) + (matrix.v[1].z * y) + (matrix.v[2].z * z) + (matrix.v[3].z * w);
			m.v[1].w = (matrix.v[0].w * x) + (matrix.v[1].w * y) + (matrix.v[2].w * z) + (matrix.v[3].w * w);

			x = v[2].x;
			y = v[2].y;
			z = v[2].z;
			w = v[2].w;
			m.v[2].x = (matrix.v[0].x * x) + (matrix.v[1].x * y) + (matrix.v[2].x * z) + (matrix.v[3].x * w);
			m.v[2].y = (matrix.v[0].y * x) + (matrix.v[1].y * y) + (matrix.v[2].y * z) + (matrix.v[3].y * w);
			m.v[2].z = (matrix.v[0].z * x) + (matrix.v[1].z * y) + (matrix.v[2].z * z) + (matrix.v[3].z * w);
			m.v[2].w = (matrix.v[0].w * x) + (matrix.v[1].w * y) + (matrix.v[2].w * z) + (matrix.v[3].w * w);

			x = v[3].x;
			y = v[3].y;
			z = v[3].z;
			w = v[3].w;
			m.v[3].x = (matrix.v[0].x * x) + (matrix.v[1].x * y) + (matrix.v[2].x * z) + (matrix.v[3].x * w);
			m.v[3].y = (matrix.v[0].y * x) + (matrix.v[1].y * y) + (matrix.v[2].y * z) + (matrix.v[3].y * w);
			m.v[3].z = (matrix.v[0].z * x) + (matrix.v[1].z * y) + (matrix.v[2].z * z) + (matrix.v[3].z * w);
			m.v[3].w = (matrix.v[0].w * x) + (matrix.v[1].w * y) + (matrix.v[2].w * z) + (matrix.v[3].w * w);
			return m;
		}

		inline Matrix4x4 Transpose(Matrix4x4 Matrix) {
			Matrix4x4 tempMatrix;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					tempMatrix.v[i].v[j] = Matrix.v[j].v[i];
				}
			}

			return tempMatrix;
		}

		inline Matrix4x4 Translation(float X, float Y, float Z) {
			Matrix4x4 translationMatrix;
			translationMatrix.v[0].x = 1.0f;
			translationMatrix.v[1].y = 1.0f;
			translationMatrix.v[2].z = 1.0f;
			translationMatrix.v[3].w = 1.0f;
			translationMatrix.v[3].x = X;
			translationMatrix.v[3].y = Y;
			translationMatrix.v[3].z = Z;
			return translationMatrix;
		}

		inline Matrix4x4 Translation(Vector3 translationVector) {
			Matrix4x4 translationMatrix;
			translationMatrix.v[0].x = 1.0f;
			translationMatrix.v[1].y = 1.0f;
			translationMatrix.v[2].z = 1.0f;
			translationMatrix.v[3].w = 1.0f;
			translationMatrix.v[3].x = translationVector.x;
			translationMatrix.v[3].y = translationVector.y;
			translationMatrix.v[3].z = translationVector.z;
			return translationMatrix;
		}

		inline Matrix4x4 RotationRollPitchYaw(float Roll, float Pitch, float Yaw) {
			Matrix4x4 rotMat;
			rotMat = RotationRoll(Roll)
				* RotationPitch(Pitch)
				* RotationYaw(Yaw);

			return rotMat;
		}

		inline Matrix4x4 RotationRollPitchYaw(Vector3 RollPitchYawVector) {
			Matrix4x4 rotMat;
			rotMat = RotationRoll(RollPitchYawVector.x)
				* RotationPitch(RollPitchYawVector.y)
				* RotationYaw(RollPitchYawVector.z);

			return rotMat;
		}

		inline Matrix4x4 RotationRoll(float degrees) { // X
			Matrix4x4 rotationMatrixRoll;
			rotationMatrixRoll.v[0].x = 1.0f;
			
			rotationMatrixRoll.v[1].y = cos(radians(degrees));
			rotationMatrixRoll.v[1].z = sin(radians(degrees));

			rotationMatrixRoll.v[2].z = cos(radians(degrees));
			rotationMatrixRoll.v[2].y = -sin(radians(degrees));
			rotationMatrixRoll.v[3].w = 1;
			
			
			return rotationMatrixRoll;
		}

		inline Matrix4x4 RotationPitch(float degrees) { // Y
			Matrix4x4 rotationMatrixPitch;
			rotationMatrixPitch.v[0].x = cos(radians(degrees));
			rotationMatrixPitch.v[1].y = 1.0f;
			rotationMatrixPitch.v[0].z = -sin(radians(degrees));
			rotationMatrixPitch.v[2].x = sin(radians(degrees));
			rotationMatrixPitch.v[2].z = cos(radians(degrees));
			rotationMatrixPitch.v[3].w = 1;
			return rotationMatrixPitch;
		}

		inline Matrix4x4 RotationYaw(float degrees) { // Z
			Matrix4x4 rotationMatrixYaw;
			rotationMatrixYaw.v[0].x = cos(radians(degrees));
			rotationMatrixYaw.v[0].y = sin(radians(degrees));
			rotationMatrixYaw.v[1].x = -sin(radians(degrees));
			rotationMatrixYaw.v[1].y = cos(radians(degrees));
			rotationMatrixYaw.v[2].z = 1;
			rotationMatrixYaw.v[3].w = 1;
			return rotationMatrixYaw;
		}

		inline Matrix4x4 Scaling(float X, float Y, float Z) {
			Matrix4x4 scalingMatrix;
			scalingMatrix.v[0].x = X;
			scalingMatrix.v[1].y = Y;
			scalingMatrix.v[2].z = Z;
			scalingMatrix.v[3].w = 1;
			return scalingMatrix;
		}

		inline Matrix4x4 Scaling(Vector3 scalingVector) {
			Matrix4x4 scalingMatrix;
			scalingMatrix.v[0].x = scalingVector.x;
			scalingMatrix.v[1].y = scalingVector.y;
			scalingMatrix.v[2].z = scalingVector.z;
			scalingMatrix.v[3].w = 1;
			return scalingMatrix;
		}

		inline Matrix4x4 PerspectiveProjection(float FOV, float WindowWidth, float WindowHeight, float ZClipNear, float ZClipFar) {
			Matrix4x4 projectionMatrix;
			projectionMatrix.v[0].x = (WindowWidth / WindowHeight) * (1.0/tan(FOV * 0.5));
			projectionMatrix.v[1].y = 1.0/tan(FOV * 0.5);
			projectionMatrix.v[2].z = ZClipFar / (ZClipFar -  ZClipNear);
			projectionMatrix.v[2].w = 1;
			projectionMatrix.v[3].z = -ZClipNear * (ZClipFar / (ZClipFar - ZClipNear));
			return projectionMatrix;
		}

		inline Matrix4x4 LookAtView(Vector3 Position, Vector3 LookAt, Vector3 UpDir) {
			Matrix4x4 lookAtMatrix;

			Vector3 lookVec = normalize(LookAt - Position);
			Vector3 rightVec = normalize(CrossProduct(UpDir, lookVec));
			Vector3 upVec = CrossProduct(lookVec, rightVec);
			float A = -DotProduct(rightVec, Position);
			float B = -DotProduct(upVec, Position);
			float C = -DotProduct(lookVec, Position);
			lookAtMatrix.v[0].x = rightVec.x;
			lookAtMatrix.v[0].y = upVec.x;
			lookAtMatrix.v[0].z = lookVec.x;

			lookAtMatrix.v[1].x = rightVec.y;
			lookAtMatrix.v[1].y = upVec.y;
			lookAtMatrix.v[1].z = lookVec.y;

			lookAtMatrix.v[2].x = rightVec.z;
			lookAtMatrix.v[2].y = upVec.z;
			lookAtMatrix.v[2].z = lookVec.z;

			lookAtMatrix.v[3].x = A;
			lookAtMatrix.v[3].y = B;
			lookAtMatrix.v[3].z = C;

			lookAtMatrix.v[3].w = 1.0f;

			return lookAtMatrix;
		}
	
}
