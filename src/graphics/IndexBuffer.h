#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

namespace s3d
{
	namespace graphics
	{
		class IndexBuffer
		{
		private:
			IndexBuffer(const IndexBuffer& rhs);
		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
			UINT _bufferSize;
		public:
			IndexBuffer() {}

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

			HRESULT Initialize(ID3D11Device *device, DWORD *data, UINT numIndices)
			{
				this->_bufferSize = numIndices;

				D3D11_BUFFER_DESC bufferDesc;
				ZeroMemory(&bufferDesc, sizeof D3D11_BUFFER_DESC);

				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA bufferData;
				bufferData.pSysMem = data;
				HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, this->_buffer.GetAddressOf());
				return hr;
			}
		};
	}
}
