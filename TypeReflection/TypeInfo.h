#ifndef __TYPE_INFO_H__
#define __TYPE_INFO_H__

#include <string>

namespace MetaData
{
	class TypeInfo
	{
		public :
			TypeInfo() = delete;
			explicit TypeInfo(const std::string& _strName, const TypeInfo* _pSuper = nullptr);
			TypeInfo(const TypeInfo& _rhs);
			TypeInfo(TypeInfo&& _rhs);

			bool operator==(const TypeInfo& _rhs) const;

		public :
			void SetSuper(const TypeInfo*  _pSuper);

			const TypeInfo* GetSuper() const;
			const std::string& GetTypeName() const;

		private :
			const std::string m_strTypeName;
			const TypeInfo* m_pSuper;
	};
};

#endif // __TYPE_INFO_H__
