/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:37:33 by emcorona          #+#    #+#             */
/*   Updated: 2025/04/25 18:45:25 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
// DEFINITION OF BUFFER SIZE IF IT IS NOT DEFINED IN THE COMPILATION
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
//include: ---------------------------------------------------------
# include <unistd.h> // write, read
# include <stdlib.h> // free malloc
// prototype:-------------------------------------------------------
char	*get_next_line(int fd);
// utils.c:----------------------------------------------------
char	*ft_gnl_substr(char const *s, unsigned int start, size_t len);
char	*ft_gnl_strjoin(char const *s1, char const *s2);
char	*ft_gnl_strdup(const char *s);
// --------------------------------------------------------------------
#endif