/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:47:43 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 14:06:30 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor_simulation(t_program *prog)
{
	int	i;
	int	all_philos_are_full;
	int	sim_should_end;

	while (1)
	{
		all_philos_are_full = 1;
		sim_should_end = 0;
		i = -1;
		while (++i < prog->num_philo)
		{
			pthread_mutex_lock(&prog->philosophers[i].meal_lock);
			if (get_time() - prog->philosophers[i].last_meal_time > prog->time_to_die)
			{
				print_message(&prog->philosophers[i], MSG_DIED);
				sim_should_end = 1;
			}
			if (prog->must_eat_count != -1
				&& prog->philosophers[i].eat_count < prog->must_eat_count)
				all_philos_are_full = 0;
			pthread_mutex_unlock(&prog->philosophers[i].meal_lock);
			if (sim_should_end)
				return ;
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
