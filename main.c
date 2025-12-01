/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dashvydk <dashvydk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:47:43 by dashvydk          #+#    #+#             */
/*   Updated: 2025/11/05 12:24:26 by dashvydk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_program	prog;
	int			i;

	if (init_program(&prog, argc, argv) != 0)
		return (1);
	i = 0;
	while (i < prog.num_philo)
	{
		if (pthread_create(&prog.philosophers[i].thread, NULL,
				&philosopher_routine, &prog.philosophers[i]) != 0)
			return (1);
		i++;
	}
	// monitor loop here to check for deaths
	cleanup(&prog);
	return (0);
}
