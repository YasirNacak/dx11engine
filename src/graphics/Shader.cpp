#include "Shader.h"

namespace s3d { namespace graphics
{
	bool VertexShader::Initialize(ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC *layoutDesc, UINT elementCount)
	{
		HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->_shaderBuffer.GetAddressOf());
		if(FAILED(hr))
		{
			std::wstring errorMessage = L"Failed to load shader: ";
			errorMessage += shaderPath;
			utility::ErrorLogger::Log(hr, errorMessage);
			return false;
		}

		hr = device->CreateVertexShader(this->_shaderBuffer.Get()->GetBufferPointer(),
		                                this->_shaderBuffer->GetBufferSize(), NULL, this->_shader.GetAddressOf());
		if(FAILED(hr))
		{
			std::wstring errorMessage = L"Failed to create vertex shader: ";
			errorMessage += shaderPath;
			utility::ErrorLogger::Log(hr, errorMessage);
			return false;
		}

		hr = device->CreateInputLayout(
			layoutDesc,
			elementCount,
			this->_shaderBuffer->GetBufferPointer(),
			this->_shaderBuffer->GetBufferSize(),
			this->_inputLayout.GetAddressOf());

		if (FAILED(hr))
		{
			utility::ErrorLogger::Log(hr, "Failed to create input layout");
			return false;
		}

		return true;
	}

	ID3D11VertexShader* VertexShader::GetShader() const
	{
		return this->_shader.Get();
	}

	ID3D10Blob* VertexShader::GetBuffer() const
	{
		return this->_shaderBuffer.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout() const
	{
		return this->_inputLayout.Get();
	}
} }