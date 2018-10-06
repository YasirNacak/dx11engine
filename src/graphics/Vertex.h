#pragma once
#include <DirectXMath.h>

namespace s3d { namespace graphics {
	struct Vertex
	{
		Vertex() :
			Position(0, 0)
		{
		}

		Vertex(float x, float y) :
			Position(x, y)
		{
		}

		DirectX::XMFLOAT2 Position;
	};
} }