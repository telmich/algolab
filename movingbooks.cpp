#include <iostream>
#include <set>

using namespace std;


int main()
{

    ios_base::sync_with_stdio(false);


    int t;

    cin >> t;

    while(t--) {
        int m, n;
        int rounds;

        cin >> n >> m;

        multiset<int, greater<int> > strength;
        multiset<int, greater<int> > weight;

        for(int i=0; i<n; i++) {
            int tmp;
            cin >> tmp;
            strength.insert(tmp);
        }

        for(int i=0; i<m; i++) {
            int tmp;
            cin >> tmp;
            weight.insert(tmp);
        }

        if( *(weight.begin()) > *(strength.begin())) {
            cout << "impossible" << endl;
            continue;
        }


        rounds = 0;

        /* as long as there are boxes left... */
        while(!weight.empty()) {
            bool erasemode = false;

            /* go through all friends: O(n) */
            for(auto ai=strength.begin(); ai != strength.end();) {
                // cerr << "forloop\n";
                if(erasemode) {
                    ai = strength.erase(ai);
                    continue;
                }

                /* and look for the next box this friend can take */
                auto box = weight.lower_bound(*ai);

                // cerr << "box: " << *box << endl;

                /* weak friend - remove all friends that are weaker */
                if(box == weight.end()) {
                    ai = strength.erase(ai);
                    erasemode = true;
                    continue;
                }

                weight.erase(box);
                ai++;
            }

            rounds++;
        }

        cout << (rounds*3)-1 << endl;

    }

}
