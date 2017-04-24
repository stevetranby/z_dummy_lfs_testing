// Dynamic Library Loading
////////////////////////////////////////////////////////////////
//
// DLL Handling
//
//

#define GB_DEF static
// #define GB_DEF extern

typedef void *tr_DllHandle;
typedef void (*tr_DllProc)(void);

GB_DEF tr_DllHandle tr_dll_load        (char const *filepath);
GB_DEF void        	tr_dll_unload      (tr_DllHandle dll);
GB_DEF tr_DllProc   tr_dll_proc_address(tr_DllHandle dll, char const *proc_name);

#if defined(_MSC_VER)

	#include <windows.h>

	gbDllHandle gb_dll_load(char const *filepath) {
		return cast(gbDllHandle)LoadLibraryA(filepath);
	}
	static void gb_dll_unload (gbDllHandle dll) { FreeLibrary(cast(HMODULE)dll); }
	gb_inline gbDllProc gb_dll_proc_address(gbDllHandle dll, char const *proc_name) { return cast(gbDllProc)GetProcAddress(cast(HMODULE)dll, proc_name); }

#endif // Microsoft compiler

#if defined(__GNUC__)

	#include <dlfcn.h>

	gbDllHandle gb_dll_load(char const *filepath) {
		// TODO(bill): Should this be RTLD_LOCAL?
		return cast(gbDllHandle)dlopen(filepath, RTLD_LAZY|RTLD_GLOBAL);
	}

	gb_inline void gb_dll_unload (gbDllHandle dll)                        { dlclose(dll); }
	gb_inline gbDllProc gb_dll_proc_address(gbDllHandle dll, char const *proc_name) { return cast(gbDllProc)dlsym(dll, proc_name); }

#endif // GNU compiler

/*

  #if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
/ Boby Thomas(RBIN/ECB-T1)
// 24-02-1981
// add.h
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
*/

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ADD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ADD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//#ifdef ADD_EXPORTS
//#define ADD_API __declspec(dllexport)
//#else
//#define ADD_API __declspec(dllimport)
//#endif

#if defined(_MSC_VER)
#	define ADD_API __declspec(dllexport)
#else
#	define ADD_API
#endif

// This class is exported from the add.dll
class CAdd {
public:
	CAdd(void);
	// TODO: add your methods here.
};


extern "C" ADD_API int fnAdd(int, int);

