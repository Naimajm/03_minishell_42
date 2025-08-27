/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:28 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/26 17:22:28 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	g_signal_flag; // Variables globales, el subject permite solo una.

static void	ft_handle_sigint(int signum);
void		ft_handle_sig_quit(int signum);

void	setup_signals(void) // comportamiento asíncrono
{
	g_signal_flag = 0; // REINICIA A MODO INTEREACTIVO
	signal(SIGINT, ft_handle_sigint); // Asocia la señal SIGINT (Ctrl+C) con la función ft_handle_sigint. A partir de ahora, cada vez que se presione Ctrl+C, se llamará a esa función. Al llamar a tu función, el sistema operativo le pasa automáticamente como argumento el número de la señal que ha ocurrido. En este caso, el valor de signum será el número entero que representa a SIGINT (que es 2 en la mayoría de los sistemas POSIX).
	signal(SIGQUIT, SIG_IGN); // SIGINT (la señal enviada al presionar Ctrl+C) y SIGQUIT (Ctrl+\). Le dice al sistema que ignore la señal SIGQUIT (Ctrl+\). Este es el comportamiento estándar de las shells interactivas; Ctrl+\ no debe terminar la shell en sí, solo el comando que se esté ejecutando en primer plano (lo cual se gestiona de forma diferente para los procesos hijos). 
}

static void	ft_handle_sigint(int signum) // SE EJECUTA AL RECIBIR UN SIGINT
{
	if (g_signal_flag == 0 || g_signal_flag == (SIGINT + 128)) // Establece g_signal_flag a 130 (128 + 2, donde 2 es el número de SIGINT).
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_on_new_line();  // READLINE.H
		rl_replace_line("", 0); // READLINE.H
		rl_redisplay(); // READLINE.H Usa funciones de readline (rl_on_new_line, rl_replace_line, rl_redisplay) para limpiar la línea de entrada actual y mostrar un nuevo prompt vacío.
	}
	else if (g_signal_flag == 1) // CUANDO UN COMANDO SE ESTÁ EJECUTANDO
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		rl_replace_line("", 0); // NO MUESTRA UN NUEVO PROMT
	}
	else if (g_signal_flag == 2) // PARA HERE-DOCUMENT
	{
		g_signal_flag = 128 + signum;
		write(1, "\n", 1);
		exit(130);
	}
}

// MANEJAR SIGQUIT
void	ft_handle_sig_quit(int signum) // SE EJECUTA AL RECIBIR UN SIGQUIT
{
	ft_putendl_fd("Quit (Core dumped)", 2);
	g_signal_flag = 128 + signum;
	exit(131);
}
