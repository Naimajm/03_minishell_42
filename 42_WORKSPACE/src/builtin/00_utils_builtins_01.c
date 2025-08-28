/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils_builtins_01.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:32:33 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/27 10:51:09 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

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

void	sort_alphabetic_mtrx(char **mtrx) // Buble Sort, ordenamiento de burbuja. La función no devuelve nada (`void`) porque modifica directamente el array que se le pasa como argumento. La función recorre el array una y otra vez.
{
	int	i;
	int	j;

	i = 0;
	while (mtrx[i]) // contador de la longitud de la matriz que me determina el numero de veces que tengo que iterar para completar la comparacion de todos los elemetos. Este bucle controla el número de "pasadas" que se hacen sobre el array.
	{
		j = 0;
		while (mtrx[j + 1]) // para no comparar con el null del final
		{
			if (ft_strcmp(mtrx[j], mtrx[j + 1]) > 0)
				ft_swap_mtrx(&mtrx[j], &mtrx[j + 1]); // ordenarlo en su lugar (in-place). `matrix[i]` ahora apuntará a donde apuntaba `matrix[i+1]` y viceversa. **No copia los strings**, solo cambia de lugar los punteros,
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
