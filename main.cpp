#include <ctime>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <algorithm>


std::vector<std::thread> threads;
std::vector<double> globalPrimes;
std::mutex mylock;

void primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto diffTime = 0;
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

    globalPrimes.push_back((double)localPrimes.size()/6000);

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
    for (int i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        threads.push_back(std::thread(primes));
    }
    
    join_all(threads);

    std::sort(globalPrimes.begin(),globalPrimes.end());

    for (int i = 0; i < globalPrimes.size(); i++)
        printf("%lf\n", globalPrimes[i]);

    return 0;
}
