/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_exec_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:23:16 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 19:32:46 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int			exec_export(t_cmd *cmd, t_shell *shell);
static void	print_export(char **env_export);
char		**ft_add_modify_env(char **env, char *var, int flag);
static char	**check_flags(int index, int flag, char **env, char *var);
static void	print_after_equal(char *equal, char *new_var);

int	exec_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
		return (print_export(shell->environment), SUCCESS);
	while (cmd->args[i])
	{
		if (!ft_valid_env_var(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			shell->exit_status = ERROR;
		}
		else if (ft_strncmp(cmd->args[i], "PWD", 3) != 0
			&& ft_strncmp(cmd->args[i], "OLDPWD", 3) != 0)
			shell->environment = ft_add_modify_env(shell->environment,
					cmd->args[i], ft_valid_env_var(cmd->args[i]));
		i++;
	}
	if (shell->exit_status == ERROR)
		return (ERROR);
	return (SUCCESS);
}

static void	print_export(char **env)
{
	char	**new_env;
	int		i;
	char	*equal;

	if (!env)
		return ;
	new_env = ft_copy_mtrx(env);
	if (!new_env)
		return ;
	sort_alphabetic_mtrx(new_env);
	i = 0;
	while (new_env[i])
	{
		equal = ft_strchr(new_env[i], '=');
		if (!equal)
			printf("declare -x %s\n", new_env[i]);
		else
			print_after_equal(equal, new_env[i]);
		i++;
	}
	free_matrix(new_env);
}

char	**ft_add_modify_env(char **env, char *var, int flag)
{
	char	*key;
	int		index;
	char	**new_env;

	if (!var)
		return (env);
	key = ft_get_keyvar(var);
	if (!key)
		return (env);
	index = ft_search_index_env(env, key);
	free (key);
	new_env = check_flags(index, flag, env, var);
	if (new_env != env)
		free_matrix(env);
	return (new_env);
}

static char	**check_flags(int index, int flag, char **env, char *var)
{
	char	**new_env;
	char	*new_var;

	if (index != -1 && flag == 1)
	{
		new_var = ft_strdup(var);
		if (!new_var)
			return (env);
		free(env[index]);
		env[index] = new_var;
		return (env);
	}
	if (index != -1 && flag == 2)
	{
		new_env = ft_append_env_var_value(env, var, index);
		if (!new_env)
			return (env);
		return (new_env);
	}
	new_env = ft_create_new_env(env, var);
	if (!new_env)
		return (env);
	return (new_env);
}

static void	print_after_equal(char *equal, char *new_var)
{
	*equal = '\0';
	if (*(equal + 1) != '\0')
		printf("declare -x %s=\"%s\"\n", new_var, equal + 1);
	else
		printf("declare -x %s=\"\"\n", new_var);
	*equal = '=';
}
