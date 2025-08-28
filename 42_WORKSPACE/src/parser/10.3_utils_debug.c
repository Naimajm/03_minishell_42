/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.3_utils_debug.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:13:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 14:40:46 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void		print_commands_list(t_cmd *commands_list);
static void	print_command_arguments(char **args);
static void	print_output(char **args);
void		print_config_shell(t_shell *shell);
void		print_strings_array(char **array);

void	print_commands_list(t_cmd *commands_list)
{
	t_cmd	*command;
	int		node_index;

	if (!commands_list)
		return ;
	command = (t_cmd *)(commands_list);
	node_index = 1;
	while (command)
	{
		printf("┌──────────────┐\n");
		printf("| command [%i]  |\n", node_index);
		printf("└──────────────┘\n");
		printf("\t command \t-> %s\n", command->command);
		printf("\t current -> %p // ", command);
		printf("next -> %p\n", command->next);
		printf("\t infile \t-> %s\n", command->infile);
		printf("\t outfile \t-> %s\n", command->outfile);
		printf("\t delimiter \t-> %s\n", command->delimiter);
		printf("\t append \t-> %d\n", command->append);
		printf("\t hd \t\t-> %d\n", command->hd);
		printf("\t is_btn \t-> %d\n", command->is_btn);
		printf("\t exit_status \t-> %d\n", command->exit_status);
		print_words_list(command->words_list); // IMPRESION LISTA NODOS EXPAND
		print_command_arguments(command->args);
		printf("\t ───────── \n\n");
		print_output(command->args); // IMPRESION OUTPUT
		node_index++;
		command = command->next;
	}
	printf("\n");
}

static void	print_command_arguments(char **args)
{
	int	index;

	if (!args)
	{
		printf("\t └──> args -> [ (null) ]\n"); // No es error, solo información
		return ;
	}
	index = 0;
	printf("\t └──> args -> [ ");
	while (args[index])
	{
		printf("%s ", args[index]);
		if (args[index + 1])
			printf(",");
		index++;
	}
	printf("]\n");
}

static void	print_output(char **args)
{
	int	index;

	if (!args)
	{
		printf("└──> OUTPUT -> (null)\n"); // No es error, solo información
		return ;
	}
	index = 0;
	printf("└──> OUTPUT -> (");
	while (args[index])
	{
		printf("%s", args[index]);
		if (args[index + 1])
			printf(" ");
		index++;
	}
	printf(")\n");
}

void	print_config_shell(t_shell *shell)
{
	if (!shell)
		return ;
	printf("input -> %s\n", shell->input);
	print_strings_array(shell->environment);
	printf("exit_status -> %i", shell->exit_status);
	printf("last_exit_status -> %i", shell->last_exit_status);
	print_strings_array(shell->readonly_vars);
	print_commands_list(shell->commands_list);
}

void	print_strings_array(char **array)
{
	int	index;

	if (!array)
		return ;
	index = 0;
	while (array[index])
	{
		printf("%s\n", array[index]);
		index++;
	}
}
