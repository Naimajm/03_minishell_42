/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:31:52 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/26 18:39:33 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int			exec_unset(t_shell *shell, t_cmd *cmd);
static char	**ft_change_env(char **env, char *str);
static int	ft_is_readonly(char **readonly_vars, char *var_name);
static void	print_unset_err(t_shell *shell, char *arg, char *mesasage);

int	exec_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!shell || !cmd || !shell->environment || !cmd->args)
		return (1);
	while (cmd->args[i])
	{
		if (!ft_valid_env_var(cmd->args[i]) || ft_strchr(cmd->args[i], '='))
			print_unset_err(shell, cmd->args[i], "': not a valid identifier");
		else if (ft_is_readonly(shell->readonly_vars, cmd->args[i]))
			print_unset_err(shell, cmd->args[i], "': readonly variable");
		else if (ft_strcmp(cmd->args[1], "HOME") != 0
			&& ft_strcmp(cmd->args[1], "PWD") != 0
			&& ft_strcmp(cmd->args[1], "OLDPWD") != 0)
		{
			shell->environment = ft_change_env(shell->environment,
					cmd->args[i]);
			if (!shell->environment)
				return (ERROR);
		}
		i++;
	}
	return (shell->exit_status);
}

static char	**ft_change_env(char **env, char *str)
{
	int		index_env;
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	index_env = ft_search_index_env(env, str);
	if (index_env == -1)
		return (env);
	new_env = (char **)ft_calloc(ft_mtrx_size(env), sizeof(char *));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (i != index_env)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
				return (free_matrix(new_env), NULL);
			j++;
		}
		i++;
	}
	return (free_matrix(env), new_env);
}

static int	ft_is_readonly(char **readonly_vars, char *var_name)
{
	int				i;
	unsigned int	size_var;

	size_var = ft_strlen(var_name);
	i = 0;
	if (!var_name || !readonly_vars)
		return (0);
	while (readonly_vars[i])
	{
		if ((size_var == ft_strlen(readonly_vars[i]))
			&& ft_strncmp(var_name, readonly_vars[i], size_var) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	print_unset_err(t_shell *shell, char *arg, char *mesasage)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(mesasage, STDERR_FILENO);
	shell->exit_status = ERROR;
}
