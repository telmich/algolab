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

        /******************** pair all jobs in j^3 ********************/
        list<tuple<set<int>, set<int>, int, int> >  my_pairs;

        /* create initial list */
        for(int i=0; i<j; ++i) {
            set<int> this_zones;
            set<int> this_jobs;
            int this_reward;
            int this_cost = 0;

            this_jobs.insert(i);
            this_zones.insert(job_ticket[i].begin(), job_ticket[i].end());

            for(auto ai = job_ticket[i].begin(), ai != job_ticket[i].end(); ++ai) {
                this_cost += *ai;
            }
            this_reward = reward[i];

            my_pairs.push_back(make_tuple(this_jobs, this_zones, this_reward, this_cost));
        }

        vector<list<tuple<set<int>, set<int>, int> > > all_pairs;

        all_pairs.push_back(my_pairs);

        for(int i=0; i<j; ++i) {
            list<tuple<set<int>, set<int>, int> > new_pairs; /* length i of the jobs */

            for(auto it = all_pairs[i].begin(); it != all_pairs[i].end(); ++it) {

                set<int> this_jobs(get<0>(*it)); /* new job set */

                auto search = this_jobs.find(i);

                int this_reward = get<2>(*it);
                if(search == this_jobs.end()) {
                    this_jobs.insert(i);
                    this_reward += reward[i];
                }


                /* use a zone index ? */



                set<int> this_zones(get<1>(*it)); /* new zone set */

                set<int> diffset(job_ticket[i].size());

                FIXME HERE

                auto i_it=set_difference (this_zones.begin(), this_zones.end(),
                                            job_ticket[i].begin(), job_ticket[i].end(), v.begin());
                v.resize(it-v.begin());

                this_zones.insert(job_ticket[i].begin(), job_ticket[i].end());

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

                int this_reward = get<2>(*it2);

                // for(auto job_it = get<0>(*it2).begin(); job_it != get<0>(*it2).end(); ++job_it) {
                //     cerr << *job_it << " ";
                //     this_reward += reward[*job_it];
                // }

                /* can be optimized -> we are only adding new zones above! */
                cerr << " zones : ";
                int this_cost = 0;

                for(auto zone_it = get<1>(*it2).begin(); zone_it != get<1>(*it2).end(); ++zone_it) {
                                      cerr << *zone_it << " ";
                    this_cost += cost[*zone_it];
                }

                cerr << " cost: " << this_cost;
                cerr << " reward: " << this_reward;

                best = max(best, this_reward - this_cost);

                cerr << " best: " << best << endl;
            }
        }
        cout << best << endl;


    }

}
