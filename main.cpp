//Boby Thomas Pazheparampil - march 2006
// plat_ind.cpp :

#include "os_call.h"

#include <iostream>

using namespace std;

typedef int (*AddFnPtr)(int,int);

int main(int argc, char* argv[])
{
	cout << "hello\n";

    AddFnPtr AddFn;
    void *hDLL;

//do not add extension. It is handled by LoadSharedLibrary.
hDLL = LoadSharedLibrary("add");

    if(hDLL == 0) {
    	cout << "error loading library\n";
        return 1;
	}

	cout << "dylib loaded\n";

	AddFn = (AddFnPtr)GetFunction(hDLL,"fnAdd");
	if(!AddFn) { cout << "fnAdd found."; }

    int iTmp = AddFn(8,5);
    cout << "8 + 3 = " << iTmp << "\n";

    FreeSharedLibrary(hDLL);

    return 0;
}



