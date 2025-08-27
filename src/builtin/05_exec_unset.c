/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:31:52 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/26 17:19:43 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas
// subject: with no options
//para eliminar variables de entorno o de shell, y funciones del shell (con -f). ej. unset NOMBRE_VARIABLE
/* Comportamiento importante:
No muestra ningún output al ejecutarse con éxito
Retorna 0 si tiene éxito
No da error si intentas eliminar una variable que no existe
No puede eliminar variables de solo lectura 
codigos de salida de unset:
Código	Razón del Fallo
1	Identificador de variable inválido. manejarlo con ft_check_env_var
1	Intento de eliminar una variable readonly.  manejarlo con ft_readonly_env_var
2	Se proporcionó una opción inválida. (el subject no lo contempla)
0	Éxito (incluso si la variable no existía).
VIP : unset puede recibir múltiples variables para eliminar.
Un nombre de variable válido no puede contener el signo =
*/

int			exec_unset(t_shell *shell, t_cmd *cmd);
static char	**ft_change_env(char **env, char *str);
static int	ft_is_readonly(char **readonly_vars, char *var_name);
static void	print_unset_err(t_shell *shell, char *arg, char *mesasage);

int	exec_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1; //  El bucle while debería empezar a procesar desde el primer argumento de unset, no desde el propio comando. cmd->args[0] es "unset", por lo que deberías empezar en i = 1.	
	if (!shell || !cmd || !shell->environment || !cmd->args)
		return (1);
	while (cmd->args[i])
	{
		if (!ft_valid_env_var(cmd->args[i]) || ft_strchr(cmd->args[i], '=')) // para chequear nombre invalido de variable. y verificacion de que no haya signo = // VER NOTA SOBRE IMPRESION DE ERRORES Construir el mensaje de error completo en STDERR
			print_unset_err(shell, cmd->args[i], "': not a valid identifier");
		else if (ft_is_readonly(shell->readonly_vars, cmd->args[i])) // readonly
			print_unset_err(shell, cmd->args[i], "': readonly variable");
		else if (ft_strcmp(cmd->args[1], "HOME") != 0
				&& ft_strcmp(cmd->args[1], "PWD") != 0
				&& ft_strcmp(cmd->args[1], "OLDPWD") != 0)
		{
			shell->environment = ft_change_env(shell->environment,
					cmd->args[i]);
			if (!shell->environment)
				return (ERROR); // error de memoria
		}
		i++;
	}
	return (shell->exit_status); // en funcion del valor de shell->exit_status, ya sea cero o uno // unset puede recibir múltiples variables para eliminar, por eso retornamos el error ahora y no durante el ciclo while
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
	new_env = (char **)ft_calloc(ft_mtrx_size(env), sizeof(char *)); // Usar ft_calloc en lugar de malloc para inicializar todo a NULL automáticamente
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (i != index_env) // no eliminamos una variable de entorno, si no que solo copiamos las que no coinciden con la que queremos elimar, es decir generamos un nuevo enviroment sin la que estamo queriendo quitar.  Es una forma segura y común de "eliminar" un elemento de un array de cadenas.
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j]) // comprobar quue lo anterior fue exitoso y si no liberar todo
				return (free_matrix(new_env), NULL);
			j++;
		}
		i++;
	}
	return (free_matrix(env), new_env); // new_env[j] = NULL; // tenemos sitio para cerrar el nuevo enviroment ya que al quitar una queda el sitio para el null sin tener que sumar uno  mas en el malloc.  // No es necesario añadir new_env[j] = NULL ya que ft_calloc ya inicializó todo a NULL
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
			return (1); // la variable es readonly
		i++;
	}
	return (0); // la variable no es readonly
}

static void	print_unset_err(t_shell *shell, char *arg, char *mesasage)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO); // sin salto de linea
	ft_putstr_fd(arg, STDERR_FILENO); // sin salto de linea
	ft_putendl_fd(mesasage, STDERR_FILENO);
	shell->exit_status = ERROR;
}

// NOTAS:

/* ft_putstr_fd("minishell: unset: ", STDERR_FILENO); 
printf("%s: invalid identifier\n", cmd->args[i]);
Estás mezclando ft_putstr_fd (que escribe en STDERR) con printf (que escribe en STDOUT).
El resultado es que una parte del mensaje de error (minishell: unset: ) va a la salida de error, pero el resto (nombre-invalido: invalid identifier) va a la salida estándar. Un shell debe enviar todo el mensaje de error a STDERR. */