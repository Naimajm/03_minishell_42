/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils_builtins_03.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:41:46 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:27:20 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char		**ft_append_env_var_value(char **env, char *var, int index);
static char	*ft_create_appended_var(char *new_val, char *old_val,
				char *plus_pos);

char	**ft_append_env_var_value(char **env, char *var, int index)
{
	char	*plus_pos;
	char	*old_value;
	char	*equal_pos;

	plus_pos = ft_find_plus_pos(var);
	if (!plus_pos)
		return (env);
	equal_pos = ft_strchr(env[index], '=');
	if (!equal_pos)
		old_value = ft_strdup("");
	else
		old_value = ft_strdup(equal_pos + 1);
	free(env[index]);
	env[index] = ft_create_appended_var(var, old_value, plus_pos);
	free (old_value);
	return (env);
}

static char	*ft_create_appended_var(char *var, char *old_val, char *plus_pos)
{
	char	*res;
	char	*temp;
	int		key_len;
	int		i;

	key_len = plus_pos - var;
	temp = ft_strjoin(old_val, plus_pos + 2);
	res = (char *)malloc(sizeof(char) * (key_len + 1 + ft_strlen(temp) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < key_len)
	{
		res[i] = var[i];
		i++;
	}
	res[key_len] = '=';
	ft_strlcpy(res + key_len + 1, temp, ft_strlen(temp) + 1);
	free (temp);
	return (res);
}
