## 개발 목표
 - RTTI를 사용하지 않고, Type에 대한 정보를 처리하는 방법 익히기
 - ECS, MemoryPool 프로젝트에서 사용할 수 있는 Cast 함수 직접 구현하기
 = Engine UI 혹은 직렬, 역직렬화에 용이한 Class를 구성할 수 있는 코드 짜기

## 남은 항목
 - TypeInfo의 관리 방식 변경하기 
	-> 해당 Class 내부의 GetStaticTypeInfo에서 static 변수로 존재
	-> TypeInfoManager에서 생성, 소멸 및 관리 등을 하는 것으로 변경하기

 - Property 관련 매크로 작성하기
	-> Get, Set이 아니라 Class의 Offset을 이용해서 접근하도록 하기

 - Method 관련 매크로 작성하기
	-> 가변 Argument 들을 어떤 식으로 처리할 수 있는지 고려하기

## 진행한 항목
 - 기본적인 TypeInfo를 통해서, UE의 Reflection과 유사한 GENERATE 매크로 함수 구현
 - RTTI를 최소환으로 사용하는 Type::Cast 함수 구현
	-> static TypeInfo 간의 선 비교를 통해서 Cast 여부를 판단
	-> 그 후에도 판단이 안 될 시에는 Virtual Table을 통해서 얻은 TypeInfo를 통해서 Cast 여부 판단 (RTTI)

## Reflection 관련 항목
 - RTTI : Run Time Type Info
	-> 런타임 중에 해당 객체의 타입 정보를 가져오는 기능
	-> 주로 다운 캐스팅 시에 사용됨
	-> Virtual Table을 통해서, 현재 포인터의 실 객체의 타입 정보를 가져오기에, 오버헤드가 심함
		-> 현업에선 거의 사용 안하는 듯??

 - SFINAE : Subsitution failed is not an error
	-> 적절하지 않은 치환으로 인한 실패는 컴파일 에러가 아니라, 치환 후보에서 제외되는 것
	-> 템플릿의 T 타입이 결정된 후의 구현 스코프 내부에서 코드가 생성(Instantiation) 될 때, 객체, 함수 인자 혹은 리턴 타입 등의 치환이 실패할 때에 컴파일 에러가 발생하는 것이 아니라, 치환 후보에서 제외하는 것
	-> 동일한 템플릿 정의가 있을 경우, 다른 정의를 사용하는 것

 - RAII : Resource Acquisition is initialization
	-> 획득된 자원은 초기화 되어야 함을 의미
	-> 특정 스코프 내부에서 할당된 자원을 제대로 초기화 혹은 해제하지 못 하는 상황은 RAII가 지켜지지 못한 상황황