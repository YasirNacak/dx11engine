#pragma once
#include <DirectXMath.h>
using DirectX::XMVECTOR;
using DirectX::XMMATRIX;
using DirectX::XMFLOAT3;

namespace s3d { namespace graphics {
	class Camera
	{
	public:
		const XMVECTOR DefaultForwardVector		= DirectX::XMVectorSet( 0.0f,  0.0f,  1.0f,  0.0f);
		const XMVECTOR DefaultBackwardVector	= DirectX::XMVectorSet( 0.0f,  0.0f, -1.0f,  0.0f);
		const XMVECTOR DefaultUpVector			= DirectX::XMVectorSet( 0.0f,  1.0f,  0.0f,  0.0f);
		const XMVECTOR DefaultDownVector		= DirectX::XMVectorSet( 0.0f, -1.0f,  0.0f,  0.0f);
		const XMVECTOR DefaultRightVector		= DirectX::XMVectorSet( 1.0f,  0.0f,  0.0f,  0.0f);
		const XMVECTOR DefaultLeftVector		= DirectX::XMVectorSet(-1.0f,  0.0f,  0.0f,  0.0f);

	public:
		Camera();
		void SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ);

		const XMMATRIX& GetViewMatrix() const;
		const XMMATRIX& GetProjectionMatrix() const;

		const XMVECTOR& GetPositionVector() const;
		const XMFLOAT3& GetPosition() const;
		const XMVECTOR& GetRotationVector() const;
		const XMFLOAT3& GetRotation() const;

		const XMVECTOR& GetForwardVector() const;
		const XMVECTOR& GetBackwardVector() const;
		const XMVECTOR& GetLeftVector() const;
		const XMVECTOR& GetRightVector() const;

		void SetPosition(const XMVECTOR& position);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const XMVECTOR& position);
		void AdjustPosition(float x, float y, float z);
	
		void SetRotation(const XMVECTOR& rotation);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const XMVECTOR& rotation);
		void AdjustRotation(float x, float y, float z);

		void SetLookAtPosition(XMFLOAT3 lookAtPosition);
	
	private:
		void UpdateViewMatrix();

		XMFLOAT3 _position;
		XMVECTOR _positionVector;
		
		XMFLOAT3 _rotation;
		XMVECTOR _rotationVector;
		
		XMMATRIX _viewMatrix;
		XMMATRIX _projectionMatrix;
	
		XMVECTOR _forwardVector;
		XMVECTOR _backwardVector;

		XMVECTOR _leftVector;
		XMVECTOR _rightVector;
	};
} }
