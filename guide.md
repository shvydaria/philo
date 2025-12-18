---

## Key Terms Used in the Project

- **Mutex (Mutual Exclusion):** A lock that ensures only **one thread** can access a shared resource (e.g., a fork, logging, or meal data) at a time. It's like a "private key" to prevent conflicts.

- **Thread:** A lightweight unit of execution within a program. In your project, each thread represents a **philosopher**, carrying out actions independently (thinking, eating, sleeping).

- **Critical Section:** A part of the code where shared resources are accessed. Mutexes protect these areas to avoid data races.

- **Deadlock:** A situation where threads are stuck waiting for resources (e.g., forks), resulting in a **standstill**. You solved this by using an **even-odd fork-taking strategy**.

- **Data Race:** When multiple threads access the same variable at the same time, and at least one is modifying it without synchronization. Mutexes like `meal_lock` and `write_lock` prevent this.

- **Even-Odd Method:** A strategy where philosophers with even IDs pick up forks in one order (Left, then Right), and philosophers with odd IDs pick them up in the reverse order. This avoids deadlocks and keeps the system moving.

- **Monitor Thread:** The "Grim Reaper" in your program, this checks if any philosopher has **starved** by calculating the time since their last meal.

- **Simulation State (is_simulation_running):** A shared global variable used to stop philosophers and the monitor thread safely once the simulation is done.

---