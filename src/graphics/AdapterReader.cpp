#include "AdapterReader.h"

namespace s3d
{
	namespace graphics
	{
		std::vector<AdapterData> AdapterReader::_adapters;

		std::vector<AdapterData> AdapterReader::GetAdapters()
		{
			if (!_adapters.empty())
				return _adapters;

			Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

			const auto hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));

			if(FAILED(hr))
			{
				utility::ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
				exit(-1);
			}

			IDXGIAdapter *pAdapter;
			UINT index = 0;
			while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
			{
				_adapters.emplace_back(pAdapter);
				index++;
			}
			return _adapters;
		}

		AdapterData::AdapterData(IDXGIAdapter *pAdapter)
		{
			this->PAdapter = pAdapter;
			const HRESULT hr = pAdapter->GetDesc(&this->Description);
			if(FAILED(hr))
			{
				utility::ErrorLogger::Log(hr, "Failed to get IDXGIAdapter description.");
			}
		}
	}
}