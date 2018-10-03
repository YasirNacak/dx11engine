#pragma once

#include <string>

namespace s3d {
	namespace utility {
		class StringConverter {
		public:
			static std::wstring StringToWide(std::string str);
		};
	}
}