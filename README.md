# C++ Thread Pool

A lightweight, header-only thread pool implemented in modern C++17.

## Features

- Fixed-size pool of worker threads
- Lock-free task dispatch via `std::condition_variable`
- `enqueue()` returns a `std::future<T>` for any return type
- RAII — destructor joins all workers cleanly (no leaked threads)
- Header-only: drop `thread_pool.hpp` into any project

## API

```cpp
ThreadPool pool(N);                          // N worker threads

// Submit a task — returns std::future<R>
auto fut = pool.enqueue(callable, args...);
R result = fut.get();                        // block until done

pool.size();                                 // number of workers
```

## Build

```bash
cmake -S . -B build
cmake --build build
./build/thread_pool_demo        # Linux/macOS
build\Debug\thread_pool_demo    # Windows
```

Requires: C++17, CMake 3.14+

## Demo output

```
Hardware threads available : 8
Spawning pool with         : 8 workers

[1] Parallel squares (1..8) using futures:
    1^2 = 1
    2^2 = 4
    ...

[2] Parallel sum of 1..100 (4 chunks):
    Result = 5050  (expected 5050)

[3] Fire-and-forget tasks (thread IDs):
    task 0 ran on thread 140234...
    ...

Pool destructor called — all workers joined cleanly.
```
