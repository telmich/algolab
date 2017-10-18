#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <forward_list>

using namespace std;

typedef map<int, forward_list<int>> NODES;
typedef vector<bool> NODES_SEEN;
typedef map<int, vector<pair<long, int>>> QUERY;

/* could have used
// typedef vector<vector<pair<long, int>>> QUERY;

typedef vector<int> ANSWER;
typedef map<int, long> AGE;

typedef map<long, int, greater<long> > AGE_ON_PATH;

void mydfs(int rootnode, NODES &nodes, QUERY &query,
           ANSWER &answer, NODES_SEEN &nodes_seen,
           AGE_ON_PATH &aop, AGE &age)
{
    for(auto ai=nodes[rootnode].begin(); ai != nodes[rootnode].end(); ai++) {
        if(!nodes_seen[*ai]) {
            nodes_seen[*ai] = true;

            /* we can also find ourselves! */
            aop[age[*ai]] = *ai;

            /* check all queries at this node */
            for(auto ai2=query[*ai].begin(); ai2 != query[*ai].end(); ai2++) {
                long maxage     = ai2->first;
                int  queryindex = ai2->second;

                /* runs in log(n) - there is always at least one element */
                auto ai4 = aop.lower_bound(maxage);
                int result = ai4->second;

                /* save at right position for ordering */
                answer[queryindex] = result;
            }

            mydfs(*ai, nodes, query, answer, nodes_seen, aop, age);

            /* remove our age - keeps duplicates --- FIXME: requires multimap */
            auto ai3 = aop.find(age[*ai]);
            aop.erase(ai3);
        }
    }
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        int n, q;
        cin >> n >> q;

        map<string, int> name2id;
        map<int, string> id2name;
        AGE              id2age;
        map<int, int>    offspringof;

        vector<bool>    canberoot(n, true);

        int id = 0;

        for(int i=0; i <n; i++) {
            string name;
            long age;

            cin >> name >> age;

            /* necessary or stupid? */
            name2id[name] = id;
            id2name[id] = name;

            id2age[id] = age;

            id++;
        }

        /* Read tree definition */
        for(int i=0; i <(n-1); i++) {
            string s, p;
            int si, pi;

            cin >> s >> p;

            si = name2id[s];
            pi = name2id[p];

            offspringof[si] = pi;

            canberoot[si] = false;
        }

        /* Store queries at nodes */
        QUERY query;

        for(int i=0; i <q; i++) {
            string species;
            int speciesi;
            long age;

            pair<long, int> age_and_id;

            cin >> species >> age;

            age_and_id.first  = age;
            age_and_id.second = i;

            speciesi = name2id[species];

            query[speciesi].push_back(age_and_id);
        }

        NODES nodes;

        /* Enable tree access from p, record all children */
        for(auto ai = offspringof.begin(); ai != offspringof.end(); ai++) {
            nodes[ai->second].push_front(ai->first);
        }

        /* Find the root node */
        int theroot = -1;
        for(int i=0; i < n; i++) {
            if(canberoot[i]) {
                theroot = i;
                break;
            }
        }
        if(theroot == -1) {
            cerr << "Did not find root" << endl;
            exit(42);
        }
//        cerr << "Root: " << id2name[theroot] << endl;

        /* DFS from root, answering all queries */

        ANSWER answer(q);
        NODES_SEEN nodes_seen(n, false);
        AGE_ON_PATH aop;

        aop[id2age[theroot]] = theroot;

        mydfs(theroot, nodes, query, answer, nodes_seen, aop, id2age);

        for(int i=0; i<q; i++) {
            cout << id2name[answer[i]] << " " ;
        }
        cout << endl;
    }

}
