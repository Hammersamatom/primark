#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <cmath>
#include <future>


unsigned long threadCount = 0;

std::vector<std::future<double>> returns;

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

    return (double)localPrimes.size()/diffTime;
}



int main(int argc, char* argv[])
{
    if (argv[1] != nullptr)
    {
        threadCount = atoi(argv[1]);
    }

    

    returns.push_back(std::async(primes));
    double singleScore = returns[0].get();




    if (threadCount > 1)
    {
        returns.clear();
        
        for (int i = 0; i < threadCount; i++)
        {
	        returns.push_back(std::async(primes));
        }
    }


    double multiScore = 0;
    for (int i = 0; i < returns.size(); i++)
    {
	    multiScore += returns[i].get();
    }


    for (int i = 0; i < returns.size(); i++)
    {
	    multiScore += returns[i].get();
    }

    std::cout << "[Singular-thread Score] >> " << singleScore 							                               << "\n";
    std::cout << "[Multiple-thread Score] >> " << multiScore  							                               << "\n";
    std::cout << "[Ratio]                 >> " << 1 << ":" << threadCount * multiScore/(singleScore*threadCount)       << "\n";

    return 0;
}
