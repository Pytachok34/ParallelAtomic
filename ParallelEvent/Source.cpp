#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <Windows.h>
#include <atomic>

const size_t NTHREAD = 3;
const size_t n = 10;
std::atomic<bool> result = true;

bool non_parallel_check(std::vector<int>& a)
{
    bool flag = true;
    size_t i(0);
    size_t size = a.size() - 1;
    while (i < size && flag)
    {
        if ((a[i] < 0 && a[i + 1] < 0) || (a[i] >= 0 && a[i + 1] >= 0))
            flag = false;
        ++i;
    }
    return flag;
}

void check(std::vector<int>& arg, int i, int size)
{

    while (i < size - 1&& result)
    {
        if ((arg[i] < 0 && arg[i + 1] < 0 || arg[i] >= 0 && arg[i + 1] >= 0))
            result = false;
        ++i;
    }
}

bool parallel_check(std::vector<int>& a)
{
    std::thread t[NTHREAD];
    size_t N = n / (NTHREAD+1);
    for (size_t i = 0; i < NTHREAD; ++i)
    {
            t[i] = std::thread(check, std::ref(a), i * N, (i + 1) * N + 1);
    }
    check(a, (NTHREAD - 1) * N, n);
    for (size_t i = 0; i < NTHREAD; ++i)
        t[i].join();
    return result;
}

int main()
{
    std::vector<int> vec = { -2,3,-1,5,-2,7,9,3,-10,-9 };
    if (non_parallel_check(vec))
        std::cout << "Yes \n";
    else
        std::cout << "No \n";
    if (parallel_check(vec))
        std::cout << "Yes \n";
    else
        std::cout << "No \n";
    std::cin.get();
    return 0;
}