#ifndef __REFLECTION_PROPERTYINFO_H__
#define __REFLECTION_PROPERTYINFO_H__

#include <string>

#include "Type/TypeCast.h"

namespace Reflection
{
	/**
	 * @class	PropertyInfo
	 * @brief	Class that holds information about a property in the reflection system.
	 * 			Provides functionality to get and set property values and access metadata.
	 */
	class PropertyInfo
	{
		public :
			/**
			 * @brief	Template struct for initializing PropertyInfo instances.
			 * @tparam	Type   The owner type of the property.
			 * @tparam	Property The property type.
			 */
			template<typename Type, typename Property>
			struct Initializer
			{
				TypeInfo* ownerType;
				TypeInfo* propertyType;
				const size_t propertyOffset;

				Initializer(const size_t offset)
					: ownerType(TypeManager::GetHandle().Register<Type>())
					, propertyType(TypeManager::GetHandle().Register<Property>())
					, propertyOffset(offset)
				{}
			};

			/**
			 * @brief	Function pointer type for assigning property values via Type Erasure.
			 * @details	This function pointer is essentially a "Thunk". It is generated via a lambda 
			 *			in the constructor where the template types are known. It performs a safe 
			*			C++ assignment (operator=) internally after casting the void pointers.
			* @param	instance	The raw address (void*) of the destination instance.
			* @param	value		The raw address (const void*) of the source value to copy from.
			*/
			using Assigner = void(*)(void* instance, const void* value);

		public :
			/**
			 * @brief	Constructor for PropertyInfo.
			 * @tparam	Type     The owner type of the property.
			 * @tparam	Property The property type.
			 * @param	initializer The initializer containing property metadata.
			 * @param	propertyName The name of the property.
			 */
			template<typename Type, typename Property>
			explicit PropertyInfo(const Initializer<Type, Property>& initializer, const std::string& propertyName)
				: m_ownerType(initializer.ownerType)
				, m_propertyType(initializer.propertyType)
				, m_propertyOffset(initializer.propertyOffset)
				, m_propertyName(propertyName)
				, m_propertyAssigner(nullptr)
			{
				if (nullptr != initializer.ownerType)
				{
					initializer.ownerType->AddProperty(this);
				}

				m_propertyAssigner = [](void* dest, const void* src)
				{
					Property& property = *reinterpret_cast<Property*>(dest);
					property = *reinterpret_cast<const Property*>(src);
				};
			}

			bool operator==(const PropertyInfo& other) const;

		public :
			/**
			 * @brief	Gets the value of the property from the given instance.
			 * @tparam	T The expected type of the property value.
			 * @tparam	U The type of the instance containing the property.
			 * @param	instance The instance from which to get the property value.
			 * @return	Utils::RemovePointer_t<T>* A pointer to the property value if
			 */
			template<typename T, typename U>
			Utils::RemovePointer_t<T>* Get(U& instance) const
			{
				return const_cast<Utils::RemovePointer_t<T>*>(GetImpl<T>(instance));
			}

			/**
			 * @brief	Gets the value of the property from the given instance.
			 * @tparam	T The expected type of the property value.
			 * @tparam	U The type of the instance containing the property.
			 * @param	instance The instance from which to get the property value.
			 * @return	const Utils::RemovePointer_t<T>* A pointer to the property value if
			 */
			template<typename T, typename U>
			const Utils::RemovePointer_t<T>* Get(const U& instance) const
			{
				return GetImpl<T>(instance);
			}

			/**
			 * @brief	Sets the value of the property on the given instance.
			 * @tparam	T The type of the value to set.
			 * @tparam	U The type of the instance containing the property.
			 * @param	instance The instance on which to set the property value.
			 * @param	value The value to set to the property.
			 */
			template<typename T, typename U>
			void Set(U& instance, const T& value) const
			{
				const TypeInfo* inputType = TypeManager::GetHandle().GetTypeInfo<T>();

				if constexpr (Utils::IsPointer<T>::value)
				{
					if (!IsSame(m_propertyType, inputType) && !IsChild(m_propertyType->GetPureType(), inputType->GetPureType()))
					{
						return;
					}
				}
				else
				{
					if (!IsSame(m_propertyType, inputType))
					{
						return;
					}
				}

				void* rawInstance = reinterpret_cast<void*>(&instance);
				const void* rawValue = reinterpret_cast<const void*>(&value);

				SetRaw(rawInstance, rawValue);
			}

		public :
			/**
			 * @brief	Gets the raw pointer to the property value from the given instance.
			 * @details [Unsafe API] This method just calculates the raw address of the property within the instance's address space.
			 * 			So, it does not perform any type checking or safety validation.
			 * 			The caller must ensure that the instance is of the correct type and that
			 * 			the property is accessed safely.
			 * @param	instance The instance from which to get the property value.
			 * @return	void* The raw pointer to the property value.
			 */
			void* GetRaw(void* instance) const
			{
				char* base = reinterpret_cast<char*>(instance);
				char* address = base + m_propertyOffset;

				return reinterpret_cast<void*>(address);
			}

			const void* GetRaw(const void* instance) const
			{
				const char* base = reinterpret_cast<const char*>(instance);
				const char* address = base + m_propertyOffset;

				return reinterpret_cast<const void*>(address);
			}

			/**
			 * @brief	Sets the property value on the given instance using raw pointers.
			 * @details	[Unsafe API] This method directly invokes the internal Assigner thunk.
			 * It blindly trusts that the passed 'instance' and 'value' pointers point to 
			 * memory locations compatible with the types defined during property registration.
			 * * CAUTION: No type checking is performed at runtime in this method for performance.
			 * Passing incorrect pointers will lead to Undefined Behavior or Crashes.
			 * This is intended for use by low-level systems like Garbage Collectors or Serializers.
			 * @param	instance The raw address of the instance.
			 * @param	value    The raw address of the value to assign.
			 */
			void SetRaw(void* instance, const void* value) const
			{
				char* base = reinterpret_cast<char*>(instance);
				char* address = base + m_propertyOffset;
				
				if (nullptr != m_propertyAssigner)
				{
					m_propertyAssigner(reinterpret_cast<void*>(address), value);
				}
			}

		private :
			/**
			 * @brief	Implementation of Get method to retrieve property value.
			 * @tparam	T The expected type of the property value.
			 * @tparam	U The type of the instance containing the property.
			 * @param	instance The instance from which to get the property value.
			 * @return	const Utils::RemovePointer_t<T>* A pointer to the property value
			 * 			if the type matches; otherwise, nullptr.
			 */
			template<typename T, typename U>
			const Utils::RemovePointer_t<T>* GetImpl(const U& instance) const
			{
				const TypeInfo* outputType = TypeManager::GetHandle().GetTypeInfo<T>();
				const char* base = reinterpret_cast<const char*>(&instance);
				const char* address = base + m_propertyOffset;

				if constexpr (Utils::IsPointer<T>::value)
				{
					if (IsSame(m_propertyType, outputType) || IsChild(outputType->GetPureType(), m_propertyType->GetPureType()))
					{
						const T pointer = *reinterpret_cast<const T*>(address);
						return pointer;
					}
				}
				else
				{
					if (IsSame(m_propertyType, outputType))
					{
						const T& property = *reinterpret_cast<const T*>(address);
						return &property;
					}
				}

				return nullptr;
			}

		public :
			const TypeInfo* GetOwnerType() const;
			const TypeInfo* GetPropertyType() const;
			const size_t GetPropertyOffset() const;
			const std::string& GetPropertyName() const;

		private :
			const TypeInfo* m_ownerType;
			const TypeInfo* m_propertyType;
			const size_t m_propertyOffset;
			const std::string m_propertyName;
			Assigner m_propertyAssigner;
	};
};

#endif // __REFLECTION_PROPERTYINFO_H__