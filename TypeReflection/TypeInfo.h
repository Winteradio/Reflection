#ifndef __TYPE_INFO_H__
#define __TYPE_INFO_H__

namespace MetaData
{
	class TypeInfo
	{
		public :
			TypeInfo() = delete;
			explicit TypeInfo(const char* _pName, const TypeInfo* _pSuper = nullptr);
			TypeInfo(const TypeInfo& _rhs);
			TypeInfo(TypeInfo&& _rhs);

			bool operator==(const TypeInfo& _rhs) const;

		public :
			void SetSuper(const TypeInfo*  _pSuper);

			const TypeInfo* GetSuper() const;
			const char* GetTypeName() const;

		private :
			const char* m_pTypeName;
			const TypeInfo* m_pSuper;
	};
};

#endif // __TYPE_INFO_H__
