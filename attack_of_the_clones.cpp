#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct jedi {
    int a;
    int b;
    int dist;
};

struct point {
    int a;
    bool start;
};

int mydist(int a, int b, int m)
{
    int res;
    if(a < b)
        res = (b-a);
    else
        res = (m-a) + b;

    return res;
}

bool operator<(jedi j1, jedi j2)
{
    return j1.b < j2.b;
}


bool operator<(point j1, point j2)
{
    return j1.a < j2.a;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n, m;
        cin >> n >> m;

        int no_conflicts = 0;

        vector<jedi> jedi_segment(n);
        vector<point> points;

        for(int i=0; i < n;++i) {
            jedi j;
            cin >> j.a >> j.b;

            j.dist = mydist(j.a, j.b, m);

            point p1, p2;

            /* crosses 0  -- align them correctly */
            if(j.a > j.b) {
                no_conflicts++;

                p1.a = j.b;
                p1.start = false;

                p2.a = j.a;
                p2.start = true;

            } else {
                p1.a = j.a;
                p1.start = true;
                p2.a = j.b;
                p2.start = false;
            }

            jedi_segment[i] = j;

            points.push_back(p1);
            points.push_back(p2);
        }

        cerr << "0 crosses: " << no_conflicts << endl;


        sort(points.begin(), points.end());

        /* find lowest collision */
        int min_conflicts = no_conflicts;
        int best_point = 1;

        for(auto it= points.begin(); it != points.end(); ++it) {
            if(it->start) {
                no_conflicts++;
            } else {
                no_conflicts--;
                if(no_conflicts < min_conflicts) {
                    min_conflicts = no_conflicts;
                    best_point = it->a;
                }
            }
        }
        cerr << "best start: " << best_point << " with " << min_conflicts << endl;

        /* start at the given position, use the lowest length segment */
        int start_point = best_point; /* to be used after overflow as end */

        /* prepare jedis: sort them */
        sort(jedi_segment.begin(), jedi_segment.end());
        int l = 0;
        int r = jedi_segment.size() -1;

        int jedi_pos;

        while(l < r) {
            int middle = l+(r-l)/2;

            /* found! */
            if(jedi_segment[middle].b == start_point) {
                jedi_pos = middle;

                int jedi_left = middle;
                int jedi_right = middle;

                int shortest_left = jedi_segment[middle].dist;
                int shortest_right = jedi_segment[middle].dist;

                /* find first such jedi and select the shortest */

                /* search left first */
                while(jedi_left > 0 && jedi_segment[jedi_left-1].b == start_point) {
                    if(jedi_segment[jedi_left-1].dist < shortest_left) {
                        jedi_pos = jedi_left-1;
                        shortest_left = jedi_segment[jedi_left-1].dist;
                    }
                    --jedi_left;
                }
                jedi_left = jedi_pos;

                /* search right after */
                while(jedi_right < (jedi_segment.size()-1) && jedi_segment[jedi_right+1].b == start_point) {
                    if(jedi_segment[jedi_right+1].dist < shortest_right) {
                        jedi_pos = jedi_right+1;
                        shortest_right = jedi_segment[jedi_right+1].dist;
                    }
                    ++jedi_right;
                }
                jedi_right = jedi_pos;

                jedi_pos = middle;

                int jedi_dist = jedi_segment[middle].dist;

                if(shortest_left < jedi_segment[jedi_pos].dist) {
                    jedi_pos = jedi_left;
                    jedi_dist = shortest_left;
                }

                if(shortest_right < jedi_segment[jedi_pos].dist) {
                    jedi_pos = jedi_right;
                    jedi_dist = shortest_right;
                }

                if(jedi_pos - jedi_dist < 0) {
                    start_point = m + (jedi_pos - jedi_dist);
                } else {
                    start_point = jedi_pos - jedi_dist;
                }

                break;
            }

            /* here? Did not find that jedi! */
            if(jedi_segment[middle].b < start_point) { /* too far left -> go right */
                l = middle+1;
            } else {
                r = middle;
            }
        }

        cerr << "Jedi at " << jedi_pos << " from: " << start_point << endl;

        /* afterwards go from destination point to destination point and check
         * whether it's start point is *after* the current point.
         * if yes: use it: set new destination point, increment counter
         * if no: skip it
         */

        int jedi_count = 1;

        int jedi_orig_pos = jedi_pos;
        for(int i=jedi_pos+1; i < jedi_segment.size(); ++i) {
            if(jedi_segment[i].a > jedi_segment[jedi_pos].b ) {
                ++jedi_count;
                jedi_pos = i;
            } else {
                cerr << "Jedi " << i << " too far: " << jedi_segment[i].a << " " << jedi_segment[i].b << "<<<< " << jedi_segment[jedi_pos].b << endl;
            }
        }

        /* after overflow */
        for(int i=0; i < jedi_orig_pos; ++i) {
            if(jedi_segment[i].a > jedi_segment[jedi_pos].b ) {
                ++jedi_count;
                jedi_pos = i;
            }

            /* back at the beginning */
            if(jedi_segment[jedi_pos].b == jedi_segment[jedi_orig_pos].b) {
                cerr << "early break\n";
                break;
            }
        }

        cout << jedi_count << endl;


    }
}
