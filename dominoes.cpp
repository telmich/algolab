#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<int> height(n);

        for(int i=0; i< n; i++) {
            cin >> height[i];
        }
        int fallsuntil = 0;

        for(int i=0; i < n; i++) {
            if(fallsuntil < i) break;

            int thisfallsuntil = i+height[i] -1;

            if(thisfallsuntil > fallsuntil)
                fallsuntil = thisfallsuntil;
        }
        if(fallsuntil > n)
            fallsuntil = n-1;

        cout << (fallsuntil+1) << endl;

    }


}
