#include <iostream>
#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


using namespace std;
using namespace boost;



int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;

    while(t--) {
        int n, e, s, a, b;

        cin >> n >> e >> s >> a >> b;

        vector<int> t1(e);
        vector<int> t2(e);

        vector<vector<int>> weights(e);


        for(int i=0; i < e; i++) {
            cin >> t1[i] >> t2[i];

            for(int j=0; j < s; j++) {
                int tmp;
                cin >> tmp;
                weights[i].push_back(tmp);
            }
        }

        vector<int> hive(s);

        for(int i=0; i < s; i++) {
                int tmp;
                cin >> tmp;
                hive[i] = tmp;
        }
    }

}
