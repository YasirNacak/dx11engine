#pragma once
#include "AdapterReader.h"
#include "Shader.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "../utility/Timer.h"

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_win32.h"
#include "../../external/imgui/imgui_impl_dx11.h"
#include "../entitysystem/Entity.h"

using Microsoft::WRL::ComPtr;

namespace s3d
{
	class Engine;
}

namespace s3d { namespace graphics {
	class Graphics
	{
	public:
		bool Initialize(HWND hwnd, int width, int height);
		void RenderFrame(Engine& engine);

		Camera MainCamera;

	private:
		bool InitializeDirectX(HWND hwnd);
		bool InitializeImGui(HWND hwnd);
		bool InitializeShaders();
		bool InitializeScene();

		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;
		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		
		VertexShader _vertexShader;
		PixelShader _pixelShader;

		ConstantBuffer<CB_VS_vertexshader> _vertexShaderConstantBuffer;
		ConstantBuffer<CB_PS_pixelshader> _pixelShaderConstantBuffer;

		VertexBuffer<Vertex> _vertexBuffer;
		IndexBuffer _indexBuffer;

		ComPtr<ID3D11DepthStencilView> _depthStencilView;
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilState> _depthStencilState;

		ComPtr<ID3D11RasterizerState> _rasterizerState;
		ComPtr<ID3D11BlendState> _blendState;

		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> _spriteFont;

		ComPtr<ID3D11SamplerState> _samplerState;
		ComPtr<ID3D11ShaderResourceView> _grassExampleTexture;
		ComPtr<ID3D11ShaderResourceView> _pavementExampleTexture;
		ComPtr<ID3D11ShaderResourceView> _pinkExampleTexture;

		int _windowWidth;
		int _windowHeight;

		utility::Timer _fpsTimer;
	};
} }