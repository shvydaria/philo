/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:48:58 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/02 18:53:17 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	// Deadlock prevention: even/odd rule
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
	print_message(philo, MSG_EAT);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_lock);
	usleep(philo->prog->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
// Eating (The Hard Part):

// Deadlock: If every philosopher grabs their left fork at the same time, nobody can grab a right fork, and everyone starves. This is a deadlock.
// The Solution: We'll use the even/odd strategy you mentioned.
// If a philosopher has an even ID, they will lock their left_fork first, then their right_fork.
// If they have an odd ID, they will do the opposite: lock right_fork first, then left_fork.
// This breaks the circular dependency and prevents deadlock.
// Actions: Once they have both forks, they print_message, update their last_meal_time and eat_count, and then usleep for time_to_eat.
// Cleanup: After eating, they must unlock both forks so others can use them.
// Sleeping & Thinking: These are simple. Just print the message and usleep for sleeping. Thinking doesn't require a usleep;
// it's just the state they enter after sleeping and before trying to eat again.

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->prog->num_philo == 1)
	{
		print_message(philo, MSG_FORK);
		usleep(philo->prog->time_to_die * 1000);
		print_message(philo, MSG_DIED);
		return (NULL);
	}
	philosopher_main_routine(philo);
	return (NULL);
}

void	*philosopher_main_routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	// Small delay for odd philosophers to prevent them all starting at once
	if (philo->id % 2 != 0)
		usleep(1000);
	// The monitor will stop the simulation
	while (1) {
		// Check if simulation should end
		pthread_mutex_lock(&philo->prog->write_lock);
			// Re-using write_lock for sim status
		if (!philo->prog->is_sim_running) {
			pthread_mutex_unlock(&philo->prog->write_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->prog->write_lock);
		eat(philo);
		print_message(philo, MSG_SLEEP);
		usleep(philo->prog->time_to_sleep * 1000);
		print_message(philo, MSG_THINK);
	}
	return (NULL);
}
// 1. THINKING 🧘
//    - Log: timestamp_in_ms X is thinking

// 2. GET FORKS & EAT 🍝
//    - Grasp the FIRST fork (e.g., the lower-numbered one): pthread_mutex_lock(first_fork)
//    - Log: timestamp_in_ms X has taken a fork
//    - Grasp the SECOND fork: pthread_mutex_lock(second_fork)
//    - Log: timestamp_in_ms X has taken a fork
//    - Log: timestamp_in_ms X is eating
//    - Update philo->last_meal_time
//    - Call usleep(philo->prog->time_to_eat * 1000);
//    - Update philo->eat_count

// 3. PUT FORKS DOWN
//   - Release the forks: pthread_mutex_unlock(first_fork) and pthread_mutex_unlock(second_fork)

// 4. SLEEPING 😴
//    - Log: timestamp_in_ms X is sleeping
//    - Call usleep(philo->prog->time_to_sleep * 1000);
