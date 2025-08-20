/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 17:35:20 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 04:24:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif
# ifndef MAX_FD
#  define MAX_FD 1024
# endif

char	*get_next_line(int fd);
void	**read_imput_controler(void **table, char *buffer, int fd);
void	**ft_split_lines(void **table, char *s, ssize_t readed);
char	*l(char *line, char *buffer, ssize_t len, ssize_t cut);
void	**dispatch_table_lines(void **table);
void	**create_new_branch(void **table);
void	**ft_redim_fill_table(void **table, ssize_t new_lines, ssize_t lines);
void	*ft_realloc_fill_ssline(void **table, ssize_t new_columns,
			ssize_t columns, ssize_t target);
void	**free_all(void **table, char *buffer);

#endif
