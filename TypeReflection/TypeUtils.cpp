#include "TypeUtils.h"

#include <string>
#include <iostream>

namespace Type
{
	namespace Utils
	{
		const std::string ConvertToType(const char* _pFuncSignature)
		{
			if (_pFuncSignature == nullptr)
			{
				return nullptr;
			}

			static const char* pSpace = " ";
			static const char* pScope = "::";
			constexpr size_t spaceLength = 1;
			constexpr size_t scopeLength = 2;

			std::string strFuncSignature = _pFuncSignature;

			{
				const size_t position = strFuncSignature.rfind(pScope);
				if (position == std::string::npos)
				{
					return nullptr;
				}

				strFuncSignature = strFuncSignature.substr(0, position);
			}

			{
				size_t position = strFuncSignature.rfind(pSpace);
				if (position == std::string::npos)
				{
					return nullptr;
				}

				position++;

				strFuncSignature = strFuncSignature.substr(position, strFuncSignature.length() - position);
			}

			return strFuncSignature;
		}
	}
}