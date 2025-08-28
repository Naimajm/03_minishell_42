/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_exec_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:32:43 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:18:07 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->environment[i])
	{
		if (ft_strncmp(shell->environment[i], "_=", 2) == 0)
		{
			free(shell->environment[i]);
			shell->environment[i] = ft_strdup("_=/usr/bin/env");
			if (!shell->environment[i])
				return (ERROR);
		}
		i++;
	}
	i = 0;
	while (shell->environment[i])
	{
		if (ft_strchr(shell->environment[i], '='))
			printf("%s\n", shell->environment[i]);
		i++;
	}
	return (SUCCESS);
}
