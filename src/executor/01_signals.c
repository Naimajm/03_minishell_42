/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:28 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/26 17:20:59 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	g_signal_flag;

static void	ft_handle_sigint(int signum);
void		ft_handle_sig_quit(int signum);

void	setup_signals(void)
{
	g_signal_flag = 0;
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	ft_handle_sigint(int signum)
{
	if (g_signal_flag == 0 || g_signal_flag == (SIGINT + 128))
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal_flag == 1)
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	else if (g_signal_flag == 2)
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		exit(130);
	}
}

void	ft_handle_sig_quit(int signum)
{
	ft_putendl_fd("Quit (Core dumped)", 2);
	g_signal_flag = 128 + signum;
	exit(131);
}
