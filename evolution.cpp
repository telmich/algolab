#include <iostream>
#include <climits>
#include <vector>
#include <algorithm> /* sort */

using namespace std;

struct node {
    string name;
    int age;
    node *prev;
    node *next;
};

bool operator < (const node& n1, const node& n2) {
//    cerr << "cmp: " << n1.name << "<->" << n2.name << ": " << (n1.name < n2.name) << "\n";
    return n1.name < n2.name;
}

struct query {
    string name;
    int age;
};

int main() {
    ios_base::sync_with_stdio(false);

    int t,n,q,tmp, age;
    string name;

    // cout << INT_MAX << endl;
    // cout << LONG_MAX << endl;

    cin >> t;

    while(t--) {
        cin >> n;
        cin >> q;

        vector<node> species(n);
        vector<query> queries(q);

        for(int i=0; i <n; ++i) {
            node n;
            n.prev = NULL;
            n.next = NULL;

            cin >> n.name;
            cin >> n.age;

            // cerr << n.name << endl;

            species[i] = n;

        }
        /* sort for searches */
        sort(species.begin(), species.end());

        /* read relations / graph building */
        node offspring, ancestor;
        for(int i=0; i < (n-1); ++i) {

            cin >> offspring.name;
            cin >> ancestor.name;

            // cerr << ancestor.name << "<----" << offspring.name << endl;

            auto off_node = lower_bound(species.begin(), species.end(), offspring);
            auto ancestor_node = lower_bound(species.begin(), species.end(), ancestor);

            if(off_node == species.end() || ancestor_node == species.end()) {
                cerr << "very wrong\n";
                exit(33);
            }

            off_node->prev = &(*ancestor_node);
            ancestor_node->next = &(*off_node);
        }

        node n, *np;
        for(int i=0; i < q; ++i) {
            cin >> n.name;
            cin >> n.age;

            auto nit = lower_bound(species.begin(), species.end(), n);
            np = &(*nit);

            age = 0;
            while(np) {
                // cerr << "(" << age << ":" << np->prev->age << ":" << np->name << "->" << np->prev->name << ")->";

                // if((np->prev->age + age) > n.age) break;

                /* root node */
                if(!(np->prev)) break;

                if((np->prev->age) > n.age) break;

                age += np->prev->age;
                np = np->prev;

            }
            // cerr << endl;
            cout << np->name;
            if((q-i) > 1) cout << " ";

        }
        cout << "\n";


    }
}
