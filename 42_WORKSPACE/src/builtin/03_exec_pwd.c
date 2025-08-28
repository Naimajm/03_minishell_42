/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_exec_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 09:52:44 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/27 10:52:06 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

int	exec_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); // asigna memoria dinámicamente al pasarn NULL como argumento y cero como tamaño de buffer, por lo que el tamaño será determinado por la función.
	if (!pwd)
	{
		perror("minishell: pwd:"); // perror para errores del sistema (open, read, dup,....)
		return (ERROR);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (SUCCESS);
}
