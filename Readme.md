-----

# C++17 Reflection System

## Project Overview

This project is a **Type Reflection** system implemented based on the C++17 standard.

At compile time, the design blueprints (Types, Member Variables, Member Functions) of objects are determined.
At runtime (when the program starts), instances of these design blueprints (`TypeInfo`, `PropertyInfo`, `MethodInfo`) are generated.
It provides functionality to access member variables (Properties) and member functions (Methods) existing within object instances at runtime.

The main goals are to minimize the use of `dynamic_cast` during runtime casting and to implement functionality for dynamically accessing instance information via string names.

## Key Features

  * **Type Information (`TypeInfo`):** Stores and manages class names, hash codes, and parent class (`SuperType`) information.
  * **Property Information (`PropertyInfo`):** Stores names, types, and memory offsets of member variables. Provides a type-safe `Get`/`Set` interface. Safely supports both `const` and `non-const` objects.
      * *(Note: Handling for `const` member variables is currently not applied.)*
  * **Container Support (`ContainerPropertyInfo`):** Enables traversal and manipulation of custom containers like `wtr::DynamicArray`, `wtr::StaticArray`, `wtr::HashSet`, and `wtr::HashMap` via a single interface. Provides a **Zero-Allocation** iterator wrapper.
  * **Method Information (`MethodInfo`):** Stores names and signature information (return type, argument types) for member functions and static functions. Provides a type-safe `Invoke` interface.
  * **Safe Casting (`Cast`):** Utilizes `TypeInfo` to provide safe up/downcasting at runtime while maximizing the avoidance of `dynamic_cast`.
  * **Auto Registration:** Uses `GENERATE`, `PROPERTY`, and `METHOD` macros to automatically register reflection information within the class definition.
  * **C++17 Utilization:** Actively uses `if constexpr`, `inline static` variables, and template metaprogramming to minimize performance overhead and ensure code clarity.

-----

## Project Structure

The project has the following directory structure:

```bash
├── CMakeLists.txt         # Main CMake build script
├── include/               # Public header files
│   ├── Type/              # Type-related headers (TypeInfo, TypeManager, TypeCast, TypeMacro)
│   ├── Property/          # Property-related headers (PropertyInfo, ContainerPropertyInfo, PropertyMacro)
│   ├── Method/            # Method-related headers (MethodInfo, MethodCall, MethodMacro)
│   ├── Utils.h            # Shared template utilities
│   └── Reflection.h       # Main header for users to include
├── src/                   # Source files (Implementation)
│   ├── Type/              # Type implementation (.cpp)
│   ├── Property/          # Property implementation (.cpp)
│   └── Method/            # Method implementation (.cpp)
├── demofile/              # Example project using the library
├── externaldemofile/      # Example project using the library (via CMake External Project)
└── cmake/                 # CMake helper scripts (Build options, Installation, etc.)
```

  * **`include/`**: Location of all public header files, organized into `Type`, `Property`, and `Method` subdirectories by function.
  * **`src`**: Location of implementation (`.cpp`) files for classes and functions defined in headers, mirroring the `include` structure.
  * **`demofile`, `externaldemofile`**: Example codes demonstrating the actual usage of the library.
  * **`cmake`**: Includes helper scripts for the CMake build system.

-----

## Usage

1.  **Include Header:** Include `#include "Reflection/Reflection.h"` to use all necessary features.

2.  **Class Registration (`GENERATE`):** Add the `GENERATE(ClassName)` macro inside the class definition (in either `public` or `private` section) that you want to reflect.

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

3.  **Property Registration (`PROPERTY`):** Add the `PROPERTY(VariableName)` macro **immediately above** the member variable declaration you want to access via reflection. Container types are automatically detected and registered.

    ```cpp
    class ObjectB : public ObjectA
    {
        GENERATE(ObjectB);

        public :
            virtual ~ObjectB() = default;

        public :
            PROPERTY(m_integerValue);
            int m_intergetValue = 0; // Basic type

            PROPERTY(m_floatValue);
            float m_floatValue = 1.0f;

            PROPERTY(m_array);
            wtr::DynamicArray<int> m_array; // Automatic container support
    };
    ```

4.  **Method Registration (`METHOD`):** Add the `METHOD(FunctionName)` macro **immediately above** the member function or static function declaration you want to invoke via reflection.
    *(Function overloading is currently difficult to support with this macro approach)*

    ```cpp
    class ObjectC : public ObjectB
    {
        GENERATE(ObjectC);

        public :
            virtual ~ObjectC() = default;

            METHOD(Print);      // Member function registration
            void Print(const std::string& message) { std::cout << message << std::endl; }

            METHOD(Add);        // Const member function registration
            int Add(int a, int b) const { return a + b; }

            METHOD(Any);        // Static function registration
            static void Any() { /*...*/ }
    };
    ```

5.  **API Usage (Iteration Example):**

    ```cpp
    void IterateContainer(IObject* obj)
    {
        const Reflection::TypeInfo* typeInfo = obj->GetTypeInfo();
        const Reflection::PropertyInfo* prop = typeInfo->GetProperty("m_array");

        // Check if it is a container property
        if (auto* arrayProp = Reflection::Cast<const Reflection::ArrayPropertyInfo*>(prop))
        {
            // Create iterator (No heap allocation)
            auto it = arrayProp->begin(obj);
            auto end = arrayProp->end(obj);

            for (; it != end; ++it)
            {
                int value = *static_cast<const int*>(it.get());
                std::cout << value << std::endl;
            }
        }
    }
    ```

-----

## Development Process & Problem Solving

During the development of this reflection system, several significant technical challenges were encountered and resolved as follows:

1.  **Need for C++17 Standard:**

      * **Problem:** The initial design planned to define and initialize `static` member variables in header files (for auto-registration of `Property`/`Method` info). However, prior to C++17, initializing `static` member variables in headers without the `inline` keyword caused linkage errors.
      * **Solution:** The project standard was **upgraded to C++17** to utilize the **`inline static` member variable** feature supported from C++17 onwards. This allowed for clean handling of reflection information definition and initialization within header files using only macros.

2.  **Learning and Applying Template Metaprogramming (TMP):**

      * **Problem:** Complex requirements involving compile-time type information manipulation, such as deducing parent class types (`SuperType`), distinguishing `const` member functions, and analyzing function signatures.
      * **Solution:**
          * **SFINAE (Substitution Failure Is Not An Error):** Learned and applied techniques using `Utils::TypeDetector` (specifically utilizing `Utils::TypeWrapper`) to branch template implementations based on the existence of specific types (e.g., `SuperType` or `ThisType`). It was crucial to understand that SFINAE considers substitution failures only within the **immediate context**.
          * **`if constexpr`:** Actively utilized C++17's `if constexpr` to branch code at compile-time based on traits of template arguments `T` and `U` (e.g., `Utils::IsPointer`, `Utils::IsSame`). This optimized performance by eliminating unnecessary runtime branches or virtual function calls.
          * **Template Specialization:** Understood the constraint that **function templates cannot be partially specialized**, and implemented `MethodCreator` by leveraging the fact that **class templates support partial specialization**.
          * **Template Parameter Rules:** Required understanding and application of subtle C++ template rules, such as the order of template parameters (Type -\> Type Pack -\> Non-Type) and limitations of non-type parameters (function pointer values are allowed, but using them for class template specialization has specific constraints).

3.  **Container Reflection and Iterator Abstraction (Container Support):**

      * **Problem:** Needed to control containers with different memory layouts, such as `wtr::DynamicArray` and `wtr::HashMap`, via a **single interface (`Iterator`)**. A typical virtual function-based iterator (`IIterator*`) causes **heap allocation (`new`)** every time, leading to performance degradation and memory fragmentation.
      * **Solution:**
          * **Type Erasure & SBO:** Implemented an `Iterator` class with a fixed-size 64-byte buffer (`alignas(void*) uint8_t m_storage[64]`) to eliminate heap allocation.
          * **Placement New & VTable:** Implemented manipulation of the actual internal STL iterator within the buffer via lambda functions (`m_nextFunc`, `m_copyFunc`).
          * **Safety Assurance:** Implemented `CopyFunc` and `DestroyFunc` to perform **Deep Copy** during iterator copy/assignment and to explicitly call destructors, thereby perfectly preventing crashes (Double Free, List Corruption) in Debug Mode.
          * **Auto Detection:** Used SFINAE (`HasIterator`, `HasKey`, etc.) in `PropertyCreator` to automatically determine the container type (`Array`/`Set`/`Map`) at compile-time and generate the appropriate `ContainerPropertyInfo`.

4.  **Property `const` Correctness:**

      * **Problem:** If a `Get` function receives a `const` object but returns a `non-const` pointer, it violates C++ `const` rules and allows for dangerous code. Merging into a single function was impossible due to differing return types required by C++ `const` rules.
      * **Solution:** **Overloaded** the `Get` function into a version accepting `const U&` (returning a const pointer) and a version accepting `U&` (returning a non-const pointer). The actual implementation logic was written in a single private `GetImpl` function (const version), and the `non-const Get` calls this `Impl` and then safely applies `const_cast` only to the return value to avoid code duplication. This is a standard C++ idiom for handling `const`.

5.  **Type Safety and Simplification in Property `Set`:**

      * **Problem:** Initially, `Get` and `Set` used the same type-checking logic (allowing inheritance relationships). However, for `Set`, assigning an `instance` caused object slicing, and assigning pointers caused issues with unintended polymorphism allowance. Specifically, `instance` assignment posed a risk of memory corruption.
      * **Solution:** For API clarity and safety, the rule for `Set` was simplified to allow only **strictly identical types (`Utils::IsSame`)**. This made API usage more predictable and fundamentally blocked all hazardous situations.

6.  **Static Generation of Method Objects (Avoiding Dynamic Allocation):**

      * **Problem:** Dynamically allocating `MethodCall` objects for each method using `new` could cause memory fragmentation and management overhead. Attempting to use `static` variables faced the issue where different functions with the same signature (e.g., `Foo()`, `Bar()`) would share the same `static` object.
      * **Solution:** Introduced a `MethodCreator<typename Func, Func func>` template struct using **non-type template parameters**. Since this struct is instantiated uniquely for each function pointer **value** (`func`), declaring a `static MethodCall` object inside the struct allows for the creation of unique `static` objects for each function pointer without `new`.

7.  **Header Circular Dependency:**

      * **Problem:** Header files such as `MethodInfo`, `MethodCall`, `Macro`, and `TypeInfo` referenced each other, causing circular inclusion issues.
      * **Solution:** Split the initial `Macro.h` into functional units (`TypeMacro.h`, `PropertyMacro.h`, `MethodMacro.h`). In each header file, instead of `#include`-ing required classes, **forward declarations** were utilized as much as possible. The headers were included only in the `.cpp` files where actual implementation was needed, breaking the circular dependency chain.

8.  **Miscellaneous:** Fixed compilation errors such as resolving `const` correctness in `Cast` function calls to `GetTypeInfo()` (changed `GetTypeInfo` in `GENERATE` macro to `const`), and fixing missing `typename` keywords when using the `GENERATE` macro inside template classes.
