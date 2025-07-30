#include <stdio.h>
#include <unistd.h>

int main()
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    printf("%s\n", pwd);
    return (0);
}