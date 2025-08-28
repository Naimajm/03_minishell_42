/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_exec_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:13:03 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 17:33:00 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

static void	update_pwd(t_shell *shell);
static char	*path_cd(t_cmd *cmd, t_shell *shell);

int	exec_cd(t_cmd *cmd, t_shell *shell) //  , según el subject, solo para path relativos o absoolutos, no hay que manejar el ir a $HOME en caso de no argumentos, ni actualizar las variables de entorno PWD y OLDPWD, ni Manejar el caso cd - (ir al directorio anterior)
{
	char	*path;

	if (cmd->args && ft_mtrx_size(cmd->args) > 2)
		return (ft_putendl_fd("minishell: cd: too many arguments",
				STDERR_FILENO), ERROR); // para errores de sintaxis o uso del comando ft_putstr_fd es mas apropiado. Sigue el formato estándar de shell minishell: comando: mensaje
	path = path_cd(cmd, shell);
	if (chdir(path) == -1) // chdir funciona tanto con paths relativos como absolutos.  mostrando el mensaje de error proporcionado por perror(), que incluirá el motivo específico del fallo. Por ejemplo: minishell: cd: No such file or directory , minishell: cd: Not a directory, minishell: cd: Permission denied
	{
		perror("minishell: cd:"); // perror para errores del sistma (chdir) //shell->exit_status = ERROR; // estado de salida en caso de error del ultimo comando ejecutado		
		if (path)
			free(path);
		return (ERROR);
	}
	update_pwd(shell);
	if (path)
		free(path);
	return (SUCCESS);
}

static void	update_pwd(t_shell *shell)
{
	char	*cwd;
	char	*pwd;
	char	*old_pwd;

	pwd = get_environment_var(shell->environment, "PWD");
	old_pwd = ft_strjoin("OLDPWD=", pwd);
	free(pwd);
	cwd = getcwd(NULL, 0); // asigna memoria dinámicamente al pasarn NULL como argumento y cero como tamaño de buffer, por lo que el tamaño será determinado por la función.
	pwd = ft_strjoin("PWD=", cwd);
	shell->environment = ft_add_modify_env(shell->environment, old_pwd,
			ft_valid_env_var(old_pwd));
	shell->environment = ft_add_modify_env(shell->environment, pwd,
			ft_valid_env_var(pwd));
	free(pwd);
	free(cwd);
	free(old_pwd);
}

static char	*path_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*home;

	path = NULL;
	if (cmd->args[1] == NULL || ft_strcmp(cmd->args[1], "") == 0)
		path = get_environment_var(shell->environment, "HOME");
	else if (cmd->args[1][0] == '~')
	{
		home = get_environment_var(shell->environment, "HOME");
		path = ft_strjoin(home, &cmd->args[1][1]);
		free(home);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
		path = get_environment_var(shell->environment, "OLDPWD");
	else
		path = ft_strdup(cmd->args[1]);
	return (path);
}

/* El subject no pide lo siguiente
Manejar el caso sin argumentos (ir a $HOME)
Manejar el caso cd - (ir al directorio anterior)
Actualizar las variables de entorno PWD y OLDPWD */
/* La implementación correcta del comando cd en un shell mas complejo normalmente debería incluir:
Manejo de cd sin argumentos (ir a HOME)
Actualización de las variables de entorno PWD y OLDPWD
Manejo de cd - (ir al directorio anterior)
Manejo de paths relativos y absolutos */