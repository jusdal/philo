# philo

![C](https://img.shields.io/badge/language-C-blue) ![42 Network](https://img.shields.io/badge/school-42%20Network-black)

A C implementation of the classic **Dining Philosophers problem** — a foundational concurrency exercise used in computer science to illustrate the challenges of resource sharing, deadlock prevention, and thread synchronization.

## The problem

N philosophers sit at a table. Each needs two forks to eat, but there is only one fork between each pair. The challenge: let every philosopher eat without any of them starving, and without the program deadlocking or producing data races.

## Implementation

- Each philosopher runs as a separate **POSIX thread** (`pthread`)
- Forks are represented as **mutexes** — only one philosopher can hold a fork at a time
- A monitor thread watches for starvation conditions and terminates the simulation cleanly
- Precise timing via `gettimeofday()` to track meal intervals and death thresholds

## Skills demonstrated

- **Thread lifecycle management:** creation, synchronization, and clean teardown
- **Mutex usage:** avoiding race conditions on shared resources
- **Deadlock prevention:** fork acquisition ordering and timing logic
- **Failure mode thinking:** designing for edge cases (1 philosopher, very short time-to-die values)

## Usage

```bash
make
./philo [num_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [meals_required]

# Example: 5 philosophers, die in 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200
```

## Context

Built as part of the 42 Network common core curriculum. No external threading libraries — only POSIX threads and standard C. The constraint of working without high-level abstractions makes the concurrency challenges explicit and unavoidable.
