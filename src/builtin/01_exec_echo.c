/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_exec_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:35:42 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/27 10:51:38 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

int			exec_echo(t_cmd *cmd);
static void	ft_print_echo(char **args);
static int	check_option_echo(char *str);
static int	stdout_to_outfile(char *outfile, int append);
// Comportamiento con señales: El comando echo debería manejar apropiadamente las señales del sistema, aunque esto generalmente se maneja a nivel del shell principal.

int	exec_echo(t_cmd *cmd)
{
	int	fd;

	if (!cmd || !cmd->args)
		return (ERROR);
	if (cmd->exit_status == 1)
		return (ERROR);
	fd = dup(STDOUT_FILENO); // copia de seguridad por si tenemos que redireccionar a un archivo, podamos volver a recuper el standar output
	if (fd == -1)
		return (ERROR);
	if (cmd->outfile) // apuntamos a un archivo de salida en vez de al standar output
	{
		if (stdout_to_outfile(cmd->outfile, cmd->append) != 0) // STDOUT ahora apunta al archivo outfile
			return (ERROR); // manejar errores de redireccion
	}
	ft_print_echo(cmd->args);// si lo anterior, el echo se mandará al archivo, si no a la salida standar. 
	// guardarlo en una variable facilita añadir mas codigos de error en un futuro
	// VIP en un shell los comandos deber retornar su estado de ejecución, 0 exito, otros valores distintos tipos de error.
	dup2(fd, STDOUT_FILENO); // restaura la salida standar al original, ya que puede haberse quedado apuntando al archivo aoutfile
	close(fd);
	return (SUCCESS); // echo siempre devuelve cero, a menos que haya errores de redireccion.
}

/* La redirección persiste porque estamos modificando el descriptor de archivo a nivel del proceso, no solo de la función. Solo se restaura cuando explícitamente lo volvemos a cambiar con el dup2(fd, STDOUT_FILENO) final. */

static int	stdout_to_outfile(char *outfile, int append)
{
	int	fd;

	if (append == 1)
		fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: echo:"); // perror para errores del sistma (open, read, dup,....)
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1) // STDOUT apunta al mismo archivo que fd.
	{
		perror("minishell: echo:");
		close(fd);
		return (1);
	}
	close(fd); // cerramos fd, pero STDOUT sigue abierto apuntando a aoutfile, incluso fuera de la función. Este cambio persiste hasta que explicitamente lo cambiemos de nuevo.
	return (0);
}

static void	ft_print_echo(char **args)
{
	int	i;
	int	flag_option;

	i = 1;
	flag_option = 0;
	if (!args[1])
	{
		printf("\n");
		return ;
	}
	while (args[i] && check_option_echo(args[i]) != 0)
	{
		flag_option = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (flag_option == 0)
		printf("\n");
	return ;
}

static int	check_option_echo(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-') // el primer caracter debe ser -
		return (0);
	i++;
	while (str[i]) // los siguientes caracteres deben ser n
	{
		if (str[i] != 'n') // la opción -n que evita imprimir un salto de línea al final.
			return (0);
		i++;
	}
	return (i > 1); // retorna un booleano 0 o 1 en función de si la expresión es verdadera o falsa. comprueba que la longitud sea mayor de dos posiciones (0 y 1, '-' y 'n'). Asegura que al menos hay un n despues de -
}
