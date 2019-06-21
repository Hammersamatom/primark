#include <ctime>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <algorithm>

std::mutex mylock;
std::vector<std::thread> threads;
std::vector<double> multiPrimes;
double singlePrime = 0;

void primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::high_resolution_clock::now();
    double diffTime = 0;
    unsigned long n = 0;

    while (diffTime < 6000)
    {
        bool flag = true;

        for (unsigned long i = 2; i <= sqrtl(n); i++)
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

    //printf("Final score: %.2lf\n", localPrimes.size()/(((float)startTicks/CLOCKS_PER_SEC)*100));

    mylock.lock();

    multiPrimes.push_back((double)localPrimes.size()/6000);

    mylock.unlock();

}

void join_all(std::vector<std::thread>& threadVect)
{
    // I thought this would be simpler than the example I found. Condensed into one function.
    for (int i = 0; i < threadVect.size(); i++)
    {
        threadVect[i].join();
    }
}

int main()
{
    threads.push_back(std::thread(primes));
    join_all(threads);

    singlePrime = multiPrimes[0];

    threads.clear();
    multiPrimes.clear();


    for (int i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        threads.push_back(std::thread(primes));
    }
    join_all(threads);

    double projectedTotal = singlePrime * std::thread::hardware_concurrency();
    double actualTotal = 0;

    for (int i = 0; i < multiPrimes.size(); i++)
    {
        actualTotal += multiPrimes[i]; 
    }


    printf("Single-Core Score: %lf\n", singlePrime);
    printf("Projected Multi-Core Score: %lf\n", singlePrime*std::thread::hardware_concurrency());
    printf("Average Multi-Core Score: %lf\n", actualTotal/std::thread::hardware_concurrency());
    printf("Actual Multi-Core Score: %lf\n", actualTotal);

    return 0;
}
