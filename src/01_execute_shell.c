/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_execute_shell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/27 11:49:45 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int		read_user_input(t_shell *shell, char *prompt);
static int		process_input(t_shell *shell);
void			process_commands(t_shell *shell); //11_test_parser.c 
static void		no_input(char *input, t_shell *shell);

int	execute_shell(t_shell *shell) // TODO: EN EL OTRO REPO LA HEMOS DEJADO COMO VOID, PORQUE NO ESTAMOS VERIFICANDO EL RETORNO
{
	//int	iteration; // DEBUG CONTADOR
	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	//iteration = 0; // DEBUG CONTADOR 
	while (1)
	{
		//iteration++;
		//printf("\n\n=== Input() ITERATION %d ===\n\n", iteration);
		if (read_user_input(shell, PROMPT) == FAILURE)
			break ;
		if (process_input(shell) == FAILURE) // TODO: PASAR AL OTRO REPO. // SE ARREGLAN LOS ERRORES DE VALGRIND EN EL CASO DE FALLO DE DOBLES COMILLAS Y ESPACIO
		{
			free_iteration_input(shell);
			continue ;
		}	
		//print_config_shell(shell); // DEBUG
		free_iteration_input(shell);
		//printf("DEBUG: Memory freed, iteration %d\n", iteration); // DEBUG
	}
	free_iteration_input(shell);
	return (SUCCESS);
}

// OJO, LA FUNCION READLINE DA LEAKS DE MEMORIA, Y EL SUBJEC LOS SABE Y DICE QUE NO HAY QU E LIBERARLOS
/*solo aparecen bloques de memoria marcados como "still reachable" (aún alcanzables), lo que no se considera una pérdida real de memoria. Estos bloques son:

Principalmente de la biblioteca readline (aproximadamente 208,283 bytes en 228 bloques)
No hay pérdidas definitivas o indirectas ("definitely lost" o "indirectly lost")
No hay errores de acceso a memoria inválida

 ==7342== LEAK SUMMARY:
==7342==    definitely lost: 0 bytes in 0 blocks
==7342==    indirectly lost: 0 bytes in 0 blocks
==7342==      possibly lost: 0 bytes in 0 blocks
==7342==    still reachable: 208,283 bytes in 228 blocks
==7342==         suppressed: 0 bytes in 0 blocks */

static int	read_user_input(t_shell *shell, char *prompt)
{
	char	*input;

	shell->last_exit_status = shell->exit_status; // 1. GUARDAR estado anterior ANTES de signals	
	setup_signals();
	// DESHABILITAR AUTOCOMPLETADO DE READLINE. MEJOR LO QUITAMOS PORQUE SI NO VA A COMPLICAR EL AUTOCOMPLETATO
	// rl_bind_key('\t', rl_insert); // hacer que tab inserte un caracter tab literal
	input = readline(prompt);
	if (!input)
		return (ft_putendl_fd("exit", STDOUT_FILENO), FAILURE);
	if (g_signal_flag)
	{
		shell->exit_status = g_signal_flag;
		shell->last_exit_status = g_signal_flag; // SEÑALES (Ctrl+C = 130)
		g_signal_flag = 0; // Reset después de procesar		
	}
	else if (!g_signal_flag && shell->last_exit_status == 0) // RESET A 0 SI NO HAY SEÑALES  // PARA QUE EL TEST DE BUILTIN Y SYNTAX DE CORRECTO Y SI EL LAST EXIT ESTATUS TAMBIEN ES CERO
		shell->exit_status = SUCCESS;
	if (input[0] == '\0') // CASO INPUT VACÍO - CONTINUAR SIN PROCESAR
		return (free(input), SUCCESS);
	add_history(input); // añadir a historial si input no vacio	
	shell->input = ft_strdup(input); // PROCESAR INPUT VÁLIDO
	if (!shell->input)
	{
		no_input(input, shell);
		return (ft_putendl_fd(ERROR_INPUT_READER, STDERR_FILENO), FAILURE);
	}
	return (free(input), SUCCESS);
}

static int	process_input(t_shell *shell)
{
	if (!shell || !shell->input)
		return (FAILURE);
	if (validate_syntax(shell) == SYNTAX_ERROR)
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO), FAILURE);
	create_commands_structure(shell);
	if (validate_command_structure(shell) == SYNTAX_ERROR)
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO), FAILURE);
	process_commands(shell);
	if (validate_command_semantics(shell) == SYNTAX_ERROR)
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO), FAILURE);
	build_execution_structure(shell->commands_list, shell);
	//print_config_shell(shell);
	if (shell->commands_list)
		exec_commands(shell);
	//print_commands_list(shell->commands_list);			// Debug
	free_commands_list(&shell->commands_list);
	return (SUCCESS);
}

void	process_commands(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_command = (t_cmd *) shell->commands_list;
	while (current_command)
	{
		lexical_analyzer(current_command, shell);
		tokenizer(current_command->words_list, shell);
		variable_expander(current_command->words_list, shell);
		dequotize_tokens(current_command->words_list, shell);
		generate_processed_word(current_command->words_list, shell);
		current_command = current_command->next;
	}
}

static void	no_input(char *input, t_shell *shell)
{
	free(input);
	shell->exit_status = ERROR;
}
