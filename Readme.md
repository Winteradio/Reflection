# C++17 Reflection System

## 프로젝트 개요

본 프로젝트는 C++17 표준을 기반으로 구현된 **타입 리플렉션(Type Reflection)** 시스템입니다.

컴파일 타임 때, 객체의 설계 도면( 타입, 멤버 변수, 멤버 함수) 가 결정이 됩니다.  
런타임, 프로그램 시작 시 해당 객체의 설계 도면 인스턴스(`TypeInfo`, `PropertyInfo`, `MethodInfo`)들이 생성됩니다.  
런타임 때, 객체 인스턴스에 존재하는 멤버 변수(프로퍼티)와 멤버 함수(메소드)에 접근할 수 있는 기능을 제공합니다.  

주된 목표는 런타임 중, 캐스팅 시 최대한 'dynamic_cast'를 피하는 것과  
문자열 이름을 통한 인스턴스의 정보에 동적으로 접근할 수 있는 기능을 구현하는 것입니다.  

## 주요 기능

* **타입 정보 (`TypeInfo`):** 클래스의 이름, 해시 코드, 부모 클래스(`SuperType`) 정보 등을 저장 및 관리
* **프로퍼티 정보 (`PropertyInfo`):** 멤버 변수의 이름, 타입, 메모리 오프셋을 저장, 타입 안전한 `Get`/`Set` 인터페이스 제공, `const` 객체와 `non-const` 객체를 모두 안전하게 지원  
**(`const` 변수에 대한 처리 미적용 상태)**
* **메소드 정보 (`MethodInfo`):** 멤버 함수 및 정적 함수의 이름, 시그니처(반환 타입, 인자 타입) 정보 저장, 타입 안전한 `Invoke` 인터페이스를 제공
* **안전한 캐스팅 (`Cast`):** `TypeInfo`를 활용하여, `dynamic_cast`의 사용을 최대한 피하면서 런타임 중에 안전한 업/다운캐스팅 제공
* **자동 등록:** `GENERATE`, `PROPERTY`, `METHOD` 매크로를 사용하여 클래스 정의부에 리플렉션 정보 자동 등록
* **C++17 활용:** `if constexpr`, `inline static` 변수, 템플릿 메타프로그래밍 등을 적극 활용, 성능 오버헤드를 최소화 및 코드의 명확성 확보

---

## 프로젝트 구성

프로젝트는 다음과 같은 디렉토리 구조를 가집니다.

```bash
├── CMakeLists.txt         # 메인 CMake 빌드 스크립트
├── include/               # 공개 헤더 파일
│   ├── Type/          # 타입 관련 헤더 (TypeInfo, TypeManager, TypeCast, TypeMacro)
│   ├── Property/      # 프로퍼티 관련 헤더 (PropertyInfo, PropertyMacro)
│   ├── Method/        # 메소드 관련 헤더 (MethodInfo, MethodCall, MethodMacro)
│   ├── Utils.h        # 공용 템플릿 유틸리티
│   └── Reflection.h   # 사용자가 포함할 메인 헤더
├── src/                   # 소스 파일 (구현)
│   ├── Type/              # 타입 관련 구현 (.cpp)
│   ├── Property/          # 프로퍼티 관련 구현 (.cpp)
│   └── Method/            # 메소드 관련 구현 (.cpp)
├── demofile/              # 라이브러리 사용 예제 프로젝트
├── externaldemofile/	   # 라이브러리 사용 예제 프로젝트 (CMake External Project 사용)
└── cmake/                 # CMake 헬퍼 스크립트 (빌드 옵션, 설치 등)
```


* **`include/`**: 라이브러리의 모든 공개 헤더 파일 위치, 기능별로 `Type`, `Property`, `Method` 하위 폴더로 구성  
* **`src`**: 헤더 파일에 정의된 클래스 및 함수의 구현(`.cpp`) 파일 위치, `include`와 동일한 하위 폴더 구조  
* **`demofile`, `externaldemofile`**: 라이브러리 실제 사용하는 예제 코드  
* **`cmake`**: CMake 빌드 시스템을 위한 헬퍼 스크립트들이 포함  

---

## 사용법

1.  **헤더 포함:** `#include "Reflection/Reflection.h"` 를 사용하여 필요한 모든 기능을 포함  
2.  **클래스 등록 (`GENERATE`):** 리플렉션을 사용하려는 클래스 정의부 (`public` 또는 `private` 영역)에 `GENERATE(ClassName)` 매크로를 추가  

    ```cpp
    #include "Reflection/Reflection.h"

	class IObject
    {
		GENERATE(IObject);

    	public :
    		virtual ~Object() = default;
    };

    class ObjectA : public IObject
    {
    	GENERATE(ObjectA);

    	public :
    		virtual ~ObjectA() = default;
    };
    ```
3.  **프로퍼티 등록 (`PROPERTY`):** 리플렉션으로 접근하려는 멤버 변수 선언 **바로 위**에 `PROPERTY(VariableName)` 매크로 추가  

    ```cpp
    class ObjectB : public ObjectA
    {
		GENERATE(ObjectB);
	
		public :
    		virtual ~ObjectB() = default;
    
    	public :
    		PROPERTY(m_integerValue);
    		int m_intergetValue = 0; // 멤버 변수 등록

    		PROPERTY(m_floatValue);
    		float m_floatValue = 1.0f;
    };
    ```
4.  **메소드 등록 (`METHOD`):** 리플렉션으로 호출하려는 멤버 함수 또는 정적 함수 선언 **바로 위**에 `METHOD(FunctionName)` 매크로를 추가  
**(함수 오버로딩은 현재 매크로 방식으로는 지원하기 어려움)**

    ```cpp
    class ObjectC : public ObjectB
    {
    	GENERATE(ObjectC);

    	public :
    		virtual ~ObjectC() = default;

    		METHOD(Print);		// 멤버 함수 등록
    		void Print(const std::string& message) { std::cout << message << std::endl; }

    		METHOD(Add);		// const 멤버 함수 등록
    		int Add(int a, int b) const { return a + b; }

    		METHOD(Any);		// 정적 함수 등록
    		static void Any() { /*...*/ }
    };
    ```
5.  **API 사용:**

    ```cpp
	#include <LogProject/Log.h>
	#include <LogProject/LogPlatform.h>
	#include <ReflectionProject/Reflection.h>
	
	class IObject
	{
	    GENERATE(IObject);			// 타입 등록
	
	    public :
	        IObject()
	            : m_Value(-1)
	        {}
	
	        virtual ~IObject() = default;
	
	    public :
	        PROPERTY(m_Value);		// 멤버 변수 등록
	        const int m_Value;
	};
	
	class ObjectA : public IObject
	{
	    GENERATE(ObjectA);			// 타입 등록
	
	    public :
	        ObjectA()
	            : m_Value(1)
	        {}
	
	        virtual ~ObjectA() = default;
	
	    public :
	        PROPERTY(m_Value);		// 멤버 변수 등록록
	        const int m_Value;
	
	        PROPERTY(m_Other);
	        int m_Other;			// 멤버 변수 등록
	};
	
	class ObjectB : public ObjectA
	{
	    GENERATE(ObjectB);			// 타입 등록
	
	    public :
	        ObjectB()
	            : m_Value(2)
	        {};
	
	    public :
	        METHOD(Hello);			// 멤버 함수 등록
	        void Hello(const int value) 
	        {
	            LOGINFO() << "Member Hello " << value;
	        };
	
	        METHOD(ConstHello);		// const 멤버 함수 등록
	        void ConstHello(const int value1, const float value2) const 
	        {
	            LOGINFO() << "Member Const Hello " << value1 << " " << value2;
	        };
	
	        METHOD(StaticHello);	// 정적 함수 등록
	        static void StaticHello(const float value1) 
	        {
	            LOGINFO() << "Static Hello " << value1;
	        };
	
	        PROPERTY(m_Value);		// 멤버 변수 등록
	        int m_Value;
	};
	
	class ObjectHolder
	{
	    GENERATE(ObjectHolder);		// 타입 등록록
	
	public:
	    ObjectHolder()
	        : m_MyObject(new ObjectB())
	    {
	        m_MyObject->m_Value = 99;
	    }
	
	    ~ObjectHolder()
	    {
	        delete m_MyObject;
	    }
	
	public:
	    PROPERTY(m_MyObject);		// 멤버 변수 등록록
	    ObjectB* m_MyObject;
	};
	
	namespace Test
	{
	    void Print(const Reflection::PropertyInfo* propertyInfo)	// 타입 정보 출력
	    {
	        if (propertyInfo == nullptr)
	        {
	            LOGERROR() << "The property info is invalid";
	            return;
	        }
	
	        LOGINFO() << "  Property " 
	                    << " Type : " << propertyInfo->GetPropertyType()->GetTypeName()
	                    << " / Name : " << propertyInfo->GetPropertyName();
	    }
	
	    void Print(const Reflection::MethodInfo* methodInfo)	// 함수 정보 출력
	    {
	        if (methodInfo == nullptr)
	        {
	            LOGERROR() << "The method info is invalid";
	            return;
	        }
	
	        LOGINFO() << "  Method "
	                    << " Type : " << methodInfo->GetMethodType()->GetTypeName()
	                    << " / Name : " << methodInfo->GetMethodName();
	    }
	
	    void Print(const Reflection::TypeInfo* typeInfo)	// 멤버 변수 정보 출력력
	    {
	        if (typeInfo == nullptr)
	        {
	            LOGERROR() << "The typeinfo is invalid";
	            return;
	        }
	
	        if (typeInfo->GetSuperType() == nullptr)
	        {
	            LOGINFO() << "Type : " << typeInfo->GetTypeName()
	                        << " / Super : none";
	        }
	        else
	        {
	            LOGINFO() << "Type : " << typeInfo->GetTypeName()
	                        << " / Super : " << typeInfo->GetSuperType()->GetTypeName();
	        }
	
	        for (const auto& propertyPair : typeInfo->GetProperties())
	        {
	            Print(propertyPair.second);
	        }
	
	        for (const auto& methodPair : typeInfo->GetMethods())
	        {
	            Print(methodPair.second);
	        }
	    }
	
	    void ChangeInstanceProperty()
	    {
	        IObject instance;
	
	        const Reflection::PropertyInfo* propertyInfo = IObject::GetStaticTypeInfo()->GetProperty("m_Value");
	        if (propertyInfo != nullptr)
	        {
	            int* value = propertyInfo->Get<int>(instance);	// 멤버 변수 포인터 획득
	            if (value != nullptr)
	            {
	                LOGINFO() << "IObject Value : " << *value;
	            }
	
	            propertyInfo->Set(instance, 2);		// 멤버 변수 바꾸기기
	
	            LOGINFO() << "IObject Changed Value : " << instance.m_Value;
	        }
	    }
	
	    void ChangeConstantInstanceProperty()
	    {
	        const IObject constInstance;
	
	        const Reflection::PropertyInfo* propertyInfo = IObject::GetStaticTypeInfo()->GetProperty("m_Value");
	        if (propertyInfo != nullptr)
	        {
	            const int* value = propertyInfo->Get<int>(constInstance);	// const 멤버 변수 포인터 획득
	            if (value != nullptr)
	            {
	                LOGINFO() << "const IObject Value : " << *value;
	            }
	        }
	    }
	
	    void ChangePointerProperty()
	    {
	        ObjectHolder holder;
	
	        const Reflection::PropertyInfo* propertyInfo = ObjectHolder::GetStaticTypeInfo()->GetProperty("m_MyObject");
	        if (propertyInfo != nullptr)
	        {
	            ObjectB* value = propertyInfo->Get<ObjectB*>(holder);	// 멤버 변수 포인터 획득
	            if (value != nullptr)
	            {
	                LOGINFO() << "ObjectHolder ObjectB Value : " << value->m_Value;
	            }
	
	            ObjectB newValue;
	            newValue.m_Value = 150;
	
	            propertyInfo->Set(holder, &newValue);	// 멤버 변수 변경
	
	            LOGINFO() << "ObjectHolder Changed Value : " << holder.m_MyObject->m_Value;
	
	            propertyInfo->Set(holder, value);	// 멤버 변수 변경
	        }
	    }
	
	    void Print()
	    {
	        LOGINFO() << "[ Print Type Info ]";
	
	        for (const auto& typePair : Reflection::TypeManager::GetHandle().GetTypeMap())
	        {
	            Print(typePair.second);
	        }
	
	        LOGINFO() << " ";
	    }
	
	    void Property()
	    {
	        LOGINFO() << "[ Test Property ]";
	        
	        ChangeInstanceProperty();
	        ChangeConstantInstanceProperty();
	        ChangePointerProperty();
	
	        LOGINFO() << " ";
	    }
	
	    void Cast()
	    {
	        LOGINFO() << "[ Test Type Casting ]";
	
	        IObject* objectA = new ObjectA;
	        IObject* objectB = new ObjectB;
	
	        ObjectB* wrongB = Reflection::Cast<ObjectB*>(objectA);	// 잘못된 캐스팅 (ObjectA 인스턴스 -> ObjectB 인스턴스)
	        if (nullptr == wrongB)
	        {
	            LOGINFO() << "Failed to cast the ObjectA(ObjectA) to ObjectB";
	        }
	
	        ObjectB* castB = Reflection::Cast<ObjectB*>(objectB);	// 올바른 캐스팅 (ObjectB 인스턴스 -> ObjectB 인스턴스)
	        if (nullptr != castB)
	        {
	            LOGINFO() << "Succeed to cast the IObject(ObjectB) to the ObjectB : " << castB->m_Value;
	        }
	
	        ObjectA* castA = Reflection::Cast<ObjectA*>(objectB);	// 올바른 캐스팅 (ObjectA 인스턴스 -> ObjectA 인스턴스)
	        if (nullptr != castA)
	        {
	            LOGINFO() << "Succeed to cast the ObjectB(ObjectB) to the ObjectA : " << castA->m_Value;
	        }
	
	        LOGINFO() << " ";
	    }
	
	    void Invoke()
	    {
	        LOGINFO() << "[ Test Method Invoke ]";
	
	        ObjectB value;
	
	        const Reflection::TypeInfo* typeInfo = value.GetTypeInfo();
	        if (nullptr == typeInfo)
	        {
	            return;
	        }
	
	        const Reflection::MethodInfo* hello = typeInfo->GetMethod("Hello");				// 멤버 함수 포인터 획득
	        const Reflection::MethodInfo* constHello = typeInfo->GetMethod("ConstHello");	// const 멤버 함수 포인터 획득
	        const Reflection::MethodInfo* staticHello = typeInfo->GetMethod("StaticHello");	// static 함수 포인터 획득
	
	        if (nullptr != hello)
	        {
	            hello->Invoke<void>(value, 10);					// 함수 호출
	        }
	
	        if (nullptr != constHello)
	        {
	            constHello->Invoke<void>(value, 10, 0.5f);		// 함수 호출
	        }
	
	        if (nullptr != staticHello)
	        {
	            staticHello->Invoke<void>(value, 0.5f);			// 함수 호출
	        }
	
	        LOGINFO() << " ";
	    }
	};
	
	int MAIN()
	{
	    Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Time | Log::Enum::eLevel_Type);
	    LOGINFO() << "|| Reflection Test ||";
	    LOGINFO() << " ";
	
	    Test::Print();
	    Test::Cast();
	    Test::Property();
	    Test::Invoke();
	
	    system("pause");
	
	    return 0;
	}
    ```

---

## 개발 과정 및 문제 해결

이 리플렉션 시스템을 개발하면서 몇 가지 중요한 기술적 문제에 직면했고, 다음과 같이 해결했습니다.

1.  **C++17 표준 필요성:**
    * **문제:** 초기 설계는 헤더 파일에서 `static` 멤버 변수를 정의하고 초기화하는 방식을 구상했습니다 (`Property`/`Method` 정보 자동 등록). 하지만 C++17 이전 버전에서는 `static` 멤버 변수를 헤더에서 `inline` 키워드 없이 초기화할 수 없어 링크 오류가 발생했습니다.
    * **해결:** C++17 표준부터 지원되는 **`inline static` 멤버 변수** 기능을 활용하기 위해 프로젝트의 C++ 표준을 **C++17로 상향** 조정했습니다. 이를 통해 매크로만으로 헤더 파일에서 리플렉션 정보의 정의와 초기화를 깔끔하게 처리할 수 있게 되었습니다.

2.  **템플릿 메타프로그래밍(TMP) 학습 및 적용:**
    * **문제:** 부모 클래스 타입 추론(`SuperType`), `const` 멤버 함수 구분, 함수 시그니처 분석 등 컴파일 타임에 타입 정보를 다루어야 하는 복잡한 요구사항들이 있었습니다.
    * **해결:**
        * **SFINAE (Substitution Failure Is Not An Error):** `Utils::TypeDetector` 등에서 SFINAE 기법(특히 `Utils::TypeWrapper` 활용)을 사용하여 특정 타입(예: `SuperType` 또는 `ThisType`)의 존재 유무에 따라 템플릿 구현을 분기하는 방법을 학습하고 적용했습니다. SFINAE가 **즉시 컨텍스트(immediate context)** 내에서의 치환 실패만 오류로 간주하지 않는다는 점을 이해하는 것이 중요했습니다.
        * **`if constexpr`:** C++17의 `if constexpr`를 적극 활용하여, 템플릿 인자(`T`, `U`)의 특성(`Utils::IsPointer`, `Utils::IsSame` 등)에 따라 컴파일 타임에 코드를 분기했습니다. 이를 통해 불필요한 런타임 분기나 가상 함수 호출 없이 성능을 최적화할 수 있었습니다.
        * **템플릿 특수화:** **함수 템플릿은 부분 특수화가 불가능**하다는 제약을 이해하고, 클래스 템플릿의 **부분 특수화(partial specialization)**가 가능하다는 점을 이용하여 `MethodCreator`를 구현했습니다. 
        * **템플릿 매개변수 규칙:** 템플릿 매개변수의 순서 (타입 -> 타입 팩 -> 비-타입), 비-타입 매개변수의 제한 (함수 포인터 값은 가능하지만 이를 이용한 클래스 템플릿 특수화는 불가) 등 C++ 템플릿의 미묘한 규칙들을 이해하고 적용하는 과정이 필요했습니다.

3.  **Property의 `const` 정확성 문제:**
    * **문제:** `Get` 함수가 `const` 객체를 받을 때 `non-const` 포인터를 반환하면 C++의 `const` 규칙을 위반하고 위험한 코드를 작성할 수 있었습니다. 하나의 함수로 합치려 했으나, C++의 `const` 규칙 상 반환 타입이 달라 불가능했습니다.
    * **해결:** `Get` 함수를 `const U&`를 받는 버전(const 포인터 반환)과 `U&`를 받는 버전(non-const 포인터 반환)으로 **오버로딩**했습니다. 실제 구현 로직은 `private GetImpl` 함수 하나에 `const` 버전으로 작성하고, `non-const Get`는 이 `Impl`을 호출한 뒤 반환값에만 안전하게 `const_cast`를 적용하여 코드 중복을 피했습니다. 이는 C++의 표준적인 `const` 처리 방식입니다.

4.  **Property `Set` 시 타입 안전성 및 단순화:**
    * **문제:** 초기에는 `Get`와 `Set`가 동일한 타입 체크 로직(상속 관계 허용)을 사용했으나, `Set`의 경우 `instance` 대입 시 객체 잘림(slicing) 또는 포인터 대입 시 의도치 않은 다형성 허용 문제가 있었습니다. 특히 `instance` 대입은 메모리 오염 위험도 있었습니다.
    * **해결:** API의 명확성과 안전성을 위해 `Set`는 **정확히 동일한 타입(`Utils::IsSame`)**만 허용하도록 규칙을 단순화했습니다. 이로 인해 API 사용이 더 예측 가능해지고 모든 위험한 상황이 원천 차단되었습니다.

5.  **Method 객체의 `static` 생성 (동적 할당 회피):**
    * **문제:** 각 메소드 정보를 담는 `MethodCall` 객체들을 `new`로 동적 할당하면 메모리 단편화 및 관리 오버헤드가 발생할 수 있었습니다. `static` 변수를 사용하려 했으나, 시그니처가 같은 다른 함수들(e.g., `Foo()`, `Bar()`)이 동일한 `static` 객체를 공유하는 문제가 있었습니다.
    * **해결:** **비-타입 템플릿 매개변수(non-type template parameter)**를 활용한 `MethodCreator<typename Func, Func func>` 템플릿 구조체를 도입했습니다. 이 구조체는 함수 포인터 **값**(`func`)마다 고유하게 인스턴스화되므로, 구조체 내부에서 `static MethodCall` 객체를 선언하면 각 함수 포인터마다 고유한 `static` 객체를 `new` 없이 생성할 수 있게 되었습니다.

6.  **헤더 순환 참조 문제:**
    * **문제:** `MethodInfo`, `MethodCall`, `Macro`, `TypeInfo` 등의 헤더 파일들이 서로를 참조하면서 순환 포함 문제가 발생했습니다.
    * **해결:** 초기 `Macro.h`를 기능별(`TypeMacro.h`, `PropertyMacro.h`, `MethodMacro.h`)로 분리하고, 각 헤더 파일에서는 필요한 다른 클래스를 `#include`하는 대신 **전방 선언(forward declaration)**을 최대한 활용했습니다. 실제 구현이 필요한 `.cpp` 파일에서만 해당 헤더들을 포함하도록 하여 순환 고리를 끊었습니다.

8.  **기타:** `Cast` 함수 내 `GetTypeInfo()` 호출 시 `const` 문제 해결 (`GENERATE` 매크로의 `GetTypeInfo`를 `const`로 변경), 템플릿 클래스 내 `GENERATE` 매크로 사용 시 `typename` 키워드 누락 문제 해결 등 컴파일 오류들을 수정했습니다.
---
