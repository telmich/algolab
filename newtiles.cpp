#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


bool fourones(int x, int y, vector<vector<int>> &field)
{
    if(y+1 > field.size())    return false;
    if(x+1 > field[0].size()) return false;

//    cerr << "ff " << x << " " << y << " " << field[0].size() << " " << field.size() << endl;
    int t = field[x][y] +  field[x+1][y] +  field[x][y+1] +  field[x+1][y+1];
//    cerr << "fe " << x << " " << y << endl;

    return t == 4;
}

int num_conflicts(int x, int y, vector<vector<int>> &field)
{
    int cnt = 0;

    if(fourones(x-1, y-1, field)) cnt++;
    if(fourones(x,   y-1, field)) cnt++;
    if(fourones(x+1, y-1, field)) cnt++;

    if(fourones(x-1, y,   field)) cnt++;
    if(fourones(x+1, y,   field)) cnt++;

    if(fourones(x-1, y+1, field)) cnt++;
    if(fourones(x,   y+1, field)) cnt++;
    if(fourones(x+1, y+1, field)) cnt++;

    return cnt;
}

void zerofield(int x, int y, vector<vector<int>> &field)
{
    field[x][y] = 0;
    field[x+1][y] = 0;
    field[x][y+1] = 0;
    field[x+1][y+1] = 0;
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        int h, w;
        cin >> h >> w;

        cerr << "cc: " << h << " " << w << endl;

        vector<vector<int>> field(w);

        for(int i=0; i<w; i++) {
            field[i].resize(h);
        }

        for(int i=0; i<h; i++) {
            for(int j=0; j<w; j++) {
                cin >> field[j][i];
            }
        }
//        cerr << "dd" << endl;

        vector<pair<int, int>> xy;

        for(int i=1; i<h-1; i++) {
            for(int j=1; j<w-1; j++) {
                if(fourones(j,i, field)) {
                    xy.push_back(make_pair(j, i));
                }
            }
        }

        if(xy.size() == 0) {
            cout << 0 << endl;
            continue;
        }

        vector<pair<int, int>> conflicts;

        for(int i=0; i < xy.size(); i++) {
            int num = num_conflicts(xy[i].first, xy[i].second, field);

            conflicts.push_back(make_pair(num, i));
//            cerr << xy[i].first << " " << xy[i].second << " conflicts: " << num << endl;
        }

        int res = 0;

        sort(conflicts.begin(), conflicts.end());

        for(auto ai = conflicts.begin(); ai != conflicts.end(); ai++) {
            int idx = ai->second;
            int cnt = ai->first;

            int x = xy[idx].first;
            int y = xy[idx].second;

            cerr << "conf: " << cnt << endl;

            if(fourones(x, y, field)) {
                ++res;
                zerofield(x, y, field);
            }
        }

        cout << res << endl;

    }
}
