#ifndef __REFLECTION_CONTAINERPROPERTYINFO_H__
#define __REFLECTION_CONTAINERPROPERTYINFO_H__

#include "Property/PropertyInfo.h"

namespace Reflection
{
	class ContainerPropertyInfo : public PropertyInfo
	{
		GENERATE(ContainerPropertyInfo);
	public :
		class Iterator;

		using BeginFunc = void(*)(const void* instance, Iterator& iterator);
		using EndFunc = void(*)(const void* instance, Iterator& iterator);
		using NextFunc = void(*)(Iterator& iterator);
		using PrevFunc = void(*)(Iterator& iterator);
		using DestroyFunc = void(*)(Iterator& iterator);
		using ElementFunc = void* (*)(const Iterator& iterator);
		using CopyFunc = void(*)(Iterator& lhs, const Iterator& rhs);
		using CompareFunc = bool(*)(const Iterator& lhs, const Iterator& rhs);

		class Iterator
		{
		public:
			Iterator()
				: m_storage{}
				, m_nextFunc(nullptr)
				, m_prevFunc(nullptr)
				, m_destroyFunc(nullptr)
				, m_elementFunc(nullptr)
				, m_copyFunc(nullptr)
				, m_compareFunc(nullptr)
			{
			}

			~Iterator()
			{
				if (nullptr != m_destroyFunc)
				{
					m_destroyFunc(*this);
				}
			}

			Iterator(const Iterator& other)
				: m_storage{}
				, m_nextFunc(other.m_nextFunc)
				, m_prevFunc(other.m_prevFunc)
				, m_destroyFunc(other.m_destroyFunc)
				, m_elementFunc(other.m_elementFunc)
				, m_copyFunc(other.m_copyFunc)
				, m_compareFunc(other.m_compareFunc)
			{
				if (nullptr != m_copyFunc)
				{
					m_copyFunc(*this, other);
				}
			}

			Iterator& operator=(const Iterator& other)
			{
				if (this != &other)
				{
					if (nullptr != m_destroyFunc)
					{
						m_destroyFunc(*this);
					}

					m_nextFunc = other.m_nextFunc;
					m_prevFunc = other.m_prevFunc;
					m_destroyFunc = other.m_destroyFunc;
					m_elementFunc = other.m_elementFunc;
					m_copyFunc = other.m_copyFunc;
					m_compareFunc = other.m_compareFunc;

					if (nullptr != m_copyFunc)
					{
						m_copyFunc(*this, other);
					}
				}

				return *this;
			}

			Iterator& operator++()
			{
				if (nullptr != m_nextFunc)
				{
					m_nextFunc(*this);
				}

				return *this;
			}

			Iterator& operator--()
			{
				if (nullptr != m_prevFunc)
				{
					m_prevFunc(*this);
				}

				return *this;
			}

			Iterator operator++(int)
			{
				Iterator itr = *this;
				++(*this);
				return itr;
			}

			Iterator operator--(int)
			{
				Iterator itr = *this;
				--(*this);
				return itr;
			}

			bool operator==(const Iterator& other) const
			{
				if (nullptr != m_compareFunc)
				{
					return m_compareFunc(*this, other);
				}
				else
				{
					return false;
				}
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			const void* get() const
			{
				return m_elementFunc(*this);
			}

		private :
			friend class ContainerPropertyInfo;

			alignas(void*) uint8_t m_storage[64];

			NextFunc m_nextFunc;
			PrevFunc m_prevFunc;
			DestroyFunc m_destroyFunc;
			ElementFunc m_elementFunc;
			CopyFunc m_copyFunc;
			CompareFunc m_compareFunc;
		};

	public :
		template<typename Type, typename Property>
		explicit ContainerPropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
			: PropertyInfo(initializer, propertyName)
		{
			using ContainerConstIterator = typename Utils::IteratorTraits<Property>::ConstIterator;

			m_nextFunc = [](Iterator& iterator)
			{
				ContainerConstIterator* rawIterator = reinterpret_cast<ContainerConstIterator*>(&iterator.m_storage[0]);
				if (nullptr != rawIterator)
				{
					(*rawIterator)++;
				}
			};

			m_prevFunc = [](Iterator& iterator)
			{
				ContainerConstIterator* rawIterator = reinterpret_cast<ContainerConstIterator*>(&iterator.m_storage[0]);
				if (nullptr != rawIterator)
				{
					(*rawIterator)--;
				}
			};

			m_destroyFunc = [](Iterator& iterator)
				{
					auto* itr = reinterpret_cast<ContainerConstIterator*>(&iterator.m_storage[0]);
					itr->~ContainerConstIterator();
				};

			m_elementFunc = [](const Iterator& iterator) -> void*
			{
				const ContainerConstIterator* rawIterator = reinterpret_cast<const ContainerConstIterator*>(&iterator.m_storage);
				if (nullptr != rawIterator)
				{
					return (void*) & (**rawIterator);
				}
				else
				{
					return nullptr;
				}
			};

			m_copyFunc = [](Iterator& lhs, const Iterator& rhs)
				{
					const ContainerConstIterator* itr = reinterpret_cast<const ContainerConstIterator*>(&rhs.m_storage);

					new (&lhs.m_storage) ContainerConstIterator(*itr);
				};

			m_compareFunc = [](const Iterator& lhs, const Iterator& rhs) -> bool
			{
				const ContainerConstIterator* lhsIterator = reinterpret_cast<const ContainerConstIterator*>(&lhs.m_storage);
				const ContainerConstIterator* rhsIterator = reinterpret_cast<const ContainerConstIterator*>(&rhs.m_storage);

				return *lhsIterator == *rhsIterator;
			};

			m_beginFunc = [](const void* container, Iterator& iterator)
			{
				const Property* property = static_cast<const Property*>(container);

				const auto itr = property->begin();
				new (&iterator.m_storage) ContainerConstIterator(itr);
			};

			m_endFunc = [](const void* container, Iterator& iterator)
			{
				const Property* property = static_cast<const Property*>(container);

				const auto itr = property->end();
				new (&iterator.m_storage) ContainerConstIterator(itr);
			};
		}

		Iterator begin(const void* container) const
		{
			if (nullptr != m_beginFunc)
			{
				Iterator itr;
				m_beginFunc(container, itr);

				itr.m_nextFunc = m_nextFunc;
				itr.m_prevFunc = m_prevFunc;
				itr.m_destroyFunc = m_destroyFunc;
				itr.m_elementFunc = m_elementFunc;
				itr.m_copyFunc = m_copyFunc;
				itr.m_compareFunc = m_compareFunc;

				return itr;
			}
			else
			{
				return Iterator();
			}
		}

		Iterator end(const void* container) const
		{
			if (nullptr != m_endFunc)
			{
				Iterator itr;
				m_endFunc(container, itr);

				itr.m_nextFunc = m_nextFunc;
				itr.m_prevFunc = m_prevFunc;
				itr.m_destroyFunc = m_destroyFunc;
				itr.m_elementFunc = m_elementFunc;
				itr.m_copyFunc = m_copyFunc;
				itr.m_compareFunc = m_compareFunc;

				return itr;
			}
			else
			{
				return Iterator();
			}
		}

	private :
		BeginFunc m_beginFunc;
		EndFunc m_endFunc;
		NextFunc m_nextFunc;
		PrevFunc m_prevFunc;
		DestroyFunc m_destroyFunc;
		ElementFunc m_elementFunc;
		CopyFunc m_copyFunc;
		CompareFunc m_compareFunc;
	};

	class ArrayPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(ArrayPropertyInfo);

	public :
		template<typename Type, typename Property>
		struct Initializer : public PropertyInfo::Initializer<Type, Property>
		{
			using ValueType = typename Utils::ValueTraits<Property>::ValueType;

			const TypeInfo* valueType;

			Initializer(const size_t offset)
				: PropertyInfo::Initializer<Type, Property>(offset)
				, valueType(TypeInfo::Get<ValueType>())
			{}
		};

	public :
		template<typename Type, typename Property>
		explicit ArrayPropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
			: ContainerPropertyInfo(initializer, propertyName)
			, m_valueType(initializer.valueType)
		{
		}

	public :
		const TypeInfo* GetValueType() const { return m_valueType; }

	private :
		const TypeInfo* m_valueType;
	};

	class SetPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(SetPropertyInfo);

	public:
		template<typename Type, typename Property>
		struct Initializer : public PropertyInfo::Initializer<Type, Property>
		{
			using ValueType = typename Utils::ValueTraits<Property>::ValueType;
			using KeyType = typename Utils::KeyTraits<Property>::KeyType;

			const TypeInfo* valueType;
			const TypeInfo* keyType;

			Initializer(const size_t offset)
				: PropertyInfo::Initializer<Type, Property>(offset)
				, valueType(TypeInfo::Get<ValueType>())
				, keyType(TypeInfo::Get<KeyType>())
			{}
		};

	public:
		template<typename Type, typename Property>
		explicit SetPropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
			: ContainerPropertyInfo(initializer, propertyName)
			, m_valueType(initializer.valueType)
			, m_keyType(initializer.keyType)
		{
		}

	public :
		const TypeInfo* GetValueType() const { return m_valueType; }
		const TypeInfo* GetKeyType() const { return m_keyType; }

	private :
		const TypeInfo* m_valueType;
		const TypeInfo* m_keyType;
	};

	class MapPropertyInfo : public ContainerPropertyInfo
	{
		GENERATE(MapPropertyInfo);

	public:
		template<typename Type, typename Property>
		struct Initializer : public PropertyInfo::Initializer<Type, Property>
		{
			using ValueType = typename Utils::ValueTraits<Property>::ValueType;
			using KeyType = typename Utils::KeyTraits<Property>::KeyType;
			using MappedType = typename Utils::MappedTraits<Property>::MappedType;

			const TypeInfo* valueType;
			const TypeInfo* keyType;
			const TypeInfo* mappedType;

			const size_t keyOffset;
			const size_t mappedOffset;

			Initializer(const size_t offset)
				: PropertyInfo::Initializer<Type, Property>(offset)
				, valueType(TypeInfo::Get<ValueType>())
				, keyType(TypeInfo::Get<KeyType>())
				, mappedType(TypeInfo::Get<MappedType>())
				, keyOffset(reinterpret_cast<size_t>(&(static_cast<ValueType*>(nullptr)->first)))
				, mappedOffset(reinterpret_cast<size_t>(&(static_cast<ValueType*>(nullptr)->second)))
			{}
		};

	public:
		template<typename Type, typename Property>
		explicit MapPropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
			: ContainerPropertyInfo(initializer, propertyName)
			, m_valueType(initializer.valueType)
			, m_keyType(initializer.keyType)
			, m_mappedType(initializer.mappedType)
			, m_keyOffset(initializer.keyOffset)
			, m_mappedOffset(initializer.mappedOffset)
		{
		}

	public :
		const TypeInfo* GetValueType() const { return m_valueType; }
		const TypeInfo* GetKeyType() const { return m_keyType; }
		const TypeInfo* GetMappedType() const { return m_mappedType; }

		const void* GetRawKey(const void* rawValue) const
		{
			const char* base = reinterpret_cast<const char*>(rawValue);
			const char* address = base + m_keyOffset;

			return reinterpret_cast<const void*>(address);
		}

		const void* GetRawMapped(const void* rawValue) const
		{
			const char* base = reinterpret_cast<const char*>(rawValue);
			const char* address = base + m_mappedOffset;

			return reinterpret_cast<const void*>(address);
		}

	private :
		const TypeInfo* m_valueType;
		const TypeInfo* m_keyType;
		const TypeInfo* m_mappedType;

		const size_t m_keyOffset;
		const size_t m_mappedOffset;
	};
};

#endif // __REFLECTION_CONTAINERPROPERTYINFO_H__