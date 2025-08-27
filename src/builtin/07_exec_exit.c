/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_exec_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:32:48 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 18:11:37 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

// (sin opciones segun el subject)
// terminar la ejecuición de la shell actual, 
// sale del shel con el estado de salida del ultimo comando ejecutado.
// No tiene opciones como otros comandos de bash, pero puede aceptar un argumento numérico que representa el código de salida.

/* Para implementar esto en tu minishell, necesitarás:

Una función que simplemente termine la ejecución del programa
Usar la variable global o estructura donde guardas el estado del último comando ejecutado */

void		exec_exit(t_shell *shell, t_cmd *cmd, int prev_fd);
static int	ft_valid_arg_exit(char *arg);

void	exec_exit(t_shell *shell, t_cmd *cmd, int prev_fd)
{
	int	exit_status;

	if (cmd->args[1])
	{
		if (!ft_valid_arg_exit(cmd->args[1]))
		{
			ft_putendl_fd(" numeric argument required", STDERR_FILENO);
			cleanup_minishell(shell);
			exit(SYNTAX_ERROR);
		}
		else if (cmd->args[2])
		{
			ft_putendl_fd(" too many arguments", STDERR_FILENO);
			shell->last_exit_status = ERROR;
			shell->exit_status = ERROR;
			return ;
		}
		else
			shell->last_exit_status = (ft_atoi(cmd->args[1]) % 256); // En Unix/Linux, los códigos de salida están limitados a 8 bits (0-255).  esto asegura que cualquier número negativo usado como estado de salida se convierta en un número positivo entre 0 y 255, que es el rango válido para los estados de salida en sistemas Unix. Si el número es mayor a 255, se hace un "wrap-around"
	}
	if (prev_fd == -1) // mostrar exit solo si no es parte de un pipe, es decir es el ultimo comando
		ft_putendl_fd("exit", STDOUT_FILENO);
	exit_status = shell->last_exit_status;
	cleanup_minishell(shell); // exit(shell->last_exit_status); error  en Valgrind es un problema clásico de "uso después de liberación" (use-after-free)
	exit(exit_status); // salir con el estado del ultimo comando almacenado en la variable, despues de liberar la shell
}

static int	ft_valid_arg_exit(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[i] == '+' || arg[i] == '-') // manejar el signo mas o menos al principio
		i++;
	if (!arg[i]) // si solo hay un signo no es válido
		return (0);
	while (arg[i]) // verificamos que los siguientes caracteres sean numeros
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

/* comportamiento de exit en bash:

Sin argumentos: sale con el último estado de salida
Un argumento numérico: sale con ese número (módulo 256)
Argumento no numérico: error de sintaxis
Múltiples argumentos: error y no sale
En pipes: no muestra "exit" excepto en el último comando
Limpieza: libera memoria antes de salir */