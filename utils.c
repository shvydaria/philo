/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:11:29 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 15:06:29 by dshvydka         ###   ########.fr       */
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

int ft_usleep(size_t milliseconds, t_program *prog)
{
    size_t start;

    start = get_time();
    while ((get_time() - start) < milliseconds)
    {
        // Check if someone died while we are sleeping
        if (!is_simulation_running(prog))
            return (0);
        
        // Sleep in very short bursts (500 microseconds)
        usleep(500); 
    }
    return (1);
}
