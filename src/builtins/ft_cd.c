#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


char *ft_strjoin(char const *s1, char const *s2)
{
    char    *str;
    size_t  len;
    size_t  i;

    len = strlen(s1) + strlen(s2) + 1;
    str = (char *)malloc(sizeof(char) * len);
    if (!s1 || !s2 || !str)
        return (NULL);
    i = 0;
    while(*s1)
        str[i++] = *s1++;
    while (*s2)
        str[i++] = *s2++;
    str[i] = 0;
    return (str);
}

void    ft_cd(int argc, char **argv, char **env)
{
    int i;
    char    *path;
    char    *new_path;
    char    *oldpwd;
    char    *home;
    int check_join;

    printf("hi ft_cd\n");
    i = 0;
    path = getcwd(NULL, 0); // path actual
    oldpwd = getenv("OLDPWD"); //path anterior "OLDPWD"
    home = getenv("HOME"); //path anterior "OLDPWD"
    if (argc == 2) // Si solo tenemos cd vamos a HOME
        printf("%s\n", home);
    else if (argc == 3 && !strcmp(argv[2],"-")) // Si escribirmos cd - nos vamos al anterior path
        printf("%s\n", oldpwd);
    else if (argc == 3) // Creamos el nuevo path
    {
        new_path = ft_strjoin(path, argv[2]);
        printf("%s\n", new_path);
    }
    /* if (chdir(path)) // Intentamos cambiar de directorio
        printf("Error\n");
    else
        printf("OK\n") // nos vamos a la nueva dirección */
   free(path);
   free(new_path);
}

int main(int argc, char **argv, char **env)
{
    if (argc > 3)
        return(1); // KO
    ft_cd(argc, argv, env);
    printf("OK\n");
    return(0);
}