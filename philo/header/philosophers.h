/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:35:40 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/16 11:31:28 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_input
{
	int	number_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_time_to_eat;
}				t_input;

typedef	struct	s_variables
{
	char			*forks;
	int				position;
	pthread_mutex_t	mutex;
}				t_variables;

t_input	*read_input(int ac, char *av[]);
size_t	ft_strlen(const char *str);
int		ft_isdigit(int c);

#endif