/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:50:12 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 14:05:37 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philo *philo, const char *message)
{
	long long	time;

	pthread_mutex_lock(&philo->prog->write_lock);
	time = get_time() - philo->prog->start_time;
	if (philo->prog->is_sim_running)
	{
		printf("%lld %d %s\n", time, philo->id, message);
		if (ft_strncmp(message, MSG_DIED, 4) == 0)
			philo->prog->is_sim_running = 0;
	}
	pthread_mutex_unlock(&philo->prog->write_lock);
}
