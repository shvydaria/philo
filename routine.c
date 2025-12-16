/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:48:58 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 15:23:45 by dshvydka         ###   ########.fr       */
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
		// usleep(philo->prog->time_to_die * 1000);
		// print_message(philo, MSG_DIED);
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
            ft_usleep(10, philo->prog);
    }
    return (NULL);
}

// void	*philosopher_main_routine(void *philo_ptr)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)philo_ptr;
// 	if (philo->id % 2 != 0)
// 		usleep(1000);
// 	while (1) {
// 		pthread_mutex_lock(&philo->prog->write_lock);
// 		if (!philo->prog->is_sim_running) {
// 			pthread_mutex_unlock(&philo->prog->write_lock);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&philo->prog->write_lock);
// 		eat(philo);
// 		print_message(philo, MSG_SLEEP);
// 		usleep(philo->prog->time_to_sleep * 1000);
// 		print_message(philo, MSG_THINK);
// 	}
// 	return (NULL);
// }
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
