#ifndef RT_PROG_BAR_HPP
#define RT_PROG_BAR_HPP

#include <chrono>
#include <mutex>
#include <utility>

#include "fmt/core.h"
#include "fmt/color.h"

class ProgressBar {
public:
    explicit ProgressBar(std::string name, int size): total(size), progress(0), name(std::move(name)) {
        print_interval = size / 2000;
        start_time = std::chrono::high_resolution_clock::now();
    };

    void Step(size_t s = 1) {
        std::unique_lock<std::mutex> lk(mtx);

        progress += s;
        if (progress % print_interval == 0 || progress == total) {
            float progress_proportion = (float) progress / (float) total;
            auto time_passed = std::chrono::high_resolution_clock::now() - start_time;
            size_t passed = std::chrono::duration_cast<std::chrono::seconds>(time_passed).count();
            size_t eta = std::chrono::duration_cast<std::chrono::seconds>(time_passed / progress_proportion - time_passed).count();
            fmt::print(fg(fmt::color::aqua), "\r{}: {:.2f}% ({}/{}) completed after {} secs, eta: {} secs", name, progress_proportion * 100, progress, total, passed, eta);
            if (progress == total) {
                fmt::print("\n");
            }
        }
    }

    void Reset() {
        progress = 0;
        start_time = std::chrono::high_resolution_clock::now();
    }

private:
    std::mutex mtx;
    size_t total;
    size_t progress;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    size_t print_interval;
    std::string name;
};

#endif //RT_PROG_BAR_HPP
