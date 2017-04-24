////////////////////////////////////////////////////////////////
// File: smt.h
// Author: Steve Tranby
//
// References:
// https://github.com/gingerBill/gb/blob/master/gb.h
//
////////////////////////////////////////////////////////////////

#ifndef os_call_h
#define os_call_h

#if defined(__cplusplus)
extern "C" {
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
	#define ST_EXTERN extern "C"
#else
	#define ST_EXTERN extern
#endif

#if defined(_WIN32)
	#define GB_DLL_EXPORT GB_EXTERN __declspec(dllexport)
	#define GB_DLL_IMPORT GB_EXTERN __declspec(dllimport)
#else
	#define GB_DLL_EXPORT GB_EXTERN __attribute__((visibility("default")))
	#define GB_DLL_IMPORT GB_EXTERN
#endif

// NOTE(bill): Redefine for DLL, etc.
#ifndef GB_DEF
	#ifdef GB_STATIC
		#define GB_DEF static
	#else
		#define GB_DEF extern
	#endif
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
	#ifndef GB_ARCH_64_BIT
	#define GB_ARCH_64_BIT 1
	#endif
#else
	// NOTE(bill): I'm only supporting 32 bit and 64 bit systems
	#ifndef GB_ARCH_32_BIT
	#define GB_ARCH_32_BIT 1
	#endif
#endif

#ifndef GB_EDIAN_ORDER
#define GB_EDIAN_ORDER
	// TODO(bill): Is the a good way or is it better to test for certain compilers and macros?
	#define GB_IS_BIG_EDIAN    (!*(u8*)&(u16){1})
	#define GB_IS_LITTLE_EDIAN (!GB_IS_BIG_EDIAN)
#endif

#if defined(_WIN32) || defined(_WIN64)
	#ifndef GB_SYSTEM_WINDOWS
	#define GB_SYSTEM_WINDOWS 1
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	#ifndef GB_SYSTEM_OSX
	#define GB_SYSTEM_OSX 1
	#endif
#elif defined(__unix__)
	#ifndef GB_SYSTEM_UNIX
	#define GB_SYSTEM_UNIX 1
	#endif

	#if defined(__linux__)
		#ifndef GB_SYSTEM_LINUX
		#define GB_SYSTEM_LINUX 1
		#endif
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#ifndef GB_SYSTEM_FREEBSD
		#define GB_SYSTEM_FREEBSD 1
		#endif
	#else
		#error This UNIX operating system is not supported
	#endif
#else
	#error This operating system is not supported
#endif

#if defined(_MSC_VER)
	#define GB_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define GB_COMPILER_GCC 1
#elif defined(__clang__)
	#define GB_COMPILER_CLANG 1
#else
	#error Unknown compiler
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#ifndef GB_CPU_X86
	#define GB_CPU_X86 1
	#endif
	#ifndef GB_CACHE_LINE_SIZE
	#define GB_CACHE_LINE_SIZE 64
	#endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#ifndef GB_CPU_PPC
	#define GB_CPU_PPC 1
	#endif
	#ifndef GB_CACHE_LINE_SIZE
	#define GB_CACHE_LINE_SIZE 128
	#endif

#elif defined(__arm__)
	#ifndef GB_CPU_ARM
	#define GB_CPU_ARM 1
	#endif
	#ifndef GB_CACHE_LINE_SIZE
	#define GB_CACHE_LINE_SIZE 64
	#endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
	#ifndef GB_CPU_MIPS
	#define GB_CPU_MIPS 1
	#endif
	#ifndef GB_CACHE_LINE_SIZE
	#define GB_CACHE_LINE_SIZE 64
	#endif

#else
	#error Unknown CPU Type
#endif



#ifndef GB_STATIC_ASSERT
	#define GB_STATIC_ASSERT3(cond, msg) typedef char static_assertion_##msg[(!!(cond))*2-1]
	// NOTE(bill): Token pasting madness!!
	#define GB_STATIC_ASSERT2(cond, line) GB_STATIC_ASSERT3(cond, static_assertion_at_line_##line)
	#define GB_STATIC_ASSERT1(cond, line) GB_STATIC_ASSERT2(cond, line)
	#define GB_STATIC_ASSERT(cond)        GB_STATIC_ASSERT1(cond, __LINE__)
#endif











////////////////////////////////////////////////////////////////
// C Helpers

#define global static
#define internal static
#define local_persist static

////////////////////////////////////////////////////////////////
// Dynamic Library Loading

#define dll_export
#define dll_import

typedef void *dll_handle;
typedef void (*dll_func_ptr)(void);

dll_handle LoadDynamicLibarary(dllHandle, const char* proc_name) {}
void UnloadDynamicLibarary(dll_handle dllHandle) {}
dll_func_ptr GetFuncAddress(const char* func_name) {}

static DllHandle LoadDynamicLibrary(char const *filepath);

ST_DEF void st_dll_unload(SMTDllHandle dll);

ST_DEF st_DllProc st_dll_proc_address(smt_dll_handle dll, char const *proc_name);

// TODO: change to use SMT_COMPILER_MSVC
#if defined(_MSC_VER)

	#include <windows.h>

	SMT_INLINE SMTDllHandle LoadDynamicLibrary(char const *filepath)
	{
		return cast(SMTDllHandle) LoadLibraryA(filepath);
	}

	SMT_INLINE void UnloadDynamicLibrary(SMTDllHandle dll)
	{
		FreeLibrary(cast(HMODULE) dllHandle);
	}

	SMT_INLINE st_DllProc DllGetProcAddress(SMTDllHandle dllHandle, char const *proc_name)
	{
		return cast(gbDllProc)GetProcAddress(cast(HMODULE) dllHandle, proc_name);
	}

#elif defined(__GNUC__)

	#include <dlfcn.h>

	SMTDllHandle LoadDynamicLibrary(char const *filepath)
	{
		// TODO(steve): Should this be RTLD_LOCAL?
		return cast(SMTDllHandle) dlopen(filepath, RTLD_LAZY|RTLD_GLOBAL);
	}

	SMT_INLINE void st_dll_unload (SMTDllHandle dllHandle)
	{
		dlclose(dll);
	}

	SMT_INLINE SMTDllProc st_dll_proc_address(SMTDllHandle dllHandle, char const *proc_name)
	{
		return cast(SMTDllProc) dlsym(dllHandle, proc_name);
	}

#else
#error define your copiler
#endif


/*
#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_GLOBAL 4
*/

#if defined(_MSC_VER) // Microsoft compiler
void* LoadSharedLibrary(const char *pcDllname, int iMode = 2)
{
    std::string sDllName = pcDllname;

    sDllName += ".dll";
        return (void*)LoadLibrary(pcDllname);
    #elif defined(__GNUC__) // GNU compiler
    sDllName += ".so";
        return dlopen(sDllName.c_str(),iMode);
    #endif
}
void *GetFunction(void *Lib, const char *Fnname)
{
#if defined(_MSC_VER) // Microsoft compiler
    return (void*)GetProcAddress((HINSTANCE)Lib,Fnname);
#elif defined(__GNUC__) // GNU compiler
    return dlsym(Lib,Fnname);
#endif
}

bool FreeSharedLibrary(void *hDLL)
{
#if defined(_MSC_VER) // Microsoft compiler
    return FreeLibrary((HINSTANCE)hDLL);
#elif defined(__GNUC__) // GNU compiler
    return dlclose(hDLL);
#endif
}



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
}
#endif

#endif //os_call_h
