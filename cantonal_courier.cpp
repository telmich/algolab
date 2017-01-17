#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <tuple>

using namespace std;


int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        // cerr << "in\n";
        /******************** read data ********************/
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

        /* win with 0 zone tickets */
        int zero_win = 0;
        int max_win;

        if(!ticket_count[0].empty()) {
            for(auto it = ticket_count[0].begin(); it != ticket_count[0].end(); ++it) {
                zero_win += reward[*it];
            }
        }

        /******************** pair all jobs in j^3 ********************/
        list<tuple<set<int>, set<int>, int> >  my_pairs;

        /* create initial list */
        for(int i=0; i<j; ++i) {
            set<int> this_zones;
            set<int> this_jobs;
            int this_reward;

            this_jobs.insert(i);
            this_zones.insert(job_ticket[i].begin(), job_ticket[i].end());
            this_reward = reward[i];

            my_pairs.push_back(make_tuple(this_jobs, this_zones, this_reward));

        }

        vector<list<tuple<set<int>, set<int>, int> > > all_pairs;

        all_pairs.push_back(my_pairs);

        // cerr << "pre second\n";
        /* pair list with lists until full length */
        for(int i=0; i<j; ++i) {
            list<tuple<set<int>, set<int>, int> > new_pairs; /* length i of the jobs */

            for(auto it = all_pairs[i].begin(); it != all_pairs[i].end(); ++it) {

                set<int> this_jobs(get<0>(*it)); /* new job set */
                this_jobs.insert(i);

                set<int> this_zones(get<1>(*it)); /* new zone set */
                this_zones.insert(job_ticket[i].begin(), job_ticket[i].end());

                int this_reward = get<2>(*it) + reward[i];

                // cerr << "jobs: ";
                // for(auto debugit = this_jobs.begin(); debugit != this_jobs.end(); ++debugit) {
                //     cerr << *debugit << " ";
                // }
                // cerr << " reward: " << this_reward << endl;

                new_pairs.push_back(make_tuple(this_jobs, this_zones, this_reward));
            }
            //  cerr << endl;
            all_pairs.push_back(new_pairs);
        }

        /******************** Find best ********************/
        int best = 0;
        for(auto it = all_pairs.begin(); it != all_pairs.end(); ++it) {
            for(auto it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
                //cerr << "jobs: ";

                int this_reward = 0;

                for(auto job_it = get<0>(*it2).begin(); job_it != get<0>(*it2).end(); ++job_it) {
//                    cerr << *job_it << " ";
                    this_reward += reward[*job_it];
                }


                /* can be optimized -> we are only adding new zones above! */
                // cerr << " zones : ";
                int this_cost = 0;

                for(auto zone_it = get<1>(*it2).begin(); zone_it != get<1>(*it2).end(); ++zone_it) {
                    //                   cerr << *zone_it << " ";
                    this_cost += cost[*zone_it];
                }

                // cerr << " cost: " << this_cost;
                // cerr << " reward: " << this_reward;

                best = max(best, this_reward - this_cost);

//                cerr << " best: " << best << endl;
            }
        }
        cout << best << endl;


        continue;


        /******************** init output to -1 ********************/
        //        vector<vector<pair<set<int>, int> > > win_table(j, vector<pair<set<int>, int> >(j));
        vector<vector<pair<set<int>, int> > > win_table(j, vector<pair<set<int>, int> >(j, make_pair(set<int>(), -20)));

        for(int i=0; i<j; ++i) {
            for(int k=1; k < j; ++k) { /* pair with that many additional jobs, all  */
                win_table[i][j].second = -1;

                cerr << i << " " << k <<  " " << j << " " << win_table[i][j].second << " A" << endl;
            }
        }




        for(int i=0; i<j; ++i) {
            for(int k=i; k < j; ++k) {


            }
        }


//        map<vector<int> > win_table(z, vector<int>(z, 0));
//        map<int> zone_win(z, vector<int>(z, 0));

        /******************** calculate all win ********************/
        for(auto it = ticket_count.begin(); it != ticket_count.end(); ++it) {

            if(it->first == 0) continue;

            cerr << "analysing length " << it->first << "\n";


        }

        cerr << "-----\n";
        /******************** calculate cost ********************/
        cout << max_win << endl;


    }

}
