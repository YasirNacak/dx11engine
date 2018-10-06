#include "Graphics.h"
#include <direct.h>

namespace s3d { namespace graphics {
	bool Graphics::Initialize(HWND hwnd, int width, int height)
	{
		if (!InitializeDirectX(hwnd, width, height))
			return false;

		if (!InitializeShaders())
			return false;

		return true;
	}

	void Graphics::RenderFrame()
	{
		float bgColor[] = {0.0f, 46.0f / 255.0f, 102.0f / 255.0f, 1.0f};
		this->_deviceContext->ClearRenderTargetView(this->_renderTargetView.Get(), bgColor);
		this->_swapChain->Present(1, NULL);
	}

	bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
	{
		auto adapters = AdapterReader::GetAdapters();

		if(adapters.empty())
		{
			utility::ErrorLogger::Log("No DXGI adapters found.");
			return false;
		}

		// https://docs.microsoft.com/en-us/windows/desktop/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
		auto hr = D3D11CreateDeviceAndSwapChain(
			adapters[0].PAdapter,
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

		this->_deviceContext->OMSetRenderTargets(1, this->_renderTargetView.GetAddressOf(), NULL);

		return true;
	}

	bool Graphics::InitializeShaders()
	{
		std::wstring shaderFolder;
#pragma region DetermineShaderPath
		if(IsDebuggerPresent() == TRUE)
		{
#ifdef _DEBUG
#ifdef _WIN64
			shaderFolder = L"..\\bin\\build\\x64\\Debug\\";
#else
			shaderFolder = L"..\\bin\\build\\Win32\\Debug\\";
#endif
#else
#ifdef _WIN64
			shaderFolder = L"..\\bin\\build\\x64\\Release\\";
#else
			shaderFolder = L"..\\bin\\build\\Win32\\Release\\";
#endif
#endif
		}

		char cCurrentPath[FILENAME_MAX];

		if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
		{
			return errno;
		}

		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

		OutputDebugStringA(cCurrentPath);

		if(!_vertexShader.Initialize(this->_device, shaderFolder + L"vertexshader.cso"))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = this->_device->CreateInputLayout(
			layout,
			ARRAYSIZE(layout),
			this->_vertexShader.GetBuffer()->GetBufferPointer(),
			this->_vertexShader.GetBuffer()->GetBufferSize(),
			this->_inputLayout.GetAddressOf());

		if(FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create input layout");
			return false;
		}

		return true;
	}
} }