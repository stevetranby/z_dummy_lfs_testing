#include <dlfcn.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *name = "libc.dylib";
    if (argc > 1)
        name = argv[1];
    void *dlh = dlopen(name, 0);
    if (dlh == 0)
        printf("Failed to dlopen() %s\n", name);
    else
        printf("Got handle %p from dlopen() for %s\n", dlh, name);
    dlclose(dlh);
    return 0;
}

