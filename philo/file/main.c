/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/26 18:37:36 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

void	ft_fork(t_philos *philo)
{
	if (philo->philo_id == 0)
	{
		if (philo->forks[0] == '0')
		{
			philo->nbr_fork++;
			philo->forks[philo->philo_id] = '1';
		}
		if (philo->forks[philo->number_of_philo - 1] == '0')
		{
			philo->nbr_fork++;
			philo->forks[philo->number_of_philo - 1] = '1';
		}
	}
	else
	{
		if (philo->forks[philo->philo_id] == '0')
		{
			philo->nbr_fork++;
			philo->forks[philo->philo_id] = '1';
		}
		if (philo->forks[philo->philo_id - 1] == '0')
		{
			philo->nbr_fork++;
			philo->forks[philo->philo_id - 1] = '1';
		}
	}
}

void	*do_action(void *s)
{
	t_philos *philo;

	philo = ((t_philos *)s);
	pthread_mutex_lock(&philo->mutex);
	ft_fork(philo);
	printf("%s\n", philo->forks);
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void	threads_philo(t_input *input)
{
	int			i;
	t_philos	*philo;
	pthread_t	*pthread_philo;

	pthread_philo = (pthread_t *)malloc(sizeof(pthread_t) * input->number_of_philo);
	philo = (t_philos *)malloc(sizeof(t_philos) * input->number_of_philo);
	pthread_mutex_init(&philo->mutex, NULL);
	i = 0;
	while (i < input->number_of_philo)
	{
		philo[i].forks = (char *)malloc(sizeof(char) * (input->number_of_philo + 1));
		philo[i].forks = memset(philo[i].forks, '0', input->number_of_philo);
		philo[i].philo_id = i;
		philo[i].number_of_philo = input->number_of_philo;
		philo[i].nbr_fork = 0;
		philo[i].time_to_die = input->time_to_die;
		philo[i].time_to_eat = input->time_to_eat;
		philo[i].time_to_sleep = input->time_to_sleep;
		philo[i].number_of_time_to_eat = input->number_of_time_to_eat;
		i++;
	}
	i = 0;
	while (i < input->number_of_philo)
	{
		pthread_create(&pthread_philo[i], NULL, do_action, &philo[i]);
		i++;
	}
	i = 0;
	while (i < input->number_of_philo)
		pthread_join(pthread_philo[i++], NULL);
	pthread_mutex_destroy(&philo->mutex);
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
			free(input);
			return (0);
		}
	}
	printf("Error\n");
	return (1);
}
