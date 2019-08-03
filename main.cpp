#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <algorithm>

unsigned long systemThreadTotal = std::thread::hardware_concurrency();
unsigned long threadCount = systemThreadTotal;

std::mutex mylock;
std::vector<std::thread> threads;
std::vector<double> multiPrimes;



void primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::high_resolution_clock::now();
    double diffTime = 0;
    unsigned long n = 0;

    while (diffTime < 3000)
    {
        bool flag = true;

        for (unsigned long i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                flag = !flag;
                break;
            }
        }

        if (flag)
        {
            localPrimes.push_back(n);
        }

        n++;

        // Reset clock to stop while loop
        diffTime = std::chrono::duration<double, std::centi>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    mylock.lock();
    multiPrimes.push_back((double)localPrimes.size()/diffTime);
    mylock.unlock();
}

void join_all(std::vector<std::thread> &threadVect)
{
    // I thought this would be simpler than the example I found. Condensed into one function.
    for (int i = 0; i < threadVect.size(); i++)
    {
        threadVect[i].join();
    }
}

int main(int argc, char* argv[])
{
    // Checking for threadCount argument.
    if (argv[1] != nullptr)
    {
        threadCount = atoi(argv[1]);
        if (threadCount > systemThreadTotal)
        {
            threadCount = systemThreadTotal;
        }
    }



    threads.push_back(std::thread(primes));
    join_all(threads);

    double singleScore = multiPrimes[0];

    threads.clear();
    multiPrimes.clear();



    for (int i = 0; i < threadCount; i++)
    {
        threads.push_back(std::thread(primes));
    }
    join_all(threads);

    double  multiScore = 0;
    for (int i = 0; i < multiPrimes.size(); i++)
    {
        multiScore += multiPrimes[i]; 
    }



    std::cout << "Single-thread Score:        " << singleScore                                                          << "\n";
    //std::cout << "Average Multi-thread Score: " <<    multiScore/threadCount << "\n";
    std::cout << "Multi-thread Score:         " <<  multiScore                                                          << "\n";

    std::cout << "Ratio:                      " << 1 << ":" << threadCount * multiScore/(singleScore*threadCount)       << "\n";

    return 0;
}
