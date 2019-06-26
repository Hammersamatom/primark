#include <chrono>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <future>

unsigned long threadCount = 0;

//std::mutex mylock;
//std::vector<std::thread> threads;
std::vector<std::future<double>> returns;
//std::vector<double> multiPrimes;

double primes()
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

    //mylock.lock();
    //multiPrimes.push_back((double)localPrimes.size()/diffTime);
    //mylock.unlock();

    return (double)localPrimes.size()/diffTime;
}

void join_all(std::vector<std::thread>& threadVect)
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
	returns.push_back(std::async(primes));
        //threads.push_back(std::thread(primes));
    }
    //join_all(threads);

    double actualTotal = 0;

    //for (int i = 0; i < multiPrimes.size(); i++)
    //{
    //    actualTotal += multiPrimes[i]; 
    //}

    for (int i = 0; i < returns.size(); i++)
    {
	actualTotal += returns[i].get();
    }

    printf("Average Multi-Core Score: %lf\n", actualTotal/threadCount);
    printf("Actual Multi-Core Score: %lf\n", actualTotal);

    return 0;
}
