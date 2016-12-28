#include <iostream>

using namespace std;

int main()
{

    int testcases;
    cin >> testcases;

    while(testcases--) {
        int n;

        cin >> n;

        int numbers[n];
        int num;
        int zeros = 0;
        int ones = 0;
        int pairs;

        for(int i=0; i <n; i++) {
            cin >> num;
            if(num == 0) zeros++;
            if(num == 1) ones++;
        }
        cerr << "0: " << zeros << " 1:" << ones << "\n";

        // pairs = zeros + (zeros*(zeros+1))/2 + (((ones-1)*ones)/2);
        pairs = (zeros*(zeros+1))/2 + (((ones-1)*ones)/2);


        cout << pairs << "\n";
    }
}
