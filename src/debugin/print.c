


#include "../../inc/minishell.h"

void print_tockens(t_msl *msl)
{
    t_tocken *tocken;

    printf("El numero de tockens creados es %d\n", msl->total_tockens);
    tocken = msl->tocken;
    if (tocken == NULL && msl->total_tockens != 0)
        printf("no se han creado lo tockens como deberia \n");
    else if (tocken != NULL)
        print_tockens_info(tocken);
}

void print_pcmds(t_pcmds *pcmds)
{
    while (pcmds)
    {
        printf("  pcmd: %s\n", pcmds->cmd);
        pcmds = pcmds->next;
    }
}

void print_files(t_files *files)
{
    while (files)
    {
        printf("  file: %s (type: %d)\n", files->file_name, files->type);
        files = files->next;
    }
}

void print_tockens_info(t_tocken *tockens)
{
    while (tockens)
    {
        printf("Tocken pos: %d\n", tockens->index);
        print_pcmds(tockens->pcmds);
        print_files(tockens->files);
        // Imprime otros campos si los tienes
        tockens = tockens->next;
    }
}

void print_own_env(t_env *env)
{
    while (env)
    {
        printf("id: %s, value: %s\n", env->id, env->value);
        env = env->next;
    }
}

void print_msl(t_msl *msl)
{
    if (!msl)
    {
        printf("msl is NULL\n");
        return;
    }
    // printf("own_env:\n");
    // print_env_list(msl->own_env);
    // printf("states: %d\n", msl->states);
    printf("exit_status: %d\n", msl->exit_status);
    printf("clean_line: %s\n", msl->clean_line ? msl->clean_line : "(null)");
    printf("msl_pid: %d\n", msl->msl_pid);
    printf("total_tockens: %d\n", msl->total_tockens);
    printf("tockens:\n");
    print_tockens_info(msl->tocken);
}

