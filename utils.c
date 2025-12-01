/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dashvydk <dashvydk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:11:29 by dashvydk          #+#    #+#             */
/*   Updated: 2025/11/05 12:24:50 by dashvydk         ###   ########.fr       */
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
