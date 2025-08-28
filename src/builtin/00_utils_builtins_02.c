/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils_builtins_02.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:43:59 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:05:00 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char		**ft_create_new_env(char **env, char *var);
static char	*ft_process_new_var(char *var);

char	**ft_create_new_env(char **env, char *var)
{
	char	**new_env;
	int		size_mtrx;
	int		i;

	size_mtrx = ft_mtrx_size(env);
	new_env = (char **)malloc(sizeof(char *) * (size_mtrx + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size_mtrx)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_process_new_var(var);
	new_env[i + 1] = NULL;
	return (new_env);
}

static char	*ft_process_new_var(char *var)
{
	char	*plus_pos;
	char	*res;
	int		key_len;
	int		i;

	plus_pos = ft_find_plus_pos(var);
	if (!plus_pos)
		return (ft_strdup(var));
	key_len = plus_pos - var;
	res = (char *)malloc(sizeof(char) * ft_strlen(var));
	if (!res)
		return (NULL);
	i = 0;
	while (i < key_len)
	{
		res[i] = var[i];
		i++;
	}
	res[key_len] = '=';
	ft_strlcpy(res + key_len + 1, plus_pos + 2, ft_strlen(plus_pos + 2) + 1);
	return (res);
}
