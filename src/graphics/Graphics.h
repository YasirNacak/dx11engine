#pragma once
#include "AdapterReader.h"
#include "Shader.h"
using Microsoft::WRL::ComPtr;

namespace s3d { namespace graphics {
	class Graphics
	{
	public:
		bool Initialize(HWND hwnd, int width, int height);
		void RenderFrame();
	private:
		bool InitializeDirectX(HWND hwnd, int width, int height);
		bool InitializeShaders();

		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;
		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		VertexShader _vertexShader;
		PixelShader _pixelShader;
	};
} }