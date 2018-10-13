#include "Camera.h"
#include <cmath>
#include <complex>

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

	const XMVECTOR& Camera::GetForwardVector() const
	{
		return this->_forwardVector;
	}

	const XMVECTOR& Camera::GetBackwardVector() const
	{
		return this->_backwardVector;
	}

	const XMVECTOR& Camera::GetLeftVector() const
	{
		return this->_leftVector;
	}

	const XMVECTOR& Camera::GetRightVector() const
	{
		return this->_rightVector;
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
		this->UpdateViewMatrix();
	}

	void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
	{
		if (lookAtPosition.x == this->_position.x &&
			lookAtPosition.y == this->_position.y &&
			lookAtPosition.z == this->_position.z)
			return;

		lookAtPosition.x = this->_position.x - lookAtPosition.x;
		lookAtPosition.y = this->_position.y - lookAtPosition.y;
		lookAtPosition.z = this->_position.z - lookAtPosition.z;

		float pitch = 0.0f;
		if (lookAtPosition.y != 0.0f)
		{
			const float distance = sqrt(lookAtPosition.x * lookAtPosition.x + lookAtPosition.z * lookAtPosition.z);
			pitch = atan(lookAtPosition.y / distance);
		}

		float yaw = 0.0f;
		if (lookAtPosition.x != 0.0f)
		{
			yaw = atan(lookAtPosition.x / lookAtPosition.z);
		}

		if (lookAtPosition.z > 0)
			yaw += DirectX::XM_PI;

		this->SetRotation(pitch, yaw, 0.0f);
	}

	void Camera::UpdateViewMatrix()
	{
		XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->_rotation.x, this->_rotation.y, this->_rotation.z);
		XMVECTOR target = XMVector3TransformCoord(this->DefaultForwardVector, rotationMatrix);
		target = DirectX::XMVectorAdd(target, this->_positionVector);
		//target += this->_positionVector;
		XMVECTOR upDirection = XMVector3TransformCoord(this->DefaultUpVector, rotationMatrix);
		this->_viewMatrix = DirectX::XMMatrixLookAtLH(this->_positionVector, target, upDirection);

		rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->_rotation.y, 0.0f);

		this->_forwardVector = XMVector3TransformCoord(this->DefaultForwardVector, rotationMatrix);
		this->_backwardVector = XMVector3TransformCoord(this->DefaultBackwardVector, rotationMatrix);
		this->_leftVector = XMVector3TransformCoord(this->DefaultLeftVector, rotationMatrix);
		this->_rightVector = XMVector3TransformCoord(this->DefaultRightVector, rotationMatrix);
	}
} }