/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:34:02 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:26:04 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

void	exec_builtins(t_shell *shell, t_cmd *cmd, int prev_fd)
{
	if (!cmd->args[0])
		cmd->args[0] = ft_strdup("");
	if (!ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1))
		shell->exit_status = exec_echo(cmd);
	else if (!ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1))
		shell->exit_status = exec_cd(cmd, shell);
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
