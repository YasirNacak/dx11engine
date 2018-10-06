#pragma once
#include <DirectXMath.h>

namespace s3d { namespace graphics {
	struct Vertex
	{
		Vertex(float x = 0, float y = 0, float r = 0, float g = 0, float b = 0) :
			Position(x, y), Color(r, g, b)
		{
		}

		DirectX::XMFLOAT2 Position;
		DirectX::XMFLOAT3 Color;
	};
} }