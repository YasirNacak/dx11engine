#pragma once
#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include <wrl/client.h>
#include "..//utility/ErrorLogger.h"

namespace s3d { namespace graphics {
	template <class T>
	class ConstantBuffer
	{
	private:
		ConstantBuffer(const ConstantBuffer<T>& rhs);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
		ID3D11DeviceContext *_deviceContext = nullptr;
	public:
		T Data;

		ConstantBuffer() {}
		ID3D11Buffer* GetBuffer() const
		{
			return this->_buffer.Get();
		}

		ID3D11Buffer* const* GetBufferAddress() const
		{
			return this->_buffer.GetAddressOf();
		}

		HRESULT Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
		{
			this->_deviceContext = deviceContext;

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

			return device->CreateBuffer(&bufferDesc, 0, this->_buffer.GetAddressOf());
		}

		bool ApplyChanges()
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubresource;
			HRESULT hr = this->_deviceContext->Map(this->_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			if(FAILED(hr))
			{
				utility::ErrorLogger::Log("Failed to map constant buffer.");
				return false;
			}

			CopyMemory(mappedSubresource.pData, &Data, sizeof T);
			this->_deviceContext->Unmap(this->_buffer.Get(), 0);
			return true;
		}
	};
} }