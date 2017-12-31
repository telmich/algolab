#include <iostream>
#include <vector>

using namespace std;


inline int is_equal_sum(const vector<int> &values, int target) {
    for(int i=0; i < values.size(); ++i) {
        target -= values[i];
    }
    return target == 0;
}


int do_magic(vector<vector<int> > &switches, vector<int> &brightness)
{
    /* 1st room: all switches possible */
    /* reduce number of switches per room, abort if no combination possible */

    /* all rooms always participate ! => smaller choice for subset sum! */

    /*  each light is connected to exactly one switch. */

    for(int i=0; i < brightness.size(); ++i) {
        cerr << "Solving for room " << i << ": " << brightness[i] << endl;
    }

    return 42;
}

int solve_one_room(vector<vector<int> > &switches, int brightness, vector<int>& solutions)
{
    /* not *exactly* subset sum =>
       we always add  values of *exactly* n switches

       Does sorting or generating maximum sum help?

    */

    for(int i=0; i < switches.size(); ++i) {

    }

    return -1;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, m;
        cin >> n >> m;

        vector<int> brightness(m);
        vector<vector<int> > switches(n, vector<int>(2*m));

        vector<vector<int> > switches2(m, vector<int>(2*n));


        for(int i=0; i < m; ++i)
            cin >> brightness[i];


        for(int i=0; i < n; ++i) {
            for(int j=0; j < 2*m; ++j) {
                cin >> switches[i][j];
            }
        }

        int res = do_magic(switches, brightness);
        if(res == -1) {
            cout << "impossible\n";
        } else {
            cout << res << endl;
        }
    }
}
