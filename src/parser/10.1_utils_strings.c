/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.1_utils_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:10:19 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_free(char *str1, char *str2);
void	free_matrix(char **matrix);
int		is_expansion_char(char character);
int		is_space(char character);

int	advance_index_by_length(int current_index, int length)
{
	if (length == -1)
	{
		ft_putendl_fd(ERROR_ADVANCE_INDEX, STDERR_FILENO);
		return (0);
	}
	if (length == 0)
		current_index++;
	else
		current_index += length;
	return (current_index);
}

char	*ft_strjoin_free(char *str1, char *str2)
{
	char	*result;

	if (!str1 && !str2)
		return (NULL);
	if (!str1)
		result = ft_strdup(str2);
	else if (!str2)
		result = ft_strdup(str1);
	else
		result = ft_strjoin(str1, str2);
	if (!result)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), NULL);
	if (str1)
		free(str1);
	return (result);
}

void	free_matrix(char **matrix)
{
	int	row;

	if (!matrix)
		return ;
	row = 0;
	while (matrix[row])
	{
		if (matrix[row])
		{
			free(matrix[row]);
			matrix[row] = NULL;
		}
		row++;
	}
	free(matrix);
	matrix = NULL;
}

int	is_expansion_char(char character)
{
	int	expansion;

	expansion = 0;
	if (character == '$')
		expansion = 1;
	return (expansion);
}

int	is_space(char character)
{
	int	space;

	space = 0;
	if (character == ' ' || (character >= '\t' && character <= '\r'))
		space = 1;
	return (space);
}
