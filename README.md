# include header files
if name1.h and name2.h are simple header files that do not include each other or any other common headers and contain only function declarations (not definitions) or class declarations (not class member definitions), you can include them in main.cc without include guards and not have a problem. The compiler is fine with seeing the same function or class declared multiple times, as long as the declarations are consistent.
For standard hearder files, they include include-guard inside with means you never need to worry about multi-including them.
For functions and structures declaration, the compiler is fine to let them appear more than one time even if they are the same.
What you really need to care about is never have one then one definitions. To prevent this, you need:
1 Only include declarations inside header file, cause they may be included inside several translation units
2 Definitions of functions, methods, and variables should be placed in .cpp file, and ensure they appears in only one .cpp file. 
<img width="738" alt="image" src="https://github.com/Yzhonghua/cppTips/assets/59692712/64a8787c-5394-48cd-a9f9-81f1c0448ac5">

If you do violate the One Definition Rule (ODR) by include definitions inside a header file and make it included by more than one .cpp file, the compile would be fine cause each translation unit become a separate object file, that's fine. Include-guard also dont find any thing because it works under the scope of one file. But the linker will detect it when trying to combine them (object files) into an exe. So you fail to "building the programm".

If you include the definition inside the header file, and include it twice inside the main.cc. Include-guard works during preprocsee and you will successfully cimpile and have a -o with only one definition instead of two. But you will still fail when linker combining them if other -o also include it. So in short dont put defitions inside header file, cause you never know who include it when project goes on. But you can definitly make sure you only place the definition inside one .cpp file.


# Template instance
When we use template to build a function or class, we will need to put the definition into the header file along with the declaration, so compiler can see them to make template instance during compile phase. C++ templates is a compile-time mechanism.
- Non-template functions
  Compiler will make every .cpp file (aka translation unit) into object file, which contains machine code and other information necessary for linking. Linker will make every function call connected to its definition (maybe many points to the same one).
- Template functions
  Compile will need to make template definition for every specific argument type, otherwise the linker will be fail to find the definition and make connection. This is called Template Instantiation. Compile will generate the function with definition for this argument type (i.e. template instance), like:
  ```
  // if we have this in header file.
  template<typename T>
  T add(T a, T b) {
      return a + b;
  }
  
  // and we have this in source file.
  add<int>(a, b);
  
  // compiler will make it into this.
  int add(int a, int b) {
    return a + b;
  }
  ```
  This also means that linker wont need to look for definition and make connection any more. But it doed get a new job: Instantiation Deduplication. The linker will ensure that only one instance of the instantiated template function is kept in the final binary.


# Break into the three stages a programm runs
- Compiler's Role
  The compiler generate ocject code from the source file, which include placeholders or symbolic references for functions, variables, and other entities that are not defined within the same translation unit. At this stage, the compiler does not deal with virtual memory addresses directly. It leaves symbols or placeholders that indicate a reference needs to be resolved to an address, but it does not specify what that address will be.
- Linker's Role
  The linker resolves these symbolic references by matching them with their definitions in other object files or libraries. When the linker resolves a reference, it updates the code or data with an address that indicates where the referenced entity will be located in the final executable or shared library. These are effectively virtual addresses.
- Virtual Memory Management
  When a program is executed, the operating system loads it into virtual memory. The virtual addresses used in the program are then translated into physical addresses by the memory management unit (MMU) of the CPU, with the operating system's help.


# Access to private members
 Normally, private members of a class cannot be accessed from outside that class. However, there's an exception in C++: within a class's member functions, you can access the private (and protected) members of another object of the same class type. 
 
 That is, In C++, private and protected members of a class are accessible from within other objects of the same class. This is part of the language specification and allows one instance of a class to access the private and protected members of another instance of the same class.


# Functon overload
  The folloing code can't compile. Compiler says: functions that differ only in their return type cannot be overloaded.
  ```
  const T &operator[](size_t i) { return __data[i]; };
  
  T &operator[](size_t i) { return __data[i]; };
  ```
  But if we change it to:
  ```
  const T &operator[](size_t i) const { return __data[i]; };
  
  T &operator[](size_t i) { return __data[i]; };
  ```
  They are good to go. The first version is __const-qualified__, which means it can be called on __const__ instances of __myvector__, and it returns a reference to a __const T__. The second version is __not const-qualified__ and can be called on __non-const__ instances of __myvector__, returning a reference to a T that can be modified.
  
  Also for the above process, compiler will help use to make the choice.
  
- const object can't call the second one.
  
- non-const onject can call the second one but only if the first one is not available, or if you explicitly call a const member function through a const reference or pointer to the object. It will prefer the second one because it allows modifications to the object.


# Inheritance summary
- If you want to enforce derived classes to implement a method, use pure virtual (=0)
- If you want to make it optional for derived classes to override a method, use virtual.
- Avoid overriding non-virtual methods.


# static
- extend lifetime.
- upgrade to a class-level variable, shared by all class instances.

  and can also be accessed without generate a class instance. (size_t minCapacity = myvector<int>::MINIMUM_CAPACITY;)
- restrict internal access.
