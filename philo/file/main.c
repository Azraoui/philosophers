/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/28 12:43:15 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

t_philos	*fill_philos(t_input *input)
{
	t_philos		*philos;
	pthread_mutex_t	*mutex;
	t_shared_info	*shared_info;
	int				i;

	shared_info = (t_shared_info *)malloc(sizeof(t_shared_info));
	mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * input->number_of_philo);
	philos = (t_philos *)malloc(sizeof(t_philos) * input->number_of_philo);
	if (!philos || !shared_info || !mutex)
		return (NULL);
	i = 0;
	shared_info->fork_mutex = mutex;
	while (i < input->number_of_philo)
	{
		philos[i].philo_id = i;
		philos[i].time_to_die = input->time_to_die;
		philos[i].time_to_eat = input->time_to_eat;
		philos[i].time_to_sleep = input->time_to_sleep;
		philos[i].number_of_philo = input->number_of_philo;
		philos[i].number_of_time_to_eat = input->number_of_time_to_eat;
		philos[i].shared_info = shared_info;
		i++;
	}
	return (philos);
}

void	*spaghetti_table(void *tmp)
{
	t_philos	*philos;

	philos = (t_philos *)tmp;
	if (philos->philo_id % 2)
		usleep(500);
	pthread_mutex_lock(&philos->shared_info->fork_mutex[philos->philo_id]);
	printf("%d: tacke fork 1\n", philos->philo_id);
	pthread_mutex_lock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
	printf("%d: tacke fork 2\n", philos->philo_id);
	// pthread_mutex_unlock(&philos->shared_info->fork_mutex[philos->philo_id]);
	// pthread_mutex_unlock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
	return (NULL);
}

int	create_philo_threads(int size, t_philos *philos)
{
	int		i;
	pthread_t		*philo_threads;

	philo_threads = (pthread_t *)malloc(sizeof(pthread_t) * size);
	if (!philo_threads)
		return (1);
	i = 0;
	while (i < size)
	{
		if (pthread_create(&philo_threads[i], NULL, spaghetti_table, &philos[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < size)
		if (pthread_join(philo_threads[i++], NULL))
			return (1);
	return (0);
}

void	threads_philo(t_input *input)
{
	t_philos		*philos;

	philos = fill_philos(input);
	if (!philos)
		return ;
	for (int i = 0; i < input->number_of_philo; i++)
		pthread_mutex_init(&philos->shared_info->fork_mutex[i], NULL);
	create_philo_threads(input->number_of_philo, philos);
	for (int i = 0; i < input->number_of_philo; i++)
		pthread_mutex_destroy(&philos->shared_info->fork_mutex[i]);
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
