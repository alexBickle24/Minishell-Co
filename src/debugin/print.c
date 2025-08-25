


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
	int i;

	if (!parsing)
	{
		printf("Parsing: NULL\n");
		return;
	}
	printf("=== t_parsing ===\n");
	printf("parstat: %d\n", parsing->parstat);
	printf("lexstat: %d\n", parsing->lexstat);
	printf("infstat: %d\n", parsing->infstat);

	printf("sep_op: ");
	for (i = 0; i < 128; i++)
		if (parsing->sep_op[i])
			printf("[%c]=%d ", i, parsing->sep_op[i]);
	printf("\n");

	printf("dollar_lim: ");
	for (i = 0; i < 128; i++)
		if (parsing->dollar_lim[i])
			printf("[%c]=%d ", i, parsing->dollar_lim[i]);
	printf("\n");

	printf("ptr: %p\n", (void *)parsing->ptr);

	printf("Jump table (f):\n");
	for (i = 0; i < 8; i++)
		printf("f[%d]: %p\n", i, (void *)parsing->lex[i]);
}

void print_lex(t_lex *lexer, t_parsing *parser)
{
	int i = 0;
	while (lexer)
	{
		printf("LEX[%d]: type=%d, len=%zu, str=\"%s\"\n", i, lexer->type, lexer->len, lexer->str);
		if (parser)
		{
			printf("  parser states: parstat=%d, lexstat=%d, infstat=%d\n",
				parser->parstat, parser->lexstat, parser->infstat);
		}
		lexer = lexer->next;
		i++;
	}
}
void print_parser_state(t_parsing *parser, unsigned char c, int i)
{
	printf("i=%d, char='%c' (0x%02x) | parstat=%d, lexstat=%d, infstat=%d\n",
		i, c ? c : ' ', c, parser->parstat, parser->lexstat, parser->infstat);
}