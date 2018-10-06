#include "Shader.h"

namespace s3d { namespace graphics
{
	bool VertexShader::Initialize(ComPtr<ID3D11Device>& device, std::wstring shaderPath)
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

} }