#include <chrono>
#include <iostream>

class ScopedTimer
{
private:
    using Clock = std::chrono::steady_clock;

public:
    explicit ScopedTimer(std::string_view name)
        : name_{ name }, start_{ Clock::now() }
    {
    }

    ~ScopedTimer()
    {
        auto end = Clock::now();
        auto elapsed = std::chrono::duration<double, std::milli>(end - start_).count();
        std::cout << name_ << ": " << elapsed << "ms\n";
    }

private:
    std::string name_;
    std::chrono::time_point<Clock> start_;
};