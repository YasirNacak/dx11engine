#pragma once
#include <DirectXMath.h>

namespace s3d { namespace graphics {
	struct Vertex
	{
		Vertex(float x = 0, float y = 0, float z = 0, float u = 0, float v = 0) :
			Position(x, y, z), TexCoord(u, v)
		{
		}

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TexCoord;
	};
} }