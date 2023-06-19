// The positive integers, x, y, and z, are consecutive terms of an arithmetic progression.
// Given that n is a positive integer, the equation:
//      x^2 - y^2 - z^2 = n
// has exactly one solution when n=20:
//      13^2 - 10^2 - 7^2 = 20
//
// In fact there are twenty-five values of n below one hundred for which the equation
// has a unique solution.
//
// How many values of n less than fifty million have exactly one solution?


#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

using namespace std::chrono_literals;

using Counts = std::vector<uint64_t>;

struct ThreadParams {
    const int64_t max_n;
    const int64_t thread_index;
    const int64_t num_threads;
    Counts &counts;
};

void thread_func(const ThreadParams& params) {
    std::cout << "\tthread_func, thread id = " << std::this_thread::get_id()
              << ", max_n = " << params.max_n
              << ", thread_index = " << params.thread_index
              << std::endl;

    for (int64_t k = 1 + params.thread_index; k < params.max_n; k += params.num_threads) {
        if (k % 1000 == 0)
            std::cout << k << "\t" << std::this_thread::get_id() << std::endl;
        for (int64_t y = k + 1; y < params.max_n; ++y) {
            const int64_t n = 4 * k * y - y * y;
            if ((n >= 0) && (n < params.max_n))
                ++params.counts[n];
        }
    }
}


Counts get_counts(int64_t max_n) {
    constexpr int64_t kNumThreads{ 10 };

    std::array<std::thread, kNumThreads> threads;
    std::array<Counts, kNumThreads> all_counts;

    for (int64_t ind = 0; ind < kNumThreads; ++ind) {
        all_counts[ind].resize(max_n + 1, 0);

        ThreadParams params{ .max_n = max_n,
                             .thread_index = ind,
                             .num_threads = kNumThreads,
                             .counts = all_counts[ind]};
        threads[ind] = std::thread(thread_func, params);
        std::this_thread::sleep_for(5ms);
    }

    for (auto& th : threads)
        th.join();

    Counts ret(max_n, 0);
    for (size_t ind = 0; ind < kNumThreads; ++ind) {
        for (int64_t n = 0; n < max_n; ++n)
            ret[n] += all_counts[ind][n];
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto counts = get_counts(100);
    //    size_t num{ 0 };
    //    for (size_t n = 0; n < counts.size(); ++n) {
    //        if (counts[n] == 1) {
    //            std::cout << n << "\t" << counts[n] << std::endl;
    //            ++num;
    //        }
    //    }
    //    std::cout << "There are " << num << " values of n with exactly 1 sequences." << std::endl;
    //}

    //{
    //    auto counts = get_counts(1'000'000);
    //    size_t num{ 0 };
    //    for (size_t n = 0; n < counts.size(); ++n) {
    //        if (counts[n] == 10) {
    //            std::cout << n << "\t" << counts[n] << std::endl;
    //            ++num;
    //        }
    //    }
    //    std::cout << "There are " << num << " values of n with exactly 10 sequences." << std::endl;
    //}

    {
        auto counts = get_counts(50'000'000);
        size_t num{ 0 };
        for (size_t n = 0; n < counts.size(); ++n) {
            if (counts[n] == 1) {
                std::cout << n << "\t" << counts[n] << std::endl;
                ++num;
            }
        }
        std::cout << "There are " << num << " values of n with exactly 1 sequences." << std::endl;
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << ctime(&timenow);
    }
}
