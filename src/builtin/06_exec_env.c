/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_exec_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:32:43 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/27 10:52:30 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

int	exec_env(t_shell *shell) // subject : with no options or arguments
{
	int	i;

	i = 0;
	// buscar la variable de entorno especial _ y no asumir que se va a encontra en la posicion 1. -> free(shell->environment[1]); 
	while (shell->environment[i])
	{
		if (ft_strncmp(shell->environment[i], "_=", 2) == 0)
		{
			free(shell->environment[i]); // liberamos la variable de entorno que contenía la ruta del comando anterior, ya que la variable especial _ en shell Unix almacena la ruta completa del ultimo comando ejejcutado
			shell->environment[i] = ft_strdup("_=/usr/bin/env"); // actualizamos al comando env que qes el que estamos ejecutando, para que haya coherencia con la salida de env en bash.
			if (!shell->environment[i])
				return (ERROR);
		}
		i++;
	}
	i = 0;
	while (shell->environment[i]) // mostramos las variables
	{
		if (ft_strchr(shell->environment[i], '='))
			printf("%s\n", shell->environment[i]);
		i++;
	}
	return (SUCCESS);
}
