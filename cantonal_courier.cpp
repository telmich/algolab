#include <iostream>
#include <vector>
#include <map>
#include <list>


using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int z, j;
        cin >> z >> j;

        vector<int> cost(z);
        for(int i=0; i<z; ++i) {
            cin >> cost[i];
        }

        vector<int> reward(j);
        for(int i=0; i< j; ++i) {
            cin >> reward[i];
        }
        map<int, list<int> > ticket_count;
        vector<list<int > > job_ticket(j);

        for(int i=0; i<j; ++i) {
            int ni;
            cin >> ni;

            ticket_count[ni].push_back(i);

            /* read the tickets */
            for(int k=0; k < ni; ++k) {
                int x;
                cin >> x;
                job_ticket[i].push_back(x);
            }

        }


    }

}
