/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:34:02 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/08 18:17:40 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

void	exec_builtins(t_shell *shell, t_cmd *cmd, int prev_fd)// prev_fd sirve para verificar que no haya pipes involuctradas con prev_fd == -1, para que la ejecuciónde ciertas builtin (cd, export y exit) se ralicen en el proceso padre y no se pierdan si se hacen en el hijo
{
	if (!cmd->args[0]) // no hay comando es NULL. Ese if es una medida de seguridad, a veces llamada "cláusula de guarda"
		cmd->args[0] = ft_strdup(""); // le metemos algo para que luego no pete en las siguientes validaciones con un segfault
	// empezamos las opciones de ejecución:
	if (!ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1)) // + 1 para incluir el carácter nulo '\0' al final de la cadena yno tener errores del tipo echoo.  por seguridad para asegurarse de que la comparación incluya el carácter nulo terminador (\0) al final de la cadena. es un patrón común en C para asegurarse de que estamos comparando comandos exactos y no prefijos de otros comandos más largos.
		shell->exit_status = exec_echo(cmd);
	else if (!ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1))
		shell->exit_status = exec_cd(cmd,shell);
	else if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen("pwd")))
		shell->exit_status = exec_pwd();
	else if (!ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1))
		shell->exit_status = exec_export(cmd, shell);
	else if (!ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1))
		shell->exit_status = exec_unset(shell, cmd);
	else if (!ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1))
		shell->exit_status = exec_env(shell);
	else if (!ft_strncmp(cmd->args[0], "exit", ft_strlen("exit") + 1))
		exec_exit(shell, cmd, prev_fd);
	else
		return ;
}
