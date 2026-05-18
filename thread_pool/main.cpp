#include "thread_pool.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

// Simulates a CPU-bound task
static int slow_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    return x * x;
}

// ---- Demo helpers -------------------------------------------------------

static void demo_futures(ThreadPool& pool) {
    std::cout << "\n[1] Parallel squares (1..8) using futures:\n";

    std::vector<std::future<int>> futs;
    futs.reserve(8);
    for (int i = 1; i <= 8; ++i)
        futs.emplace_back(pool.enqueue(slow_square, i));

    for (int i = 0; i < 8; ++i)
        std::cout << "    " << (i + 1) << "^2 = " << futs[i].get() << '\n';
}

static void demo_parallel_reduce(ThreadPool& pool) {
    std::cout << "\n[2] Parallel sum of 1..100 (4 chunks):\n";

    constexpr int N = 100;
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 1);  // 1, 2, ..., 100

    constexpr int num_chunks = 4;
    const int chunk_size = N / num_chunks;

    std::vector<std::future<long long>> chunk_sums;
    chunk_sums.reserve(num_chunks);

    for (int c = 0; c < num_chunks; ++c) {
        int lo = c * chunk_size;
        int hi = (c == num_chunks - 1) ? N : lo + chunk_size;
        chunk_sums.emplace_back(pool.enqueue([&data, lo, hi] {
            return std::accumulate(data.begin() + lo, data.begin() + hi, 0LL);
        }));
    }

    long long total = 0;
    for (auto& f : chunk_sums)
        total += f.get();

    std::cout << "    Result = " << total << "  (expected 5050)\n";
}

static void demo_fire_and_forget(ThreadPool& pool) {
    std::cout << "\n[3] Fire-and-forget tasks (thread IDs):\n";

    std::mutex print_mtx;
    std::vector<std::future<void>> futs;
    futs.reserve(6);

    for (int i = 0; i < 6; ++i) {
        futs.emplace_back(pool.enqueue([i, &print_mtx] {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            std::lock_guard<std::mutex> lg(print_mtx);
            std::cout << "    task " << i
                      << " ran on thread " << std::this_thread::get_id() << '\n';
        }));
    }

    for (auto& f : futs)
        f.get();  // wait so output is flushed before main exits
}

// -------------------------------------------------------------------------

int main() {
    const std::size_t hw = std::thread::hardware_concurrency();
    std::cout << "Hardware threads available : " << hw << '\n';
    std::cout << "Spawning pool with         : " << hw << " workers\n";

    ThreadPool pool(hw);

    demo_futures(pool);
    demo_parallel_reduce(pool);
    demo_fire_and_forget(pool);

    std::cout << "\nPool destructor called — all workers joined cleanly.\n";
}
