#include <ctime> 
#include <vector>
#include <stdio.h>
#include <math.h>

int main()
{
    std::vector<unsigned long> primes;
    clock_t startTicks = clock();
    ulong n = 0;

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
        
        if (flag == true)
        {
            primes.push_back(n);
        }

        n++;

        // Reset clock to stop while loop
        startTicks = clock() - startTicks;
    }

    printf("Final score: %.2lf\n", primes.size()/(((float)startTicks/CLOCKS_PER_SEC)*100));

    return 0;
}
