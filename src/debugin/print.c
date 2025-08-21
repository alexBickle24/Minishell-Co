


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
    printf("own_env:\n");
    print_own_env(msl->own_env);
    // printf("states: %d\n", msl->states);
    printf("exit_status: %d\n", msl->exit_status);
    printf("clean_line: %s\n", msl->clean_line ? msl->clean_line : "(null)");
    printf("msl_pid: %d\n", msl->msl_pid);
    printf("msl_last_process_pid: %d\n", msl->last_process);
    printf("total_tockens: %d\n", msl->total_tockens);
    printf("parsing info\n");
    print_parsing_info(msl->parsing_utils);
    printf("tockens:\n");
    print_tockens_info(msl->tocken);
}

void print_parsing_info(t_parsing *parsing)
{
    if (!parsing)
        return;
    printf("Parsing info:\n");
    printf("  - type: %d\n", parsing->type);
    printf("  - in_quotes: %d\n", parsing->in_quotes);
    printf("  - separators: %s\n", parsing->separators);
    printf("  - special_op: %d\n", parsing->special_op[10]);
    printf("  - start: %s\n", parsing->start);
    printf("  - end: %s\n", parsing->end);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[33]);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[49]);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[39]);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[60]);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[11]);
    printf("  - dollar_lim: %d\n", parsing->dollar_lim[61]);
    // Imprime otros campos si los tienes
}