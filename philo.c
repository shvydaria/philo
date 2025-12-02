/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:06:57 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/01 22:17:59 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_program *prog)
{
	int	i;

	i = 0;
	prog->forks = malloc(sizeof(pthread_mutex_t) * prog->num_philo);
	if (!prog->forks)
		return (1);
	while (i < prog->num_philo)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&prog->write_lock, NULL) != 0)
		return (1);
	return (0);
}

static void	init_philosophers(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->num_philo)
	{
		prog->philosophers[i].id = i + 1;
		prog->philosophers[i].eat_count = 0;
		prog->philosophers[i].last_meal_time = prog->start_time;
		prog->philosophers[i].prog = prog;
		if (pthread_mutex_init(&prog->philosophers[i].meal_lock, NULL) != 0)
			return ;
		prog->philosophers[i].left_fork = &prog->forks[i];
		prog->philosophers[i].right_fork = &prog->forks[(i + 1)
			% prog->num_philo];
		i++;
	}
}

int	init_program(t_program *prog, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"), 1);
	prog->num_philo = ft_atoi(argv[1]);
	prog->time_to_die = ft_atoi(argv[2]);
	prog->time_to_eat = ft_atoi(argv[3]);
	prog->time_to_sleep = ft_atoi(argv[4]);
	prog->must_eat_count = -1;
	if (argc == 6)
		prog->must_eat_count = ft_atoi(argv[5]);
	if (prog->num_philo <= 0 || prog->time_to_die <= 0 || prog->time_to_eat <= 0
		|| prog->time_to_sleep <= 0 || (argc == 6 && prog->must_eat_count <= 0))
		return (printf("Error: Invalid arguments\n"), 1);
	prog->is_sim_running = 1;
	prog->start_time = get_time(); // "birth" moment of simulation
	prog->philosophers = malloc(sizeof(t_philo) * prog->num_philo);
	if (!prog->philosophers)
		return (1);
	if (init_mutexes(prog) != 0)
		return (free(prog->philosophers), 1);
	init_philosophers(prog);
	return (0);
}

void	cleanup(t_program *prog)
{
	int i;

	i = 0;
	while (i < prog->num_philo)
	{
		pthread_join(prog->philosophers[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < prog->num_philo)
	{
		pthread_mutex_destroy(&prog->forks[i]);
		pthread_mutex_destroy(&prog->philosophers[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&prog->write_lock);
	free(prog->forks);
	free(prog->philosophers);
}
