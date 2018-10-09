#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

namespace s3d { namespace graphics {
	template <class T>
	class VertexBuffer{
	private:
		VertexBuffer(const VertexBuffer<T>& rhs);
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
		std::unique_ptr<UINT> _stride;
		UINT _bufferSize = 0;
	public:
		VertexBuffer(){}

		ID3D11Buffer* GetBuffer() const
		{
			return this->_buffer.Get();
		}

		ID3D11Buffer* const* GetBufferAddress() const
		{
			return this->_buffer.GetAddressOf();
		}

		UINT GetBufferSize() const
		{
			return this->_bufferSize;
		}

		UINT GetStride() const
		{
			return *this->_stride.get();
		}

		const UINT* GetStridePtr() const
		{
			return this->_stride.get();
		}

		HRESULT Initialize(ID3D11Device *device, T *data, UINT numVertices)
		{
			this->_bufferSize = numVertices;
			this->_stride = std::make_unique<UINT>(sizeof(T));

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(T) * numVertices;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof D3D11_SUBRESOURCE_DATA);
			bufferData.pSysMem = data;

			return device->CreateBuffer(&bufferDesc, &bufferData, this->_buffer.GetAddressOf());
		}
	};
} }