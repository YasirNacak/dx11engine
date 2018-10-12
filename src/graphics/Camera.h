#pragma once
#include <DirectXMath.h>
using DirectX::XMVECTOR;
using DirectX::XMMATRIX;
using DirectX::XMFLOAT3;

namespace s3d { namespace graphics {
	class Camera
	{
	public:
		Camera();
		void SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ);

		const XMMATRIX& GetViewMatrix() const;
		const XMMATRIX& GetProjectionMatrix() const;

		const XMVECTOR& GetPositionVector() const;
		const XMFLOAT3& GetPosition() const;
		const XMVECTOR& GetRotationVector() const;
		const XMFLOAT3& GetRotation() const;

		void SetPosition(const XMVECTOR& position);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const XMVECTOR& position);
		void AdjustPosition(float x, float y, float z);
	
		void SetRotation(const XMVECTOR& rotation);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const XMVECTOR& rotation);
		void AdjustRotation(float x, float y, float z);

		void SetLookAtPosition(XMFLOAT3 lookAtPosition);

		const XMVECTOR DefaultForwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR DefaultUpVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	private:
		void UpdateViewMatrix();
		
		XMVECTOR _positionVector;
		XMVECTOR _rotationVector;
		XMFLOAT3 _position;
		XMFLOAT3 _rotation;
		XMMATRIX _viewMatrix;
		XMMATRIX _projectionMatrix;
	};
} }
