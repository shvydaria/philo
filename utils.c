/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:11:29 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 14:29:58 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// The time is measured in milliseconds since January 1st, 1970. int is too small
long long	get_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
		// seconds to milliseconds + microseconds to milliseconds
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_simulation_running(t_program *prog)
{
	int	running;

	pthread_mutex_lock(&prog->write_lock);
	running = prog->is_sim_running;
	pthread_mutex_unlock(&prog->write_lock);
	return (running);
}
