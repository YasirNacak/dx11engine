#pragma once

#include "../Component.h"
#include <DirectXMath.h>

namespace s3d {namespace entitysystem {namespace components
{
	struct TransformComponent : Component
	{
		DirectX::XMFLOAT3 Position{};
		DirectX::XMFLOAT3 Rotation{};
		DirectX::XMFLOAT3 Scale{};

		void Init() override
		{
			Position = { 0, 0, 0 };
			Rotation = { 0, 0, 0 };
			Scale = { 0, 0, 0 };
			Name = "Transform";
		}
	};
} } }
