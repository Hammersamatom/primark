#include <ctime>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <algorithm>

unsigned long threadCount = 0;

std::mutex mylock;
std::vector<std::thread> threads;
std::vector<double> multiPrimes;

void primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::high_resolution_clock::now();
    double diffTime = 0;
    unsigned long n = 0;
    //unsigned long correct = 0;

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
            //correct++;
        }

        n++;

        // Reset clock to stop while loop
        diffTime = std::chrono::duration<double, std::centi>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    mylock.lock();


    multiPrimes.push_back((double)localPrimes.size()/diffTime);
    //multiPrimes.push_back(correct/diffTime);
    //multiPrimes.push_back(localPrimes.size());

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
    if (argv[1] == NULL)
    {
        threadCount = std::thread::hardware_concurrency();
    }
    else
    {
        threadCount = atoi(argv[1]);
    }


    for (int i = 0; i < threadCount; i++)
    {
        threads.push_back(std::thread(primes));
    }
    join_all(threads);

    double actualTotal = 0;

    for (int i = 0; i < multiPrimes.size(); i++)
    {
        actualTotal += multiPrimes[i]; 
    }

    for (int i = 0; i < multiPrimes.size(); i++)
    {
        printf("Thread %i: %lf\n", i+1, multiPrimes[i]);
    }

    printf("Average Multi-Core Score: %lf\n", actualTotal/threadCount);
    printf("Actual Multi-Core Score: %lf\n", actualTotal);

    return 0;
}
