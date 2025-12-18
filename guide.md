# 🎓 The Walkthrough

---

## 1. **main.c (The Boss)**

### What it does:
- It's the **control center**.

### Flow:
1. **`init_program`:** Sets up the table (memory, mutexes).
2. **`pthread_create`:** Spawns the philosophers (threads). They immediately jump into `philosopher_routine`.
3. **`monitor_simulation`:** The main thread turns into the "Grim Reaper." It loops infinitely, checking if anyone died.
4. **`cleanup`:** When the simulation ends, it joins threads and destroys mutexes.

---

## 2. **philo.c (The Setup)**

### What it does:

1. **`init_program`:**
   - Parses arguments (`ft_atoi`).
   - Crucial: Initializes `write_lock`. This prevents log messages from getting jumbled (scrambled text).
2. **`init_philosophers`:**
   - Assigns forks.
   - **The Fork Trick:**
     - `left_fork = forks[i]`
     - `right_fork = forks[(i + 1) % num]`
     - This creates the **circle!** ⭕

---

## 3. **routine.c (The Life Cycle)**

### What it does:

1. **`philosopher_routine`:** The entry point. Handles the special case of 1 Philosopher (who takes one fork and dies sadly).

2. **`eat` function (The most important part):**
   - **Deadlock Prevention:** You use the **Even/Odd Strategy**:
     - Even ID: Takes Left fork, then Right fork.
     - Odd ID: Takes Right fork, then Left fork.
   - **Why?**
     - If everyone takes the **Left fork** at the exact same time, they all wait forever (**Deadlock**).
     - By switching the order for half of them, someone will always be able to eat.
   - **Locking:**
     - Lock `meal_lock` before updating `last_meal_time`.
     - This prevents the Monitor from reading the time while you are writing it (**Data Race**).

---

## 4. **monitor.c (inside main.c) - The Grim Reaper 💀**

### What it does:

- It's an **infinite loop** running in the main thread.

### The Check:
- Calculates: `Current Time - Last Meal Time`.
- If the result > `time_to_die`, it:
  1. Prints "died"
  2. Sets `is_sim_running = 0`.
- **Safety:** It uses `meal_lock` to read the meal time safely.

---

## 5. **utils.c (The Clock)**

### Key Functions:

1. **`get_time`:** Converts the system time (**seconds + microseconds**) into pure milliseconds.
2. **`ft_usleep`:**
   - Why not standard `usleep`?
     - Standard usleep is lazy and **inaccurate**.
   - Your version:
     1. Loops and checks time repeatedly.
     2. Also checks `is_simulation_running` inside the loop so threads stop immediately if someone dies, rather than finishing their sleep.

---

## 🧠 Key Concepts to Mention (Buzzwords)

### **Mutex**
- A "key" to a door. Only one thread can hold the key. Used for forks and writing to the screen.

### **Data Race**
- When two threads try to read/write the same variable at the same time. You fixed this with `meal_lock` and `write_lock`.

### **Deadlock**
- When everyone is waiting for a resource held by someone else who is also waiting. You fixed this with the **Even/Odd fork grab order**.

### **Context Switch**
- The CPU switching between threads. This is why `usleep` is inaccurate and why we need `ft_usleep`.

---

## 🔒 Introduce mutexes as table rules:

> "To solve this, I use something called a **mutex**. Think of a mutex as an agreed-upon set of table rules: a philosopher can only pick up a fork if the rule says it's available. This prevents arguments or two philosophers grabbing the same fork at once."

## 🧵 Describe pthreads as the philosophers in action:

> "Now, to bring this all to life, I use something called **pthreads**. These are like the brains of the philosophers—threads help each one figure out when they should think, grab forks, eat, and put things back down."

## 🔄 Dive into the even-odd method analogy:

> "Here’s how I manage ‘who gets to eat when.’ I use something called the even-odd method. Think of it like this: if you’re sitting at the table, people with even-numbered seats eat in one round, while odd-numbered seats wait. Then, we switch—odd numbers eat while even numbers take a break. This back-and-forth ensures no one gets left out."

## 🤝 Why this system works:

> "The magic of this method is that we’re enforcing fairness while avoiding deadlock (when everyone’s waiting and no one’s eating) or starving (when someone never gets a chance to eat). Everyone plays by the rules and takes turns—and in the end, all the philosophers get fed!"

## 👨‍💻 Wrap it up with the big picture:

> "This project isn’t just about dining philosophers—it’s an example of how computers manage complicated tasks when multiple programs want the same resources. By solving these kinds of challenges, we make sure systems run efficiently and without errors."

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
