#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> troopercon;

#define MASK 3

enum { NONE=0, GREEN = 1, RED = 2, BOTH=4, ERASED=8 };

int matching_colour(int from, int colour, vector<vector <int> > &connection, vector<int > &trooper_colour)
{
    int cnt = 0;

    if(trooper_colour[from] & colour) {
//        cerr << from << " is " << trooper_colour[from] << endl;
        ++cnt;
    }

    trooper_colour[from] |= ERASED;

    int following_from_same_colour = 0;

    for(auto it =  connection[from].begin();  it != connection[from].end(); ++it) {
        if(trooper_colour[*it] & ERASED) {
            continue;
        }
        if(trooper_colour[*it] & colour) {
            ++following_from_same_colour;
        }


        cnt += matching_colour(*it, colour, connection, trooper_colour);
    }

    /* there is at least one more candidates -> remove double counting */
    if(following_from_same_colour >= 1) {
        cerr << "Removing one from " << from << endl;
        --cnt;
    }


    return cnt;
}

void colour_tree(int from, int last_colour, vector<vector <int> > &connection, vector<int > &trooper_colour)
{
//    cerr << "Visit " << from << endl;

    /* no colour assigned yet --> command center 0 case */
    if(!trooper_colour[from]) {
        trooper_colour[from] = last_colour ^ MASK;
    }

    /* iterate over all connections */
    for(auto it =  connection[from].begin();  it != connection[from].end(); ++it) {
        if(trooper_colour[*it]) { /* no need to revisit tree, but store possible additional colour */
            trooper_colour[*it] |= last_colour;
            continue;
        }

        trooper_colour[*it] = last_colour;

        colour_tree(*it, trooper_colour[from], connection, trooper_colour);
    }

}

int main(){
    int t;
    cin >> t;

    while(t--) {
        int k, s, m;
        cin >> k >> s >> m;

        /* no connection? take all! */
        if(m == 0) {
            cout << k*s << endl;
            continue;
        }

        vector<vector <int> > connection(k*s);

        for(int i=0; i < m; ++i) {
            int u, v, h;
            cin >> u >> v >> h;

            for(int j=0; j < h; ++j) {
                troopercon tc;

                int x, y;
                cin >> x >> y;

                int from = u*s + x;
                int to   = v*s + y;

                connection[from].push_back(to);
            }
        }

        /* start colouring from command center v
           not visited = 0
           green       = 1
           red         = 2
           both        = 4
         */

        int cur_colour = GREEN;

        vector<int > trooper_colour(k*s, NONE);

        /* Follow all links from command center 0 */
        for(int i=0; i < s; ++i) {
            colour_tree(i, cur_colour, connection, trooper_colour);
        }

        for(int i=0; i < k*s; ++i) {
            cerr << "Colour of " << i << " is " << trooper_colour[i] << endl;
        }


        int green_max = 0;
        int red_max   = 0;

        /* check green */
        for(int i=0; i < s; ++i) {
            green_max += matching_colour(i, GREEN, connection, trooper_colour);
        }

        /* remove ERASED marker, try for red */
        for(int i=0; i < k*s; ++i) {
            trooper_colour[i] ^= ERASED;
        }

        /* check red */
        for(int i=0; i < s; ++i) {
            red_max += matching_colour(i, RED, connection, trooper_colour);
        }


        cerr << "red/green " << red_max << " " << green_max << endl;
        cout << max(green_max, red_max) << endl;
    }
}
