#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
    free(pwd);
    return (0);
}