/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshvydka <dshvydka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:06:47 by dashvydk          #+#    #+#             */
/*   Updated: 2025/12/16 14:33:10 by dshvydka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"

// information and settings
typedef struct s_program
{
	long long start_time; // Simulation start timestamp (for logs)
	int num_philo;        // number_of_philosophers
	int time_to_die;      // Time limits from arguments
	int		time_to_eat;
	int		time_to_sleep;
	int must_eat_count;// Optional argument: number_of_times_each_philosopher_must_eat
	int is_sim_running;// Flag to stop the simulation (0 for stop, 1 for running)
	pthread_mutex_t write_lock;// Mutex to ensure logs don't overlap (A MUST!)
	pthread_mutex_t *forks;       // Array of all fork mutexes
	struct s_philo *philosophers; // Array of all philosopher structs
}			t_program;

// philosopher data
typedef struct s_philo
{
	int id;                      // Philosopher ID (1 to num_philo)
	int eat_count;               // How many times this philo has eaten
	long long last_meal_time;   // Timestamp of the last time this philo finished eating
	pthread_t thread;            // The philo's thread handle
	t_program *prog;            // Pointer back to the main simulation structure
	pthread_mutex_t *left_fork;  // Pointer to their left fork's mutex
	pthread_mutex_t *right_fork; // Pointer to their right fork's mutex
	pthread_mutex_t meal_lock;  // Mutex to protect last_meal_time and eat_count
}			t_philo;

// utils
int			ft_atoi(const char *str);
long long	get_time(void);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			is_simulation_running(t_program *prog);
// messages
void		print_message(t_philo *philo, const char *message);
// routine
void		*philosopher_routine(void *arg);
void		*philosopher_main_routine(void *philo_ptr);
// philo
int			init_program(t_program *prog, int argc, char **argv);
void		cleanup(t_program *prog);
void		monitor_simulation(t_program *prog);

#endif
