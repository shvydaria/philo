/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dashvydk <dashvydk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:50:12 by dashvydk          #+#    #+#             */
/*   Updated: 2025/11/05 12:24:08 by dashvydk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philo *philo, const char *message)
{
	long long	time;

	pthread_mutex_lock(&philo->prog->write_lock);
	time = get_time() - philo->prog->start_time;
	if (philo->prog->is_sim_running)
		printf("%lld %d %s\n", time, philo->id, message);
	pthread_mutex_unlock(&philo->prog->write_lock);
}
