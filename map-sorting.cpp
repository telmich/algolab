#include <iostream>
#include <map>

using namespace std;

int main()
{

    map<int, int> testnormal;

    testnormal[1] = 2;
    testnormal[2] = 5;
    testnormal[3] = 7;
    testnormal[12] = 9;

    for(auto ai=testnormal.begin(); ai != testnormal.end(); ai++) {
        cout << ai->first << " " << ai->second << endl;
    }

    auto ai2 = testnormal.lower_bound(4);
    cout << "LB4: " << ai2->first << endl;

    map<int, int, greater<int>> testgreater;
    testgreater[1] = 2;
    testgreater[2] = 5;
    testgreater[3] = 7;
    testgreater[12] = 9;
    for(auto ai=testgreater.begin(); ai != testgreater.end(); ai++) {
        cout << ai->first << " " << ai->second << endl;
    }

    auto ai3 = testgreater.lower_bound(4);
    cout << "LB4>: " << ai3->first << endl;



}
