#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include "../utility/ErrorLogger.h"
#include <d3d11.h>
#include "wrl/client.h"
#include <vector>

namespace s3d { namespace graphics {
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter *pAdapter);
		IDXGIAdapter *PAdapter;
		DXGI_ADAPTER_DESC Description;
	};

	class AdapterReader
	{
	public:
		static std::vector<AdapterData> GetAdapters();
	private:
		static std::vector<AdapterData> _adapters;
	};
} }