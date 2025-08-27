/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:29:00 by emcorona          #+#    #+#             */
/*   Updated: 2025/04/25 19:10:15 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// statics funtions ----------------------------------------------------------
/**
* @brief Finds the first occurrence of a specific character in a string.
* @param s: pointer to null-terminated string in which search.
* @param c: The character to search.
* @returns -> char *-> Pointer to the first occurrence of the char in string.
-> NULL if character not found
*/
static char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	c_char;

	c_char = c;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c_char)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == '\0' && s[i] == c_char)
		return ((char *)&s[i]);
	return (NULL);
}

/**
 * @brief lee una línea por cachos de buffer hasta que ecuentre 
 * un salto de línea o cero como final de párrafo.
 * @param int fd: numero del file descriptor donde se leerá.
 * @param char *static_buffer: espacio de memoria de tamaño 
 * BUFFER_SIZE (se lo marcamos en el main)  * donde se va almacenando 
 * la lectura en cada ciclo.
 * @param char	*dirty_line: la línea a reinicializada en el punto 
 * donde read se queda leyendo
 * @return char	*dirty_line:(la línea incluyendo salto de línea)
 */
static char	*fs_read_dirty_line(int fd, char *dirty_line, char *dinamic_buffer)
{
	int		n_read;
	char	*temp;

	n_read = 1;
	while (n_read > 0)
	{
		n_read = read (fd, dinamic_buffer, BUFFER_SIZE);
		if (n_read == -1)
		{
			free(dirty_line);
			return (dirty_line = NULL, NULL);
		}
		if (n_read == 0)
			break ;
		dinamic_buffer[n_read] = '\0';
		if (!dirty_line)
			dirty_line = ft_gnl_strdup("");
		temp = dirty_line;
		dirty_line = ft_gnl_strjoin(dirty_line, dinamic_buffer);
		free(temp);
		temp = NULL;
		if (ft_strchr(dinamic_buffer, '\n'))
			break ;
	}
	return (dirty_line);
}

/**
 * @brief limpia la línea dejando la parte hasta llegar al salto de línea.
 * @param char *dirty_line: la línea que incluye el salto de línea.
 * @return char	*next_line: la línea depurada.
 */
static char	*fs_clean_line(char *dirty_line)
{
	char	*next_line;
	size_t	len;

	len = 0;
	if (!dirty_line || dirty_line[0] == '\0')
		return (NULL);
	while ((dirty_line[len] != '\n') && (dirty_line[len] != '\0'))
		len++;
	next_line = ft_gnl_substr(dirty_line, 0, len + 1);
	return (next_line);
}

/**
 * @brief recupera la parte que haya a la derecha del salto de línea y 
 * continuar el ciclo (debido a que la funcion read se  * quedó en 
 * esa posición de lectura)
 * @param char *dirty_line: la línea que incluye el salto de línea.
 * @return char *dirty_line: se autoacutaliza con  el resto de caracteres 
 * a la derecha del salto de línea.
 */
static char	*fs_restart_dirty_line(char *dirty_line)
{
	size_t	len;
	size_t	start;

	len = 0;
	start = 0;
	if (!dirty_line)
		return (NULL);
	while (dirty_line[len] != '\0')
		len++;
	while ((dirty_line[start] != '\n') && (dirty_line[start] != '\0'))
		start++;
	if (dirty_line[start] == '\0' || dirty_line[start + 1] == '\0')
		return (NULL);
	dirty_line = ft_gnl_substr(dirty_line, start + 1, (len - start));
	return (dirty_line);
}

char	*get_next_line(int fd)
{
	static char	*dirty_line;
	char		*next_line;
	char		*dinamyc_buffer;
	char		*total_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(dirty_line);
		return (dirty_line = NULL, NULL);
	}
	dinamyc_buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!dinamyc_buffer)
		return (NULL);
	total_line = fs_read_dirty_line(fd, dirty_line, dinamyc_buffer);
	free(dinamyc_buffer);
	dinamyc_buffer = NULL;
	if (!total_line)
		return (dirty_line = NULL, NULL);
	dirty_line = fs_restart_dirty_line(total_line);
	next_line = fs_clean_line(total_line);
	free(total_line);
	total_line = NULL;
	return (next_line);
}
