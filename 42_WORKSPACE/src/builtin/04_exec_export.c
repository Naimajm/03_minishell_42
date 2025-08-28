/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_exec_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:23:16 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/25 18:31:38 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas
// 1. marcar variables para exportación:
// Cuando creas una variable en el shell (por ejemplo, MI_VAR="hola"), por defecto solo existe en ese shell. Si ejecutas un script u otro comando, esa variable no estará disponible para éllos
// Ejemplo: export MI_VAR o export MI_VAR="hola". l. El comando export marca esa variable para que se "exporte" al entorno, lo que significa que todos los procesos hijos (comandos o scripts que ejecutes desde la shell) la heredarán.
//2. listar variables exportadas: Si ejecutas export sin ningún argumento, te mostrará una lista de todas las variables que están actualmente en el entorno y que serán pasadas a los procesos hijos, generalmente en el formato declare -x NOMBRE_VARIABLE="valor".
// `export` de tu minishell, que necesita mostrar las variables de entorno ordenadas.

int			exec_export(t_cmd *cmd, t_shell *shell);
static void	print_export(char **env_export);
char		**ft_add_modify_env(char **env, char *var, int flag);
static char	**check_flags(int index, int flag, char **env, char *var);
static void	print_after_equal(char *equal, char *new_var);

int	exec_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 1; // evitar el comando export e ir directamente a los argumentos
	if (!cmd->args[1]) // sin argumentos export lista las variables exportadas
	{
		print_export(shell->environment);
		return (SUCCESS);
	}
	while (cmd->args[i]) // CASO CON ARGUMENTOS
	{
		if (!ft_valid_env_var(cmd->args[i])) // para chequear nombre invalido de variable
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO); // sin salto de linea
			ft_putstr_fd(cmd->args[i], STDERR_FILENO); // sin salto de linea
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO); // con salto de linea
			shell->exit_status = ERROR; // return (1); no retornamos porque necesitamos validar todas las variables pasadas como argumento, ya que el comando export admite multiples variables
		}
		/*else if (ft_strchr(cmd->args[i], '=')) 
			shell->environment = ft_add_modify_env(shell->environment, cmd->args[i],
					ft_valid_env_var(cmd->args[i]));*/
		else if (ft_strncmp(cmd->args[i], "PWD", 3) != 0 && ft_strncmp(cmd->args[i], "OLDPWD", 3) != 0 )  // para volverlo más robusto. En nuestro código si hacemos export PWD, y despues por ejemplo cd nos produce un segment fault, me ha salido de casualidad, aprendiendo el funcionamiento. El export del bash si no se pone la variable de entorno seguida de un = no modifica nada. Para que haga alguna modificación en la variable de entorno o la cree hay que poner por ejemplo export PWD=
			shell->environment = ft_add_modify_env(shell->environment, cmd->args[i],
					ft_valid_env_var(cmd->args[i]));
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
	new_env = ft_copy_mtrx(env); // no trabajamos con strings literales, sino con una copia de las variables de entorno
	if (!new_env) // No necesitas liberar memoria en este punto porque no has allocado nada más
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
	key = ft_get_keyvar(var); // coger la primera parte hasta el igual
	if (!key)
		return (env); // Retornar el entorno original si falla la asignación de memoria
	index = ft_search_index_env(env, key);
	free (key);
	new_env = check_flags(index, flag, env, var);
	if (new_env != env)
		free_matrix(env); // Solo liberar si devolvió un nuevo entorno
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
			return (env);// Retornar el entorno original si falla la asignación
		free(env[index]);// Si no hicieras el free primero, tendrías una fuga de memoria porque perderías la referencia a la memoria original sin haberla liberado. Y si no hicieras el ft_strdup, podrías tener problemas si la variable original var se modifica o se libera posteriormente.
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
		return (env); // En caso de error, devuelve el entorno original en lugar de NULL, para evitar perder todo el entorno
	return (new_env);
}

static void	print_after_equal(char *equal, char *new_var)
{
	*equal = '\0'; // como new_env es una copia mallocada podemos sustituir un caracter por otro. ponemos el terminador cero para que printf termine la impresion ahí. La cadena ahora es: "USER\0emcorona". truco se usa para poder imprimir el nombre de la variable y su valor por separado, Es una técnica común en C para "dividir" temporalmente una cadena en dos.
	if (*(equal + 1) != '\0') // si existe algo despues del igual. tambien vale if (equal[1] != '\0') no vale if (equal + 1) porque esto es una direccion de memoria que siempre es verdadero, aunuqe apunte a nulo
		printf("declare -x %s=\"%s\"\n", new_var, equal + 1); // =\" - imprime =", las comillas están escapadas con \ para imprimirlas literalmente, \"\n - imprime " y un salto de línea
	else // si no existe nada despues del igual imprime comillas vacias
		printf("declare -x %s=\"\"\n", new_var);
	*equal = '=';
}

/*pero, en un strin literal como "USER=emcorona", cmo podemos sustituir un caracter por otro ?

¡Ah! Aquí hay un detalle importante:

En C, cuando declaras un string literal como "USER=emcorona", este se almacena en una sección de memoria de solo lectura y NO puedes modificarlo. Intentar modificar un string literal causará un error de segmentación.

PERO, en tu código no estás trabajando con strings literales, sino con una copia de las variables de entorno:

new_env = ft_copy_mtrx(env);

Esta línea crea una copia mallocada (en memoria dinámica) de las variables de entorno. Por eso:

SÍ puedes hacer esto (porque es memoria mallocada):

*equal = '\0';  // Válido porque equal apunta a memoria mallocada


NO podrías hacer esto (con un string literal):

char *str = "USER=emcorona";
str[4] = '\0';  // ¡CRASH! Segmentation fault

La diferencia es que:

Los strings literales van en memoria de solo lectura
Los strings mallocados (como tu new_env) van en el heap y son modificables
ft_copy_mtrx crea copias mallocadas de los strings, por lo que son seguros de modificar
Por eso tu código funciona: estás modificando una copia mallocada, no un string literal.*/