#pragma once

#if defined(__linux__)
    #define LF_LINUX
#elif defined(_WIN32) || defined(_WIN64)
    #define LF_WINDOWS
#elif defined(__APPLE__)
    #define LF_APPLE
#elif defined(BSD)
    #define LF_BSD
#endif // __linux__

#if defined(__clang__)
    #define LF_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define LF_COMPILER_GCC
#elif defined(_MSC_VER)
    #define LF_COMPILER_MSVC
#else
    #define LF_COMPILER_OTHER
#endif

#define SafeDelete(x)           if(x) { delete x; x = nullptr;   }
#define SafeDeleteArr(x)        if(x) { delete[] x; x = nullptr; }
#define SafeDeletePtrVector(x)  for(size_t i = 0; i < x.size(); i++) { SafeDelete(x[i]); }

#define NON_COPYABLE(Class) Class(const Class& Other) = delete; 
#define SINGLETON_INSTANCE(Class) NON_COPYABLE(Class) static Class& Instance() { static Class m_Instance; return m_Instance; }

#ifdef LF_COMPILER_MSVC
#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
#elif defined(LF_COMPILER_GCC) || defined(LF_COMPILER_CLANG)
#define FORCEINLINE inline __attribute__ ((always_inline))
#define FORCENOINLINE __attribute__(( noinline ))
#else
#define FORCEINLINE inline
#define FORCENOINLINE
#endif

#include <cstddef>
#include <cstdint>