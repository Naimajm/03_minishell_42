/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_word_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:14:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/25 20:06:50 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	insert_token_node(t_word *word);

void	generate_processed_word(t_word *words_list, t_shell *shell)
{
	t_word	*current_word;

	if (!words_list || !shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_word = (t_word *) words_list;
	while (current_word)
	{
		if (insert_token_node(current_word) == FAILURE)
			shell->exit_status = ERROR;
		current_word = current_word->next;
	}
}

static int	insert_token_node(t_word *word)
{
	t_token	*current_token;
	char	*result;
	char	*temp;

	if (!word || !word->tokens_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	result = ft_strdup("");
	if (!result)
		return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO), FAILURE);
	current_token = (t_token *) word->tokens_list;
	while (current_token)
	{
		if (current_token->noquotes_token)
		{
			temp = ft_strjoin_free(result, current_token->noquotes_token);
			if (!temp)
				return (ft_putendl_fd(ERR_MEM_ALLOC, STDERR_FILENO),
					FAILURE);
			result = temp;
		}
		current_token = current_token->next;
	}
	word->processed_word = result;
	return (SUCCESS);
}
