#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <cmath>
#include <thread>
#include <future>

unsigned long systemThreadTotal = std::thread::hardware_concurrency();
unsigned long threadCount = systemThreadTotal;
unsigned long centiLength = 3000;

std::vector<std::future<double>> returns;

double primes()
{
    std::vector<unsigned long> localPrimes;
    auto startTime = std::chrono::high_resolution_clock::now();
    double diffTime = 0;
    unsigned long n = 0;

    while (diffTime < centiLength)
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

    return (double)localPrimes.size()/diffTime;
}



int main(int argc, char* argv[])
{
    double singleScore = 0;
    double multiScore = 0;

    if (argv[1] != nullptr)
    {
        threadCount = atoi(argv[1]);
    }
    
    std::cout << "== Starting Single thread run" << std::endl;
    returns.push_back(std::async(primes));
    singleScore = returns[0].get();


    std::cout << "== Starting Multiple thread run" << std::endl;
    if (threadCount > 1)
    {
        returns.clear();
        
        for (int i = 0; i < threadCount; i++)
        {
	        returns.push_back(std::async(primes));
        }

        for (int i = 0; i < returns.size(); i++)
        {
	        multiScore += returns[i].get();
        }
    }
    else
    {
        multiScore = singleScore;
    }

    std::cout << "[Singular-thread Score] >> " << singleScore 							                               << "\n";
    std::cout << "[Multiple-thread Score] >> " << multiScore  							                               << "\n";
    std::cout << "[Ratio]                 >> " << 1 << ":" << multiScore/singleScore                                   << "\n";

    return 0;
}
