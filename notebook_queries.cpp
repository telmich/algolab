#include <vector>
#include <iostream>

using namespace std;

void store_element(int &value,
                   vector<int> &pos,
                   vector<vector<int>> &table,
                   vector<int> &ai,
                   vector<int> &ci)

{

    int t_index = 0;
    int t_pos   = pos[0] - ai[0];

    for(int i=0; i < pos.size()-1; i++) {
        int next_table;

        if(table[t_index][t_pos] == -1) { /* create new vector */
            table.resize(table.size() +1);
            next_table = table.size() -1;

            table[next_table] = vector<int>(ci[i+1], -1);

            table[t_index][t_pos] = next_table;

            // cerr << "Generating next table for " << i << "," << t_pos << ": " << next_table << " size: " << ci[i+1] << endl;

        } else {
            next_table = table[t_index][t_pos];
        }
        // cerr << "Using table for " << i << "," << t_pos << ": " << next_table << endl;

        t_index = next_table;
        t_pos   = pos[i+1] - ai[i+1];
    }

    // cerr << "Storing " << value << " at " << t_index << "," << t_pos << endl;

    table[t_index][t_pos] = value;

}

int get_element(vector<int> &pos,
                vector<vector<int>> &table,
                vector<int> &ai)

{

    int res = -1;

    int t_index = 0;
    int t_pos   = pos[0] - ai[0];
    int next_table;



    for(int i=0; i < pos.size(); i++) {
        next_table = table[t_index][t_pos];

        if(next_table == -1) break;

        t_index = next_table;
        t_pos   = pos[i+1] - ai[i+1];
    }

    return next_table;



}


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int k, n, q;
        cin >> k >> n >> q;

        vector<int> ai(k);
        vector<int> bi(k);
        vector<int> ci(k);

        vector<vector<pair<int, int>>> records(k);
        vector<int> result;




        for(int i=0; i < k; i++) {
            cin >> ai[i] >> bi[i];
            ci[i] = bi[i] - ai[i] + 1;
            records[i].resize(ci[i]);
        }

        vector<vector<int>> table(1);
        table[0].resize(ci[0], -1);

        for(int i=0; i < n; i++) {
            vector<int> pos(k);
            int res;

            for(int j=0; j < k; j++) {
                cin >> pos[j];

            }
            cin >> res;

            store_element(res, pos, table, ai, ci);
        }

        for(int i=0; i < q; i++) {
            vector<int> pos(k);
            int res;

            // cerr << "Looking for " ;

            for(int j=0; j < k; j++) {

                cin >> pos[j];
                // cerr << pos[j] << " ";
            }


            // cerr << endl;
            cout << get_element(pos, table, ai) << endl;
        }
        // cerr << "----" << endl;


    }


}
