/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:47:43 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/17 12:56:18 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_philo_status(t_philo *philo, int *all_philos_are_full)
{
	pthread_mutex_lock(&philo->meal_lock);
	if (get_time() - philo->last_meal_time > philo->prog->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (1);
	}
	if (philo->prog->must_eat_count != -1
		&& philo->eat_count < philo->prog->must_eat_count)
		*all_philos_are_full = 0;
	pthread_mutex_unlock(&philo->meal_lock);
	return (0);
}

void monitor_simulation(t_program *prog)
{
	int i;
	int all_philos_are_full;

	while (1)
	{
		all_philos_are_full = 1;
		i = -1;
		while (++i < prog->num_philo)
		{
			if (check_philo_status(&prog->philosophers[i],
					&all_philos_are_full))
			{
				print_message(&prog->philosophers[i], MSG_DIED);
				return ;
			}
		}
		if (prog->must_eat_count != -1 && all_philos_are_full)
		{
			pthread_mutex_lock(&prog->write_lock);
			prog->is_sim_running = 0;
			pthread_mutex_unlock(&prog->write_lock);
			return ;
		}
		usleep(1000);
	}
}

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
	monitor_simulation(&prog); // monitor loop here to check for deaths
	cleanup(&prog);
	return (0);
}
