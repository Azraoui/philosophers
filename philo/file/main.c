/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-azra <ael-azra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 18:34:37 by ael-azra          #+#    #+#             */
/*   Updated: 2021/06/15 12:51:29 by ael-azra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosophers.h"

void	threads_philo(t_input *input)
{
	// write code here !
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
