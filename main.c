/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:47:43 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/01 22:17:49 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void	monitor_simulation(t_program *prog)
{
	int	i;
	int	all_philos_are_full;

	while (1)
	{
		all_philos_are_full = 1;
		i = 0;
		while (i < prog->num_philo)
		{
			pthread_mutex_lock(&prog->philosophers[i].meal_lock);
			if (get_time() - prog->philosophers[i].last_meal_time > prog->time_to_die)
			{
				print_message(&prog->philosophers[i], MSG_DIED);
				pthread_mutex_lock(&prog->write_lock);
				prog->is_sim_running = 0;
				pthread_mutex_unlock(&prog->write_lock);
				pthread_mutex_unlock(&prog->philosophers[i].meal_lock);
				return ; // Exit monitor
			}
			if (prog->must_eat_count != -1
				&& prog->philosophers[i].eat_count < prog->must_eat_count)
				all_philos_are_full = 0; // At least one philo is not full
			pthread_mutex_unlock(&prog->philosophers[i].meal_lock);
			i++;
		}
		if (prog->must_eat_count != -1 && all_philos_are_full)
		{
			pthread_mutex_lock(&prog->write_lock);
			prog->is_sim_running = 0;
			pthread_mutex_unlock(&prog->write_lock);
			return ; // Exit monitor
		}
		usleep(1000); // Wait a bit to not burn CPU
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
