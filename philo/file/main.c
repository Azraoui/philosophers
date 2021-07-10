/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/07/10 14:59:57 by ael-azra         ###   ########.fr       */
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

unsigned int gettime(struct timeval time_zero)
{
	unsigned int		time;
	unsigned long long	s;
	struct timeval		last_time;

	gettimeofday(&last_time, NULL);
	s = (last_time.tv_sec - time_zero.tv_sec) * 1000;
	time = ((last_time.tv_usec - time_zero.tv_usec) / 1000) + s;
	return (time);
}

void	lock_fork(pthread_mutex_t *fork, unsigned int time, int philo_id)
{
	pthread_mutex_lock(fork);
	printf("%d: %d has taken a fork\n", time, philo_id);
}

void	*spaghetti_table(void *tmp)
{
	t_philos		*philos;
	struct	timeval first_time;
	unsigned int	die;

	philos = (t_philos *)tmp;
	die = 0;
	gettimeofday(&first_time, NULL);
	if (philos->philo_id % 2)
		usleep(500);
	while (philos->number_of_time_to_eat)
	{
		if (gettime(first_time) - (unsigned int)die > (unsigned int)philos->time_to_die)
		{
			printf("%d: %d died\n", gettime(first_time), philos->philo_id);
			philos->shared_info->philo_die = 1;
			exit(1);
		}
		lock_fork(&philos->shared_info->fork_mutex[philos->philo_id], gettime(first_time), philos->philo_id);
		lock_fork(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo], gettime(first_time), philos->philo_id);
		// pthread_mutex_lock(&philos->shared_info->fork_mutex[philos->philo_id]);
		// printf("%d: %d has taken a fork\n", gettime(t_val), philos->philo_id);
		// pthread_mutex_lock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
		// printf("%d: %d has taken a fork\n", gettime(t_val), philos->philo_id);
		printf("%d: %d is eating\n", gettime(first_time), philos->philo_id);
		usleep(philos->time_to_eat * 1000);
		die = gettime(first_time);
		pthread_mutex_unlock(&philos->shared_info->fork_mutex[philos->philo_id]);
		pthread_mutex_unlock(&philos->shared_info->fork_mutex[(philos->philo_id + 1) % philos->number_of_philo]);
		printf("%d: %d is sleeping\n", gettime(first_time), philos->philo_id);
		usleep(philos->time_to_sleep * 1000);
		printf("%d: %d is thinking\n", gettime(first_time), philos->philo_id);
		if (philos->number_of_time_to_eat != -1)
			philos->number_of_time_to_eat--;
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
	int				i;

	philos = fill_philos(input);
	if (!philos)
		return ;
	i = 0;
	while (i < input->number_of_philo)
		pthread_mutex_init(&philos->shared_info->fork_mutex[i++], NULL);
	create_philo_threads(input->number_of_philo, philos);
	while (i < input->number_of_philo)
		pthread_mutex_destroy(&philos->shared_info->fork_mutex[i++]);
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
