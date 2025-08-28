/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils_builtins_01.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:32:33 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:04:39 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		ft_strcmp(char *s1, char *s2);
char	*ft_find_plus_pos(char *var);
void	sort_alphabetic_mtrx(char **mtrx);
char	*ft_get_keyvar(char *var);

int	ft_strcmp( char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_find_plus_pos(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (var + i);
		i++;
	}
	return (NULL);
}

void	sort_alphabetic_mtrx(char **mtrx)
{
	int	i;
	int	j;

	i = 0;
	while (mtrx[i])
	{
		j = 0;
		while (mtrx[j + 1])
		{
			if (ft_strcmp(mtrx[j], mtrx[j + 1]) > 0)
				ft_swap_mtrx(&mtrx[j], &mtrx[j + 1]);
			j++;
		}
		i++;
	}
}

char	*ft_get_keyvar(char *var)
{
	int		i;
	char	*key;

	i = 0;
	key = ft_strdup(var);
	if (!key)
		return (NULL);
	while (key[i] && key[i] != '+' && key[i] != '=')
		i++;
	key[i] = '\0';
	return (key);
}
