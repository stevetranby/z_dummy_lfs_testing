#include <dyncall.h>

int main(int argc, char** argv)
{
	double r;

	DCCallVM* vm = dcNewCallVM(4096);
	dcMode(vm, DC_CALL_C_DEFAULT);

	dcReset(vm);
	dcArgDouble(vm, 4.2373);
	r = dcCallDouble(vm, (DCpointer)&sqrt);

	dcReset(vm); // Init/flush arguments.
	dcArgDouble(vm, 5.2); // Push/bind argument(s).
	r = dcCallDouble(vm, (DCpointer)&sqrt); // Call.

	dcFree(vm);
}
