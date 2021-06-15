/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/15 16:42:46 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

void*	philo_situation(void *input)
{
	printf("%d\n", ((t_input *)input)->number_of_philo);
	usleep(100);
	return (void*)1;
}

void	threads_philo(t_input *input)
{
	pthread_t	*t_philo;
	int			i;

	t_philo = (pthread_t *)malloc(sizeof(pthread_t) * input->number_of_philo);
	while (1)
	{
		i = 0;
		while (i < input->number_of_philo)
			pthread_create(&t_philo[i++], NULL, &philo_situation, input);
		i = 0;
		while (i < input->number_of_philo)
			pthread_join(t_philo[i++], NULL);
		break ;
	}
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
