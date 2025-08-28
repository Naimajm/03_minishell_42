/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:16:48 by emcorona          #+#    #+#             */
/*   Updated: 2025/04/25 19:10:54 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// statics funtions ------------------------------------------------------------

static size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	scr_len;
	size_t	i;

	scr_len = 0;
	while (src[scr_len] != '\0')
		scr_len++;
	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && (i + 1) < size)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (scr_len);
}

static void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if ((dest == NULL && src == NULL) || (dest == src))
		return (dest);
	if (dest > src)
	{
		while (n > 0)
		{
			n--;
			((unsigned char *)dest)[n] = ((unsigned char *)src)[n];
		}
	}
	else
	{
		while (i < n && n != 0)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dest);
}

// utils functions------------------------------------------------------
/**
* @brief take a portion (from star to len) of a string. 
(memory allocations is needed).
* @param s: pointer to the source string.
* @param start: Starting index of the substring in the source string.
* @param len: The length of the substring.
* @returns->char *-> a pointer to the assigned substring (also if len is zero) 
->NULL is returned if !s or memory allocation fails.
*/
char	*ft_gnl_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;
	size_t	len_s;
	char	*s_star;

	if (!s)
		return (NULL);
	len_s = 0;
	while (s[len_s] != '\0')
		len_s++;
	s_star = (char *)s + start;
	if (start > len_s)
		return (ft_gnl_strdup(""));
	if (start + len > len_s)
		len = len_s - start;
	subs = (char *)malloc((len + 1) * sizeof(char));
	if (!subs)
		return (NULL);
	ft_strlcpy(subs, s_star, len + 1);
	return (subs);
}

/**
* @brief return a new string, formed by the concatenation of str1 and str2.
(memory allocations is needed).
* @param s1: 1st string.
* @param s2: string to add to str1.
* @returns -> char *-> new string.
->NULL if memory reservation fails.
*/
char	*ft_gnl_strjoin(char const *s1, char const *s2)
{
	char	*s_plus;
	int		s1_len;
	int		s2_len;

	s1_len = 0;
	while (s1[s1_len] != '\0')
		s1_len++;
	s2_len = 0;
	while (s2[s2_len] != '\0')
		s2_len++;
	if (!s1 || !s2)
		return (NULL);
	s_plus = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!s_plus)
		return (NULL);
	ft_memmove(s_plus, s1, s1_len);
	ft_memmove(&s_plus[s1_len], s2, s2_len + 1);
	return (s_plus);
}

/**
* @brief Duplicate a null-terminated string (src) 
(memory allocations is needed).
* @param s: Pointer to the null-terminated source string to be duplicated.
* @returns -> char * -> pointer to the newly allocated duplicate string.
-> NULL If memory allocation fails.
*/
char	*ft_gnl_strdup(const char *s)
{
	size_t	len;
	char	*ptr_dup;

	len = 0;
	while (s[len] != '\0')
		len++;
	ptr_dup = (char *)malloc((len + 1) * sizeof(char));
	if (!ptr_dup)
		return (NULL);
	ft_strlcpy(ptr_dup, s, len + 1);
	return (ptr_dup);
}
