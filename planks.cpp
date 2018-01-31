#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

bool f1 (int i , int target, vector<int> &elements ) {
    if( target == 0) return true ;
    if (( i == 0 && target > 0) || target < 0) return false ;
    return f1 (i-1 , target -elements[i], elements) || f1 (i-1, target, elements);
}

int f2 (int i, int target, vector<int> elements ) {
    if(target == 0) return 1;
    if ((i == 0 && target > 0) || target < 0) return 0;

    int a = f2(i-1 , target-elements[i], elements);
    int b = f2(i-1, target, elements);;

    return max(a, b);
}


int build_line(int llength, vector<int> plank, map<int, int> memo)
{
    if(plank.size() == 1) {
        if(llength = *plank.begin()) return 1;
        else return 0;
    }

    int idx = plank.size()-1;
    int here = plank[idx];

    if(f1(idx-1, llength-here, plank)) {
        cerr << "can build sum for " << here << " to " << llength << endl;
    }

    return 1;
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<int> plank(n);
        list<int> plist;

        int planksum = 0;

        for(int i=0; i < n; i++) {
            cin >> plank[i];
            planksum += plank[i];
        }
        sort(plank.begin(), plank.end(), std::greater<int>());

//         for(int i=0; i < n; i++) {
//             plist.push_front(plank[i]);
// //            cerr << plank[i] << endl;
//         }

        int llength = planksum/4;

        map<int, int> memo;

        if(*plank.begin() > llength) {
            cout << 0 << endl;
            continue;
        } else {
            cout << build_line(llength, plank, memo) << endl;
        }


    }


}
