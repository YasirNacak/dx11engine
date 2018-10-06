#pragma once
#pragma comment(lib, "D3DCompiler.lib")
#include "../utility/ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

namespace s3d { namespace graphics {
	class VertexShader
	{
	public:
		bool Initialize(ComPtr<ID3D11Device> &device, std::wstring shaderPath);
		ID3D11VertexShader* GetShader() const;
		ID3D10Blob* GetBuffer() const;
	private:
		ComPtr<ID3D11VertexShader> _shader = nullptr;
		ComPtr<ID3D10Blob> _shaderBuffer = nullptr;
	};
} }