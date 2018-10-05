#pragma once
#include "AdapterReader.h"

namespace s3d
{
	namespace graphics
	{
		class Graphics
		{
		public:
			bool Initialize(HWND hwnd, int width, int height);
			void RenderFrame();
		private:
			bool InitializeDirectX(HWND hwnd, int width, int height);

			Microsoft::WRL::ComPtr<ID3D11Device> _device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
			Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;
		};
	}
}