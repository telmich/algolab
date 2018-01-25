#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector <int > > field;

bool can_place_tile(field &plan, pair<int, int> pos)
{
    int x = pos.first;
    int y = pos.second;

    /* check if we are within inner boundary */
    if(x+1 > plan.size()) return false;
    if(y+1 > plan[x].size()) return false;

    bool all_one = true;

    for(int i=x; i <= x+1; ++i) {
        for(int j=y; j <= y+1; ++j) {
            if(plan[i][j] != 1) {
                all_one = false;
                break;
            }
        }
        if(!all_one) break;
    }

    return all_one;
}

pair<int, int> get_next_pos(field &plan, pair<int, int> pos)
{
    int x_off = 1;

    if((pos.first + 3) == plan.size()) { /* need to to go next row */
        pos.first = 1; /* skip first row */
        pos.second += 1;
    } else { /* stay in this row */
        pos.first++;
    }

    return pos;
}

/* basically check if it is the last field */
bool is_next_field_available(field &plan, pair<int, int> pos)
{
    bool is_available = true;

    if((pos.first + 3) == plan.size()) {
//        cerr << "pos1 match\n";
        if(       ((pos.second + 3) == plan[pos.first].size())) {
//            cerr << "pos2 match";
            is_available = false;
        }
    }
//    cerr << "next field " << is_available << ": " << pos.first + 3 << " " << pos.second + 3 << " " << plan.size() << " " << plan[pos.first].size() << endl;

//    if(pos.first >= 6 || pos.second >= 6) { cerr << "BUG" ; exit(42); }

    return is_available;

}

void copy_field(field &src, field &dst)
{
    for(int i=0; i < src.size(); ++i) {
        for(int j=0; j < src[0].size(); ++j) {
//            cerr << "ij " << i << " " << j << endl;
            dst[i][j] = src[i][j];
        }
    }

}
void dump_field(field &plan)
{
    for(int i=0; i < plan.size(); ++i) {
        for(int j=0; j < plan[i].size(); ++j) {
            cerr << plan[i][j] << " ";
        }
                cerr << endl;
    }
}

void mark_tile_used(field &plan, pair<int, int> pos)
{
//    dump_field(plan);
    for(int i=pos.first; i < pos.first+2; ++i) {
        for(int j=pos.second; j < pos.second+2; ++j) {
            plan[i][j] = 0;
        }
    }
//    cerr << "------\n";
    dump_field(plan);
}


int solve(field &plan, field& results, pair<int, int> pos)
{
    bool another_available;

    cerr << "heeeee" << pos.first << " " << pos.second << "\n";
    if(results[pos.first][pos.second] != -1) {
        cerr << "<){}?\n";
        return results[pos.first][pos.second];
    }
            cerr << "heeeee2\n";

/* this should not be necessary, as always next field */
//    res[pos.first][pos.second] = 0 ;

    /* find next field */
    while(!can_place_tile(plan, pos) && (another_available = is_next_field_available(plan, pos))) {
        pos = get_next_pos(plan, pos);
        cerr << "pos = " << pos.first << " " << pos.second << endl;
    }

    int res = 0;

    /* we found a place - try to use it */
    if(can_place_tile(plan, pos)) {
        dump_field(plan);
        cerr << "placing at " << pos.first << " " << pos.second << endl;

        /* we are in the last call */
        if(!is_next_field_available(plan, pos)) {
            cerr << "max for " << pos.first << " " << pos.second << " : " << 1 << endl;
            return 1;
        }

        field plancopy = field(plan.size(), vector<int>(plan[0].size()));
        copy_field(plan, plancopy);

        mark_tile_used(plancopy, pos);

        pair<int, int> newpos = get_next_pos(plancopy, pos);
        cerr << ">>>> nextpos = " << newpos.first << " " << newpos.second << endl;

        int used_it     = 1 + solve(plancopy, results, newpos);
        int not_used_it = solve(plan, results, newpos);

        res = max(used_it, not_used_it);
        cerr << "max for " << pos.first << " " << pos.second << " : " << res << ": " << used_it << "---" << not_used_it << endl;
    }
    results[pos.first][pos.second] = res;

    return res;
}

/* 0 1 2 3
   0 0 0 0 plan.size == 4
   0 1 1 0 plan[x].size == 4
   0 1 1 0 We never check the first, last row or first,last column
   0 0 0 0
*/

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int h, w;
        cin >> h >> w;

        vector<vector <int> > plan(h, vector<int>(w));
        vector<vector <int> > results(h, vector<int>(w, -1));

        for(int i=0; i< h; ++i) {
            for(int j=0; j < w; ++j) {
                cin >> plan[i][j];
                results[i][j] = -1;
            }
        }
        cout << solve(plan, results, make_pair(1, 1)) << endl;
    }
}
