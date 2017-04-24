// Boby Thomas Pazheparampil
// 24-02-1981
// add.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
#include "add.h"

#if defined(_MSC_VER)
	bool APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
				 )
#elif defined(__GNUC__)
	 bool DllMain( void * hModule, 
                       unsigned long  ul_reason_for_call, 
                       void *lpReserved
				 )

#endif // GNU compiler

{
    switch (ul_reason_for_call)
	{
		case 1://DLL_PROCESS_ATTACH:
		case 2:// DLL_THREAD_ATTACH:
		case 3://DLL_THREAD_DETACH:
		case 0://DLL_PROCESS_DETACH:
			break;
    }
    return true;
}


// This is an example of an exported function.
extern "C" ADD_API int fnAdd(int a, int b)
{
	return (a+b);
}
   

// see add.h for the class definition
CAdd::CAdd()
{ 
	return; 
}

