#include "Camera.h"

namespace s3d { namespace graphics
{
	Camera::Camera()
	{
		this->_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->_positionVector = XMLoadFloat3(&this->_position);

		this->_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->_rotationVector = XMLoadFloat3(&this->_rotation);
	
		UpdateViewMatrix();
	}

	void Camera::SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ)
	{
		const float fovToRadians = (fov / 360.0f) * DirectX::XM_2PI;
		this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovToRadians, aspectRatio, nearZ, farZ);
	}

	const XMMATRIX& Camera::GetViewMatrix() const
	{
		return this->_viewMatrix;
	}

	const XMMATRIX& Camera::GetProjectionMatrix() const
	{
		return this->_projectionMatrix;
	}

	const XMVECTOR& Camera::GetPositionVector() const
	{
		return this->_positionVector;
	}

	const XMFLOAT3& Camera::GetPosition() const
	{
		return this->_position;
	}

	const XMVECTOR& Camera::GetRotationVector() const
	{
		return this->_rotationVector;
	}

	const XMFLOAT3& Camera::GetRotation() const
	{
		return this->_rotation;
	}

	void Camera::SetPosition(const XMVECTOR& position)
	{
		XMStoreFloat3(&this->_position, position);
		this->_positionVector = position;
		this->UpdateViewMatrix();
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		this->_position = XMFLOAT3(x, y, z);
		this->_positionVector = DirectX::XMLoadFloat3(&this->_position);
		this->UpdateViewMatrix();
	}

	void Camera::AdjustPosition(const XMVECTOR& position)
	{
		this->_positionVector = DirectX::XMVectorAdd(this->_positionVector, position);
		//this->_positionVector += position;
		XMStoreFloat3(&this->_position, this->_positionVector);
		this->UpdateViewMatrix();
	}

	void Camera::AdjustPosition(float x, float y, float z)
	{
		this->_position.x += x;
		this->_position.y += y;
		this->_position.z += z;
		this->_positionVector = XMLoadFloat3(&this->_position);
		this->UpdateViewMatrix();
	}

	void Camera::SetRotation(const XMVECTOR& rotation)
	{
		this->_rotationVector = rotation;
		XMStoreFloat3(&this->_rotation, rotation);
		this->UpdateViewMatrix();
	}

	void Camera::SetRotation(float x, float y, float z)
	{
		this->_rotation = XMFLOAT3(x, y, z);
		this->_rotationVector = DirectX::XMLoadFloat3(&this->_rotation);
		this->UpdateViewMatrix();
	}

	void Camera::AdjustRotation(const XMVECTOR& rotation)
	{
		this->_rotationVector = DirectX::XMVectorAdd(this->_rotationVector, rotation);
		//this->_rotationVector += rotation;
		XMStoreFloat3(&this->_rotation, this->_rotationVector);
		this->UpdateViewMatrix();
	}

	void Camera::AdjustRotation(float x, float y, float z)
	{
		this->_rotation.x += x;
		this->_rotation.y += y;
		this->_rotation.z += z;
		this->_rotationVector = XMLoadFloat3(&this->_rotation);
	}

	void Camera::UpdateViewMatrix()
	{
		XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->_rotation.x, this->_rotation.y, this->_rotation.z);
		XMVECTOR target = XMVector3TransformCoord(this->DefaultForwardVector, rotationMatrix);
		target = DirectX::XMVectorAdd(target, this->_positionVector);
		//target += this->_positionVector;
		XMVECTOR upDirection = XMVector3TransformCoord(this->DefaultUpVector, rotationMatrix);
		this->_viewMatrix = DirectX::XMMatrixLookAtLH(this->_positionVector, target, upDirection);
	}
}}