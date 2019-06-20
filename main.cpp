#include <ctime> 
#include <vector>
#include <stdio.h>
#include <math.h>

void primes()
{
    std::vector<unsigned long> localPrimes;
    clock_t startTicks = clock();
    unsigned long n = 0;

    while ((float)startTicks/CLOCKS_PER_SEC < 60)
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
        startTicks = clock() - startTicks;
    }

    printf("Final score: %.2lf\n", localPrimes.size()/(((float)startTicks/CLOCKS_PER_SEC)*100));
}

int main()
{
    primes();
    return 0;
}
