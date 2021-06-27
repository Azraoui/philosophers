/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/27 14:53:30 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

void	*do_action(void *s)
{
	t_philos *philo;

	philo = ((t_philos *)s);
	pthread_mutex_lock(&philo->mutex_thread);
	if (philo->philo_id == 0)
	{
		pthread_mutex_lock(&philo->mutex[philo->philo_id]);
		printf("first fork, %d\n", philo->philo_id);
		pthread_mutex_lock(&philo->mutex[philo->number_of_philo - 1]);
		printf("second fork, %d\n", philo->philo_id);
	}
	else if (philo->philo_id == 1)
	{
		pthread_mutex_lock(&philo->mutex[philo->philo_id]);
		printf("first fork, %d\n", philo->philo_id);
		pthread_mutex_lock(&philo->mutex[philo->philo_id - 1]);
		printf("second fork, %d\n", philo->philo_id);
	}
	pthread_mutex_unlock(&philo->mutex_thread);
	return (NULL);
}

void	threads_philo(t_input *input)
{
	int			i;
	t_philos	*philo;
	pthread_t	*pthread_philo;
	pthread_mutex_t *forks;
	pthread_mutex_t f;

	pthread_philo = (pthread_t *)malloc(sizeof(pthread_t) * input->number_of_philo);
	philo = (t_philos *)malloc(sizeof(t_philos) * input->number_of_philo);
	pthread_mutex_init(&f, NULL);
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * input->number_of_philo);
	// pthread_mutex_init(forks, NULL);
	i = 0;
	while (i < input->number_of_philo)
		pthread_mutex_init(&forks[i++], NULL);
	i = 0;
	while (i < input->number_of_philo)
	{
		philo[i].philo_id = i;
		philo[i].number_of_philo = input->number_of_philo;
		philo[i].mutex = forks;
		philo[i].mutex_thread = f;
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
	i = 0;
	pthread_mutex_destroy(forks);
	pthread_mutex_destroy(&f);
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
