#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        int m;
        cin >> m;

        double sum = 0;
        double input;

        while(m--) {
            cin >> input;

            sum += input;
        }
        cout << sum << endl;
    }

}
