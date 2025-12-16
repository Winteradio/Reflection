#include "Utils.h"
#include <algorithm>

namespace Reflection
{
	namespace Utils
	{
#if defined(_WIN32)
		const std::string GetName(const std::string& rawClassName)
		{
			std::string className;
			
			static const std::string first = "<";
			static const std::string last = ">(void)";
			static const std::string strClass = "class";
			static const std::string strStruct = "struct";
			
			const size_t firstPos = rawClassName.find(first) + first.size();
			const size_t lastPos = rawClassName.find(last);
			
			if (firstPos == std::string::npos || lastPos == std::string::npos)
			{
				return "";
			}
			
			className = rawClassName.substr(firstPos, lastPos - firstPos);
			
			while (true)
			{
				const size_t classPos = className.find(strClass);
				if (classPos == std::string::npos)
				{
					break;
				}
				
				className.erase(classPos, strClass.length());
			}
			
			while (true)
			{
				const size_t structPos = className.find(strStruct);
				if (structPos == std::string::npos)
				{
					break;
				}
				
				className.erase(structPos, strStruct.length());
			}
			
			return className;
		}
#else
		const std::string GetClassName(const std::string& rawClassName)
		{
			std::string className;
			
			static const std::string first = "[with T = ";
			static const std::string last = "]";
			
			const size_t firstPos = rawClassName.find(first) + first.size();
			const size_t lastPos = rawClassName.find(last);
			
			if (firstPos == std::string::npos || lastPos == std::string::npos)
			{
				return "";
			}
			
			className = rawClassName.substr(firstPos, lastPos - firstPos);
			
			return className;
		}
#endif
	}
}