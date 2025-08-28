/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.2_expand_extractor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:03:39 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);

char	*extract_key(char *token, int first_index)
{
	char	*key;
	int		index;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	index = first_index;
	while (!is_space(token[index])
		&& !is_redirection(token[index])
		&& !is_pipe(token[index])
		&& !is_quote(token[index])
		&& !is_expansion_char(token[index])
		&& token[index])
		index++;
	key = ft_substr(token, first_index, (index - first_index));
	if (!key)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), NULL);
	return (key);
}

char	*get_environment_var(char **env, char *variable)
{
	char	*value;
	int		index;
	int		match;

	if (!env || !variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	value = NULL;
	index = 0;
	match = 0;
	while (env[index])
	{
		match = ft_strncmp(env[index], variable, ft_strlen(variable));
		if (!match && env[index][ft_strlen(variable)] == '=')
		{
			value = ft_strdup(&env[index][ft_strlen(variable) + 1]);
			if (!value)
				return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO),
					NULL);
			return (value);
		}
		index++;
	}
	return (NULL);
}
