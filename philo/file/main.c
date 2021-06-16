/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/16 14:42:42 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

pthread_mutex_t mutex;
int	i;

void*	philo_situation(void *var)
{
	t_variables *tmp;

	tmp = (t_variables *)var;
	pthread_mutex_lock(&mutex);
	if (tmp->position == 0)
	{
		
	}
	pthread_mutex_unlock(&mutex);
	return ((void*)1);
}

void	threads_philo(t_input *input)
{
	pthread_t	*t_philo;
	t_variables	var;

	pthread_mutex_init(&mutex, NULL);
	var.forks = (char *)malloc(sizeof(char) * (input->number_of_philo + 1));
	var.forks = memset(var.forks, '0', input->number_of_philo);
	t_philo = (pthread_t *)malloc(sizeof(pthread_t) * input->number_of_philo);
	while (1)
	{
		var.position = 0;
		while (var.position < input->number_of_philo)
		{
			pthread_create(t_philo + var.position, NULL, &philo_situation, &var);
			usleep(100);
			var.position++;
		}
		var.position = 0;
		while (var.position < input->number_of_philo)
			pthread_join(t_philo[var.position++], NULL);
		break ;
	}
	pthread_mutex_destroy(&mutex);
	free(t_philo);
}

int	main(int ac, char *av[])
{
	t_input	*input;

	if (ac == 5 || ac == 6)
	{
		input = read_input(ac, av);
		if (!input)
		{
			printf("Error\n");
			return (1);
		}
		else
		{
			threads_philo(input);
			// system("leaks philo");
			free(input);
			return (0);
		}
	}
	printf("Error\n");
	return (1);
}
