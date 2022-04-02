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

unsigned long centiLength = 3000;


//TODO: Comment what this does.
void primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::steady_clock::now();
    double diffTime = 0;
    unsigned long n = 0;
    unsigned long s = 0;

    while (diffTime < centiLength)
    {
        bool flag = true;

        for (unsigned long i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            s++;
            //localPrimes.push_back(n);
        }

        n++;

        // Reset clock to stop while loop
        diffTime = std::chrono::duration<double, std::centi>(std::chrono::steady_clock::now() - startTime).count();
    }

    mylock.lock();
    //multiPrimes.push_back((double)localPrimes.size()/diffTime);
    multiPrimes.push_back((double)s/diffTime);
    mylock.unlock();
}


//TODO: Comment what this does.
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
    // Checking for valid threadCount argument.
    if (argv[1] != nullptr)
    {
        threadCount = atoi(argv[1]);
        if (threadCount < systemThreadTotal || threadCount == 0)
        {
            threadCount = systemThreadTotal;
        }
    }

    std::cout << "== Starting Single thread run" << std::endl;

    // This is for the single-thread score.
    threads.push_back(std::thread(primes));
    join_all(threads);

    double singleScore = multiPrimes[0];
    
    std::cout << "== Starting Multiple thread run" << std::endl;

    // Skip running a single thread twice.
    if (threadCount > 1)
    {
        threads.clear();
        multiPrimes.clear();

        for (int i = 0; i < threadCount; i++)
        {
            threads.push_back(std::thread(primes));
        }
        join_all(threads);
    }

    // Multi-thread score total.
    double multiScore = 0;
    for (int i = 0; i < multiPrimes.size(); i++)
    {
        multiScore += multiPrimes[i]; 
    }


    std::cout << "====================================================="                                                << std::endl;
    std::cout << "== [Single-thread Score] >> " << singleScore                                                          << std::endl;
    std::cout << "== [Multi-thread  Score] >> " << multiScore                                                           << std::endl;
    std::cout << "== [Ratio]               >> " << 1 << ":" << threadCount * multiScore/(singleScore*threadCount)       << std::endl;

    return 0;
}
