/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:48:58 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/18 13:45:46 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, MSG_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, MSG_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, MSG_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, MSG_FORK);
	}
	pthread_mutex_lock(&philo->meal_lock);
	print_message(philo, MSG_EAT);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->prog->time_to_eat, philo->prog);
	pthread_mutex_lock(&philo->meal_lock);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->prog->num_philo == 1)
	{
		print_message(philo, MSG_FORK);
		ft_usleep(philo->prog->time_to_die, philo->prog);
		return (NULL);
	}
	philosopher_main_routine(philo);
	return (NULL);
}

void	*philosopher_main_routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	if (philo->id % 2 != 0)
		ft_usleep(philo->prog->time_to_eat / 10, philo->prog);
	while (is_simulation_running(philo->prog))
	{
		eat(philo);
		print_message(philo, MSG_SLEEP);
		ft_usleep(philo->prog->time_to_sleep, philo->prog);
		print_message(philo, MSG_THINK);
		if (philo->prog->num_philo % 2 != 0)
			ft_usleep(philo->prog->time_to_eat, philo->prog);
	}
	return (NULL);
}
