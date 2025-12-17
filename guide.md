🎓 Evaluation Prep: The Walkthrough
Here is how to explain your project during the evaluation, file by file.

1. main.c (The Boss)
What it does: It's the control center.
Flow:
init_program: Sets up the table (memory, mutexes).
pthread_create: Spawns the philosophers (threads). They immediately jump into philosopher_routine.
monitor_simulation: The main thread turns into the "Grim Reaper." It loops infinitely, checking if anyone died.
cleanup: When the simulation ends, it joins threads and destroys mutexes.
2. philo.c (The Setup)
init_program:
Parses arguments (ft_atoi).
Crucial: Initializes write_lock. This prevents log messages from getting jumbled (scrambled text).
init_philosophers:
Assigns forks.
The Fork Trick: left_fork is forks[i], right_fork is forks[(i + 1) % num]. This creates the circle! ⭕
3. routine.c (The Life Cycle)
philosopher_routine: The entry point. Handles the special case of 1 Philosopher (who takes one fork and dies sadly).
eat function (The most important part):
Deadlock Prevention: You use the Even/Odd strategy.
Even ID: Takes Left fork, then Right fork.
Odd ID: Takes Right fork, then Left fork.
Why? If everyone takes the Left fork at the exact same time, they all wait forever (Deadlock). By switching the order for half of them, someone will always be able to eat.
Locking: You lock meal_lock before updating last_meal_time. This prevents the Monitor from reading the time while you are writing it (Data Race).
4. monitor.c (inside main.c) - The Grim Reaper 💀
What it does: It's an infinite loop running in the main thread.
The Check:
Calculates: Current Time - Last Meal Time.
If result > time_to_die, it prints "died" and sets is_sim_running = 0.
Safety: It uses meal_lock to read the meal time safely.
5. utils.c (The Clock)
get_time: Converts the system time (seconds + microseconds) into pure milliseconds.
ft_usleep:
Why not standard usleep? Standard usleep is lazy and inaccurate.
Your version: Loops and checks time repeatedly. It also checks is_simulation_running inside the loop so threads stop immediately if someone dies, rather than finishing their sleep.
🧠 Key Concepts to Mention (Buzzwords)
Mutex: A "key" to a door. Only one thread can hold the key. Used for forks and writing to the screen.
Data Race: When two threads try to read/write the same variable at the same time. You fixed this with meal_lock and write_lock.
Deadlock: When everyone is waiting for a resource held by someone else who is also waiting. You fixed this with the Even/Odd fork grab order.
Context Switch: The CPU switching between threads. This is why usleep is inaccurate and why we
need ft_usleep.


int ft_usleep(size_t milliseconds, t_program *prog)
{
    size_t start;

    // 1. Mark the start time
    // We take a snapshot of the current time so we know when we started waiting.
    start = get_time();

    // 2. The Loop (The "Are we there yet?" check)
    // We keep looping as long as the time passed (current - start) is LESS than the requested sleep time.
    while ((get_time() - start) < milliseconds)
    {
        // 3. The Safety Check
        // Inside the loop, we constantly check: "Is the simulation still running?"
        // If someone died (is_sim_running becomes 0), we return 0 immediately.
        // This stops the philosopher from sleeping for 200ms if the simulation ended 1ms ago.
        if (!is_simulation_running(prog))
            return (0);
        
        // 4. The Micro-Nap
        // We sleep for a tiny amount (500 microseconds = 0.5 milliseconds).
        // This prevents the CPU from working too hard (spinning) while waiting.
        // It's like closing your eyes for a split second, then checking the clock again.
        usleep(500); 
    }
    // 5. Done!
    // If the loop finishes naturally, it means we slept for the full duration.
    return (1);
}

⚡️ Why do we need this?
If you use normal usleep(200000) (200ms):

Scenario: A philosopher dies at millisecond 10.
Problem: The thread sleeping for 200ms will keep sleeping until millisecond 200, ignoring the death. The simulation won't stop instantly.
Your Fix: ft_usleep checks for death every 0.5ms, so it reacts

// виписати мутекси птреди
