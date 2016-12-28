#include <iostream>
#include <climits>
#include <vector>
#include <algorithm> /* sort */
#include <unordered_map>

/* Too slow, possible fixes:

[x] create hash table for name lookup? -> map? -> UNORDERED MAP!

Reversed list/vector/container:

- by age
- reachable from

vector w/ all ages containing

Can we build O(1) access for name + age?


name     age------>
|
|
|
v

>>> (50000**2)/(1024**3)
2.3283064365386963

2.3 GiB!

List is relatively small!!!

256 length => 12 MiB
>>> (50000*256)/(1024**2)
12.20703125


- precompute distances in O(1) array?

-- use ancestor age as offset to build array?


*/

using namespace std;

struct node {
    string name;
    int age;
    node *prev;
    node *next;
};

bool operator < (const node& n1, const node& n2) {
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
    node n_in;
    node *n2;

    cin >> t;

    while(t--) {
        cin >> n;
        cin >> q;

        unordered_map<string, node> species2;
        vector<query> queries(q);

        for(int i=0; i <n; ++i) {
            n_in.prev = NULL;
            n_in.next = NULL;

            cin >> n_in.name;
            cin >> n_in.age;

            species2[n_in.name] = n_in;
        }

        /* read relations / graph building */
        string offspring, ancestor;
        node n_off, n_anc;
        for(int i=0; i < (n-1); ++i) {
            cin >> offspring;
            cin >> ancestor;

            (species2[offspring]).prev = &(species2[ancestor]);
            (species2[ancestor]).next = &(species2[offspring]);
        }

        // /* Average length: 256 */
        // long cnt = 0;
        // for(auto it = species2.begin(); it != species2.end(); ++it) {
        //     n2 = &(it->second);
        //     while(n2) {
        //         cnt++;
        //         n2 = n2->prev;
        //     }
        // }
        // cout << "cnt:" << cnt/species2.size() << endl;

        // exit(0);

        /* Answer queries */
        for(int i=0; i < q; ++i) {
            cin >> name;
            cin >> age;
            n2 = &(species2[name]);

            age = 0;
            while(n2->prev && n2->prev->age > age) {
                n2 = n2->prev;
            }
            cout << n2->name;

            if((q-i) > 1) cout << " ";

        }
        cout << "\n";
    }
}
