#include <iostream>

using namespace std;

/*
   - List of coins
   - Can take from front or back
   - Find our largest *guaranteed* amount (=> lower bound)

   - Input: 1000 coins
   - Naive version: sounds exponential -> 2^10 -> works 2^100 -> fail
     - Build every possible tree
     - take left -> branch out for right and left of the choice of opponent
     - continue until empty
     - remember all lower (?) outcomes?
     - take highest lowest outcome?

   - Remembering and not recalculating paths
     - good enough?

   - 1000^3 is too big -> general case
    - O(n) ok
    - O(n log n) ok
    - O(n^2) ok

   - Ideas:
     - length based approach?
       - n + n-1 + n-2 ... => O(n^2)
       - runtime would be ok
     - length1: we take the highest
     - length2: we start: we can still take the highest and be done
     - length3: things become more interesting

   - different actions for who's turn it is?
    - odd turn is always ours
    - every time taking the best
    - but only in odd case we add it to our sum

*/

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        int coin[n];
        cin >> n;

        for(int i=0; i < n; i++) {
            cin >> coin[i];
        }

    }
}
