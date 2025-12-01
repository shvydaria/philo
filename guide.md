You have to write a program for the mandatory part and another one for the bonus part
(if you decide to do the bonus part). They both have to comply with the following rules:
• Global variables are forbidden!
• Your program(s) must take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
◦ number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher has not started eating within
time_to_die milliseconds since the start of their last meal or the start of the
simulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
• Each philosopher has a number ranging from 1 to number_of_philosophers.
• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher
N + 1.
8
Philosophers I never thought philosophy would be so deadly
About the logs of your program:
• Any state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.
• A displayed state message should not overlap with another message.
• A message announcing a philosopher’s death must be displayed within 10 ms of
their actual death.
• Again, philosophers should avoid dying!
Your program must not have any data races.
Mandatory part
Program Name philo
Files to Submit Makefile, *.h, *.c, in directory philo/
Makefile NAME, all, clean, fclean, re
Arguments number_of_philosophers time_to_die time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
External Function memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock
Libft authorized No
Description Philosophers with threads and mutexes
The specific rules for the mandatory part are:
• Each philosopher must be represented as a separate thread.
• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, they will have access to just one fork.
• To prevent philosophers from duplicating forks, you should protect each fork’s state
with a mutex.




// 1. Argument Parsing & Validation
    //    - Check argc (must be 5 or 6)
    //    - Convert argv strings to integers (e.g., using atoi or custom function)

    // 2. Initialize Data
    //    - prog.start_time = get_current_time_ms();
    //    - Initialize prog.write_lock (pthread_mutex_init)
    //    - Allocate memory for prog.forks (array of mutexes) and prog.philosophers (array of structs)
    //    - Initialize all fork mutexes (pthread_mutex_init)

    // 3. Link Philosophers to Forks
    //    - For i from 0 to num_philo - 1:
    //      - prog.philosophers[i].left_fork = &prog.forks[i];
    //      - prog.philosophers[i].right_fork = &prog.forks[(i + 1) % prog.num_philo]; // Circular structure!

    // 4. Thread Creation
    //    - Loop through philosophers and call:
    //      - pthread_create(&prog.philosophers[i].thread, NULL, philosopher_routine, &prog.philosophers[i]);

    // 5. Monitoring (The optional but highly recommended way)
    //    - A separate function/thread to constantly check if any philosopher has died.
    //    - You could also use pthread_join/detach on the philo threads.

    // 6. Cleanup
    //    - Join all philosopher threads (pthread_join) if not using detach.
    //    - Destroy all mutexes (pthread_mutex_destroy)
    //    - Free all allocated memory.


Next Steps
✅ Implement the philosopher_routine in routine.c. Use your new print_message function and the logic outlined in your comments to handle the thinking, eating, and sleeping cycle.
/// *
Cycle order:
   * Philosopher thinks
   * Tries to eat (locks forks, prints messages, updates meal time and count, sleeps for eating)
   * Sleeps (prints message, sleeps for sleeping)
   * Thinks (prints message)
   * Repeats
Deadlock prevention:
   * Even/odd fork locking order is implemented.
Mutexes:
   * Forks are protected by mutexes.
   * Meal time and eat count are protected by meal_lock.
   * Printing is protected by write_lock (in print_message, assumed).
print_message usage:
   * Used for all state changes (fork, eat, sleep, think).
Simulation stop:
   * Checks prog->is_sim_running under write_lock to break the loop.
Small delay for odd philosophers:
   * usleep(1000) for odd IDs to stagger start.
/// *

Create a monitor. This is a crucial part. You need a loop in main (or a separate thread) that constantly checks if a philosopher has died or if everyone has eaten enough. If a condition is met, it should set prog.is_sim_running to 0 to stop the simulation.

Prevent Deadlocks. A common issue is that all philosophers grab their left fork simultaneously and wait forever. A simple solution is to have even-numbered philosophers pick up their left fork first, while odd-numbered philosophers pick up their right fork first.



Mutexes
Think of a mutex as a key to a single-person room.
- The Room: This is your *shared data or resource*. In our project, we have two kinds of "rooms":
    1. A physical fork on the table.
    2. The terminal screen where we print messages.

- The People: These are your *threads* (the philosophers).
- The Rule: Only one person can be in the room at a time. To get in, you need the key.

Here's how it works in our project:

1. A Philosopher Wants a Fork:
    - The philosopher (thread) goes to the fork they want.
    - They try to take the key for that fork (pthread_mutex_lock).
    - If the key is available, they take it, "locking the door." They now exclusively own that fork. No one else can touch it.
    - If another philosopher already has the key, they must wait in line until the key is returned. The program pauses that philosopher automatically.
2. A Philosopher is Done with a Fork:
    - When they are finished eating, they put the fork down.
    - This means they return the key (pthread_mutex_unlock).
    - The key is now available, and the next philosopher waiting in line can take it.

So, the mutex doesn't stop a philosopher from doing things. It just makes them wait their turn for a resource that cannot be shared. It enforces politeness and prevents two philosophers from trying to grab the exact same fork at the exact same time, which is physically impossible and would crash your program.

1. pthread_mutex_init() - Creating the Lock
Analogy: This is you, before anyone arrives, installing a brand new lock on a door and creating the one and only key for it.
"Formula": int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
How to Use It:
`pthread_mutex_t my_lock;`
`pthread_mutex_init(&my_lock, NULL);`
    *You give it the address (&) of your mutex variable.
    *The second argument is for special settings. Just use NULL for default behavior.
In Your Project: You call this in init_mutexes for every single fork and for the write_lock.

2. pthread_mutex_destroy() - Removing the Lock
Analogy: The party is over. You are uninstalling the lock from the door and throwing the key away.
"Formula": int pthread_mutex_destroy(pthread_mutex_t *mutex);
How to Use It:
`pthread_mutex_destroy(&my_lock);`
    *You just give it the address of the mutex you want to clean up.
In Your Project: You call this in cleanup for every mutex you created. It's the opposite of init.

3. pthread_mutex_lock() - Taking the Key
Analogy: This is a philosopher trying to take the key to enter a room (e.g., to grab a fork).
"Formula": int pthread_mutex_lock(pthread_mutex_t *mutex);
How to Use It:
`pthread_mutex_lock(&my_fork);` 
// Try to take the key for the fork
// ... critical code here, e.g., use the fork ...
    *Crucial Behavior: If the key is available, your code takes it and continues immediately. If another philosopher has the key, your code will pause on this line and wait until the key is returned.
In Your Project: You will use this right before a philosopher takes a fork or right before they call printf.

4. pthread_mutex_unlock() - Returning the Key
Analogy: The philosopher is finished, leaves the room, and puts the key back on the hook for the next person.
"Formula": int pthread_mutex_unlock(pthread_mutex_t *mutex);
How to Use It:
// ... finished using the fork ...
`pthread_mutex_unlock(&my_fork);` 
// Return the key
    *This releases the lock. If other philosophers were waiting for this key, one of them will now be able to take it and continue.
In Your Project: You will use this right after a philosopher puts a fork down or after they are finished printing.

Summary: The Lifecycle
It's a simple cycle:
- init (once at the start)
- lock (wait for your turn)
- Do your protected work (use the fork, print the message)
- unlock (let the next person go)
- Repeat steps 2-4 as needed.
- destroy (once at the very end)
