/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_exec_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:32:48 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:18:42 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
			shell->last_exit_status = (ft_atoi(cmd->args[1]) % 256);
	}
	if (prev_fd == -1)
		ft_putendl_fd("exit", STDOUT_FILENO);
	exit_status = shell->last_exit_status;
	cleanup_minishell(shell);
	exit(exit_status);
}

static int	ft_valid_arg_exit(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}
