#include <iostream>
#include <vector>
#include <queue>
#include <forward_list>


using namespace std;


bool operator<(const pair<int, int> a, const pair<int, int>  b)
{
    if(a.first == b.first) return a.second < b.second;

    return a.first < b.first;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int g, s, l;

        cin >> g >> s >> l;

        /* used to distinguish */
        int satellite_offset = g;

        vector<forward_list<int> > links(g+s);

        vector<int> lcounter(g+s, 0);
        vector<bool> vertex_used(g+s, false);

        priority_queue<pair<int, int> > q;

        for(int i=0; i < l; ++i) {
            int x, y;

            cin >> x >> y;

            links[x].push_front(satellite_offset+y);
            links[satellite_offset+y].push_front(x);

            lcounter[x]++;
            lcounter[satellite_offset+y]++;
        }

        /* to be able to get the highest counted vertex */
        for(int i=0; i < (g+s); ++i) {
            q.push(make_pair(lcounter[i], i));
        }

        int num_stations = 0;
        int num_satellites = 0;

        forward_list<int> stations;
        forward_list<int> satellites;


        while(!q.empty()) {
            pair<int, int> p = q.top(); q.pop();

            int count = p.first;
            int num = p.second;


            if(vertex_used[num]) continue;

            /* add counter */
            if(num >= satellite_offset) {
                num_satellites++;
                satellites.push_front(num-satellite_offset);
            } else {
                num_stations++;
                stations.push_front(num);
            }

            /* remove this and all connected vertices */
            vertex_used[num] = true;

            for(auto it=links[num].begin(); it != links[num].end(); ++it) {
                vertex_used[*it] = true;
            }

        }
        cout << num_stations << " " << num_satellites << endl;
        for(auto it=stations.begin(); it != stations.end(); ++it) {
            cout << *it << " ";
        }
        for(auto it=satellites.begin(); it != satellites.end(); ) {
            cout << *it;
            ++it;
            if(it != satellites.end()) cout << " ";
        }
        cout << endl;
    }

}
