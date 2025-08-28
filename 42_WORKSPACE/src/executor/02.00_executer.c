/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.00_executer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:36 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/26 21:12:10 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static pid_t	exec_single_cmd(t_cmd *cmd, int *prevfd,
					int *pipefd, t_shell *shell);
static void		child_process(t_cmd *cmd, int prevfd,
					int pipefd[2], t_shell *shell);
static void		parent_process(t_shell *shell, int *prevfd, int pipefd[2]);
static void		wait_all_processes(pid_t *pids, t_shell *shell);

void	exec_commands(t_shell *shell)
{
	int		pipefd[2];
	int		prevfd;
	t_cmd	*cmd;
	pid_t	pids[MAX_PID];
	int		i;

	i = 0;
	cmd = shell->commands_list;
	prevfd = -1;
	if (cmd->is_btn && !cmd->next && (prevfd == -1))
	{
		exec_builtins(shell, cmd, prevfd);
		return ;
	}
	if (cmd->exit_status == ERROR && !cmd->next)
		return ;
	while (cmd)
	{
		pids[i] = exec_single_cmd(cmd, &prevfd, pipefd, shell);
		cmd = cmd->next;
		i++;
	}
	wait_all_processes(pids, shell);
}

static pid_t	exec_single_cmd(t_cmd *cmd, int *prevfd,
			int *pipefd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("Error creating pipe\n");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error creating child process.\n");
		exit(1);
	}
	if (pid == 0)
		child_process(cmd, *prevfd, pipefd, shell);
	else
	{
		parent_process(shell, prevfd, pipefd);
		if (cmd->is_btn && !ft_strncmp(cmd->args[0], "exit", 5)
			&& (*prevfd == -1))
			exit(shell->exit_status);
	}
	return (pid);
}

static void	child_process(t_cmd *cmd, int prev_fd,
			int pipe_fd[2], t_shell *shell)
{
	if (redirections(shell, cmd) == 1)
		exit (1);
	if (prev_fd != -1 && !cmd->infile) // los errores de redireccionamiento de priorizar entre | y < ya está,  cuando hay un nombre en infile no modificas o duplicas el STDINPUT y asi prioriza en el redireccionamiento.
		dup2(prev_fd, STDIN_FILENO);
	if (cmd->next && !cmd->outfile)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (cmd->is_btn)
	{
		exec_builtins(shell, cmd, prev_fd);
		exit(shell->exit_status);
	}
	else
		execute_command(shell, cmd);
	ft_putendl_fd("Error child process", STDERR_FILENO);
	//perror("Error executing\n");
	exit (126);
}

static void	parent_process(t_shell *shell, int *prev_fd, int pipe_fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (shell->commands_list->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
	else
		*prev_fd = -1;
	g_signal_flag = 1;
	signal(SIGQUIT, ft_handle_sig_quit); // MANEJAR CORRECTAMENTE EL CONTROL SLASH
}

static void	wait_all_processes(pid_t *pids, t_shell *shell)
{
	int		status;
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = shell->commands_list;
	while (cmd)
	{
		waitpid(pids[i], &status, 0);
		i++;
		if (!cmd->next) //chequeamos el exitstatus
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status) % 256;
			else if (WIFSIGNALED(status))
				shell->exit_status = (128 + WTERMSIG(status)) % 256;
		}
		cmd = cmd->next;
	}
}
