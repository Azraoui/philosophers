/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/07/01 13:12:24 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

t_philos	*fill_philos(t_input *input)
{
	t_philos		*philos;
	pthread_mutex_t	*mutex;
	pthread_mutex_t mut;
	t_shared_info	*shared_info;
	int				i;

	shared_info = (t_shared_info *)malloc(sizeof(t_shared_info));
	mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * input->number_of_philo);
	philos = (t_philos *)malloc(sizeof(t_philos) * input->number_of_philo);
	if (!philos || !shared_info || !mutex)
		return (NULL);
	i = 0;
	shared_info->fork_mutex = mutex;
	pthread_mutex_init(&mut, NULL);
	shared_info->wait_mutex = mut;
	shared_info->philo_die = 0;
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

unsigned int gettime(struct timeval t_val)
{
	unsigned int	time;
	unsigned long long	s;
	struct	timeval	t_vl;

	gettimeofday(&t_vl, NULL);
	s = (t_vl.tv_sec - t_val.tv_sec) * 1000;
	time = ((t_vl.tv_usec - t_val.tv_usec) / 1000) + s;
	return (time);
}

void	ft_usleep(unsigned int time)
{
	unsigned int	s;
	unsigned int	s1;
	struct			timeval t_vl1;
	struct			timeval t_vl2;

	gettimeofday(&t_vl2, NULL);
	usleep(time / (1.1));
	gettimeofday(&t_vl1, NULL);
	s = (t_vl1.tv_sec - t_vl2.tv_sec) * 1000;
	s1 = ((t_vl1.tv_usec - t_vl2.tv_usec) / 1000) + s;
	printf("s1 = %d\n", s1);
	while (s1 != time)
	{
		gettimeofday(&t_vl1, NULL);
		s = (t_vl1.tv_sec - t_vl2.tv_sec) * 1000;
		s1 = ((t_vl1.tv_usec - t_vl2.tv_usec) / 1000) + s;
	}
}

void	*spaghetti_table(void *tmp)
{
	t_philos	*philos;
	struct	timeval t_val;
	int			i;

	philos = (t_philos *)tmp;
	gettimeofday(&t_val, NULL);
	if (philos->philo_id % 2)
		usleep(500);
	i = 0;
	while (!philos->shared_info->philo_die)
	{
		pthread_mutex_lock(&philos->shared_info->fork_mutex[philos->philo_id]);
		printf("%d: %d has taken a fork\n", gettime(t_val), philos->philo_id);
		pthread_mutex_lock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
		printf("%d: %d has taken a fork\n", gettime(t_val), philos->philo_id);
		printf("%d: %d is eating\n", gettime(t_val), philos->philo_id);
		usleep(philos->time_to_eat * 1000);
		pthread_mutex_unlock(&philos->shared_info->fork_mutex[philos->philo_id]);
		pthread_mutex_unlock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
		printf("%d: %d is sleeping\n", gettime(t_val), philos->philo_id);
		usleep(philos->time_to_sleep * 1000);
		printf("%d: %d is thinking\n", gettime(t_val), philos->philo_id);
		i++;
	}
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
	pthread_mutex_destroy(&philos->shared_info->wait_mutex);
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
