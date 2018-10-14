#pragma once
#include <DirectXMath.h>

namespace s3d { namespace graphics {
	struct CB_VS_vertexshader
	{
		DirectX::XMMATRIX mat4;
	};

	struct CB_PS_pixelshader
	{
		float alpha = 1.0f;
	};
} }