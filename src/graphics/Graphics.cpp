#include "Graphics.h"
#include "../Engine.h"
#include <direct.h>
#include <minwinbase.h>

namespace s3d { namespace graphics {
	bool Graphics::Initialize(HWND hwnd, int width, int height)
	{
		this->_fpsTimer.Start();

		this->_windowWidth = width;
		this->_windowHeight = height;

		if (!InitializeDirectX(hwnd))
			return false;

		if (!InitializeShaders())
			return false;

		if (!InitializeScene())
			return false;

		//imgui init
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(this->_device.Get(), this->_deviceContext.Get());
		ImGui::StyleColorsClassic();
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::GetStyle().ScrollbarRounding = 0.0f;

		return true;
	}

	bool Graphics::InitializeDirectX(HWND hwnd)
	{
		auto adapters = AdapterReader::GetAdapters();

		if(adapters.empty())
		{
			utility::ErrorLogger::Log("No DXGI adapters found.");
			return false;
		}

		AdapterData highestMemAdapter = adapters[0];

		for (int i = 1; i < adapters.size(); i++)
		{
			if(adapters[i].Description.DedicatedVideoMemory > highestMemAdapter.Description.DedicatedVideoMemory)
			{
				highestMemAdapter = adapters[i];
			}
		}

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = this->_windowWidth;
		scd.BufferDesc.Height = this->_windowHeight;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 4;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		auto hr = D3D11CreateDeviceAndSwapChain(
			highestMemAdapter.PAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			NULL,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&scd,
			this->_swapChain.GetAddressOf(),
			this->_device.GetAddressOf(),
			NULL,
			this->_deviceContext.GetAddressOf());

		//this->_swapChain->SetFullscreenState(true, NULL);

		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating device and swap chain failed.");
			return false;
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Back buffer could not be get.");
			return false;
		}

		hr = this->_device->CreateRenderTargetView(backBuffer.Get(), NULL, this->_renderTargetView.GetAddressOf());
		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating render target view failed.");
			return false;
		}

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;

		depthStencilBufferDesc.Width = this->_windowWidth;
		depthStencilBufferDesc.Height = this->_windowHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.SampleDesc.Count = scd.SampleDesc.Count;
		depthStencilBufferDesc.SampleDesc.Quality = scd.SampleDesc.Quality;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		hr = this->_device->CreateTexture2D(&depthStencilBufferDesc, NULL, this->_depthStencilBuffer.GetAddressOf());
		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating depth stencil buffer failed.");
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof D3D11_DEPTH_STENCIL_VIEW_DESC);
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = this->_device->CreateDepthStencilView(this->_depthStencilBuffer.Get(), &depthStencilViewDesc, this->_depthStencilView.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating depth stencil view failed.");
			return false;
		}

		this->_deviceContext->OMSetRenderTargets(1, this->_renderTargetView.GetAddressOf(), this->_depthStencilView.Get());

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof D3D11_DEPTH_STENCIL_DESC);

		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = this->_device->CreateDepthStencilState(&depthStencilStateDesc, this->_depthStencilState.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating depth stencil state failed.");
			return false;
		}

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof D3D11_VIEWPORT);

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(this->_windowWidth);
		viewport.Height = static_cast<float>(this->_windowHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		this->_deviceContext->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof D3D11_RASTERIZER_DESC);

		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;

		hr = this->_device->CreateRasterizerState(&rasterizerDesc, this->_rasterizerState.GetAddressOf());
		if(hr != S_OK)
		{
			utility::ErrorLogger::Log(hr, "Creating rasterizer state failed.");
			return false;
		}

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof D3D11_BLEND_DESC);

		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
		ZeroMemory(&renderTargetBlendDesc, sizeof D3D11_RENDER_TARGET_BLEND_DESC);

		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		hr = this->_device->CreateBlendState(&blendDesc, this->_blendState.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create blend state.");
			return false;
		}

		_spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->_deviceContext.Get());
		_spriteFont = std::make_unique<DirectX::SpriteFont>(this->_device.Get(), L"data\\typeface\\argentum_sans_regular_16.spritefont");

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof D3D11_SAMPLER_DESC);

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = this->_device->CreateSamplerState(&samplerDesc, this->_samplerState.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Creating sampler state failed.");
			return false;
		}

		return true;
	}

	void Graphics::RenderFrame(Engine& engine)
	{
		float bgColor[] = {0.0f, 46.0f / 255.0f, 102.0f / 255.0f, 1.0f}; // keeping this very important color x)
		this->_deviceContext->ClearRenderTargetView(this->_renderTargetView.Get(), bgColor);
		this->_deviceContext->ClearDepthStencilView(this->_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		this->_deviceContext->IASetInputLayout(this->_vertexShader.GetInputLayout());
		this->_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->_deviceContext->RSSetState(this->_rasterizerState.Get());
		this->_deviceContext->OMSetDepthStencilState(this->_depthStencilState.Get(), 0);
		this->_deviceContext->OMSetBlendState(this->_blendState.Get(), NULL, 0xFFFFFFFF);
		this->_deviceContext->PSSetSamplers(0, 1, this->_samplerState.GetAddressOf());
		this->_deviceContext->VSSetShader(this->_vertexShader.GetShader(), NULL, 0);
		this->_deviceContext->PSSetShader(this->_pixelShader.GetShader(), NULL, 0);

		UINT offset = 0;

		static float alpha = 1.0f;

		{
			// PAVEMENT
			float translationOffset[3] = { 0, 0, 4.0f };
			XMMATRIX worldMatrix =
				DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f) *
				DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
			this->_vertexShaderConstantBuffer.Data.mat4 = worldMatrix * MainCamera.GetViewMatrix() * MainCamera.GetProjectionMatrix();
			this->_vertexShaderConstantBuffer.Data.mat4 = XMMatrixTranspose(this->_vertexShaderConstantBuffer.Data.mat4);
			if (!this->_vertexShaderConstantBuffer.ApplyChanges())
			{
				return;
			}
			this->_deviceContext->VSSetConstantBuffers(0, 1, this->_vertexShaderConstantBuffer.GetBufferAddress());

			this->_pixelShaderConstantBuffer.Data.alpha = 1.0f;
			this->_pixelShaderConstantBuffer.ApplyChanges();
			this->_deviceContext->PSSetConstantBuffers(0, 1, this->_pixelShaderConstantBuffer.GetBufferAddress());

			this->_deviceContext->PSSetShaderResources(0, 1, this->_pavementExampleTexture.GetAddressOf());
			this->_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetBufferAddress(), _vertexBuffer.GetStridePtr(), &offset);
			this->_deviceContext->IASetIndexBuffer(this->_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

			this->_deviceContext->DrawIndexed(_indexBuffer.GetBufferSize(), 0, 0);
		}

		{
			// GRASS
			float translationOffset[3] = { 0, 0, 0 };
			XMMATRIX worldMatrix =
				DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f) *
				DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
			this->_vertexShaderConstantBuffer.Data.mat4 = worldMatrix * MainCamera.GetViewMatrix() * MainCamera.GetProjectionMatrix();
			this->_vertexShaderConstantBuffer.Data.mat4 = XMMatrixTranspose(this->_vertexShaderConstantBuffer.Data.mat4);
			if (!this->_vertexShaderConstantBuffer.ApplyChanges())
			{
				return;
			}
			this->_deviceContext->VSSetConstantBuffers(0, 1, this->_vertexShaderConstantBuffer.GetBufferAddress());

			this->_pixelShaderConstantBuffer.Data.alpha = 1.0f;
			this->_pixelShaderConstantBuffer.ApplyChanges();
			this->_deviceContext->PSSetConstantBuffers(0, 1, this->_pixelShaderConstantBuffer.GetBufferAddress());

			this->_deviceContext->PSSetShaderResources(0, 1, this->_grassExampleTexture.GetAddressOf());
			this->_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetBufferAddress(), _vertexBuffer.GetStridePtr(), &offset);
			this->_deviceContext->IASetIndexBuffer(this->_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

			this->_deviceContext->DrawIndexed(_indexBuffer.GetBufferSize(), 0, 0);
		}

		{
			// PINK
			float translationOffset[3] = { 0, 0, -1.0f };
			XMMATRIX worldMatrix =
				DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
			this->_vertexShaderConstantBuffer.Data.mat4 = worldMatrix * MainCamera.GetViewMatrix() * MainCamera.GetProjectionMatrix();
			this->_vertexShaderConstantBuffer.Data.mat4 = XMMatrixTranspose(this->_vertexShaderConstantBuffer.Data.mat4);
			if (!this->_vertexShaderConstantBuffer.ApplyChanges())
			{
				return;
			}
			this->_deviceContext->VSSetConstantBuffers(0, 1, this->_vertexShaderConstantBuffer.GetBufferAddress());

			this->_pixelShaderConstantBuffer.Data.alpha = alpha;
			this->_pixelShaderConstantBuffer.ApplyChanges();
			this->_deviceContext->PSSetConstantBuffers(0, 1, this->_pixelShaderConstantBuffer.GetBufferAddress());

			this->_deviceContext->PSSetShaderResources(0, 1, this->_pinkExampleTexture.GetAddressOf());
			this->_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetBufferAddress(), _vertexBuffer.GetStridePtr(), &offset);
			this->_deviceContext->IASetIndexBuffer(this->_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

			this->_deviceContext->DrawIndexed(_indexBuffer.GetBufferSize(), 0, 0);
		}
#if _DEBUG
		static int fpsCounter = 0;
		static std::string fpsString = "FPS: 0";
		fpsCounter += 1;
		if (_fpsTimer.GetMillisecondsElapsed() > 1000.0)
		{
			fpsString = "FPS: " + std::to_string(fpsCounter);
			fpsCounter = 0;
			_fpsTimer.Restart();
		}
		engine.ShowDebugPanels();
#endif
		_spriteBatch->Begin();
#if _DEBUG
		_spriteFont->DrawString(_spriteBatch.get(), utility::StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(_windowWidth - 100, 0), DirectX::Colors::Lime, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
#endif
		_spriteBatch->End();

		this->_swapChain->Present(0, NULL);
	}

	bool Graphics::InitializeShaders()
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		if(!_vertexShader.Initialize(this->_device, L"data\\shader\\vertexshader.cso", layout, ARRAYSIZE(layout)))
			return false;

		if (!_pixelShader.Initialize(this->_device, L"data\\shader\\pixelshader.cso"))
			return false;

		return true;
	}

	// This method is being used for testing purposes only.
	// It is going to change completely in future.
	bool Graphics::InitializeScene()
	{
		// QUAD
		Vertex v[] = {
			{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 0.0f, 0.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
		};

		HRESULT hr = this->_vertexBuffer.Initialize(this->_device.Get(), v, ARRAYSIZE(v));
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create vertex buffer.");
			return false;
		}

		DWORD indices[] = {
			0, 1, 2,
			0, 2, 3, 
		};

		hr = this->_indexBuffer.Initialize(this->_device.Get(), indices, ARRAYSIZE(indices));
		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create indices buffer.");
			return false;
		}

		hr = DirectX::CreateWICTextureFromFile(this->_device.Get(), L"data\\graphic\\texture\\grass.jpg", nullptr, _grassExampleTexture.GetAddressOf());
		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create WIC texture from file buffer.");
			return false;
		}

		hr = DirectX::CreateWICTextureFromFile(this->_device.Get(), L"data\\graphic\\texture\\pink_square.jpg", nullptr, _pinkExampleTexture.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create WIC texture from file buffer.");
			return false;
		}

		hr = DirectX::CreateWICTextureFromFile(this->_device.Get(), L"data\\graphic\\texture\\pavement.jpg", nullptr, _pavementExampleTexture.GetAddressOf());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create WIC texture from file buffer.");
			return false;
		}

		hr = this->_vertexShaderConstantBuffer.Initialize(this->_device.Get(), this->_deviceContext.Get());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create constant buffer for vertex shader.");
			return false;
		}

		hr = this->_pixelShaderConstantBuffer.Initialize(this->_device.Get(), this->_deviceContext.Get());
		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create constant buffer for pixel shader.");
			return false;
		}

		MainCamera.SetPosition(0.0f, 0.0f, -2.0f);
		MainCamera.SetProjectionValues(90, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight), 0.1f, 1000.0f);

		return true;
	}
} }