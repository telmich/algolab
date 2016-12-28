#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
#include <set>

using namespace std;

int solve(const vector<vector<int> >& input) {
    priority_queue<pair<int, int>, vector<pair<int, int> > > values;
    int n = input.size();

    for(int i=0; i < n; ++i) {
        for(int j=0; j < input[i].size(); j++) {
            values.push(make_pair(input[i][j], i));
        }
    }

    /* maximum distance at the beginning */
    int min_dist = INT_MAX;

    /* NEXT: try vector with pair */
    set<pair<int, int> > allwords;
    vector<int> positions(n, -1);
    int wordcount = 0;

    while(!values.empty()) {
        pair<int, int> val = values.top(); values.pop();

        int pos = val.first;
        int word = val.second;

        /* first time found */
        if(positions[word] == -1) {
            wordcount++;
        } else {
            pair<int, int> replace = make_pair(positions[word], word);
            allwords.erase(replace);
        }

        allwords.insert(val);
        positions[word] = pos;

        if(wordcount == n) {
            int a = (allwords.begin())->first;  /* first item */
            int b = (allwords.rbegin())->first; /* last item */
            min_dist = min(b-a, min_dist);
        }

        // cerr << val.first << " " << val.second << "\n";
    }

    /* EXAMPLE: set */
    // set<pair<int, int> > s;

    // s.insert(make_pair(13, 16));
    // s.insert(make_pair(13, 12));
    // set<pair<int, int>>::iterator it;
    // for(it = s.begin(); it != s.end(); ++it) {
    //     cout << (*it).first << " " << (*it).second << "\n";
    // }

    return min_dist+1;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t, cnt=0;

    cin >> t;

    while(t--) {
        int n, tmp;
        cin >> n;
        vector<int> occurences(n);

        for(int i=0; i < n; i++) {
            cin >> occurences[i];
        }
        vector<vector<int> > positions(n);

        for(int i=0; i < n; i++) {
            for(int j=0; j < occurences[i]; j++) {
                cin >> tmp;
                positions[i].push_back(tmp);
                cnt++;
            }
        }
        cout << solve(positions) << "\n";

    }
}

        /* Sort / insert values */
        /* FIXME: priority_queue or min heap ? */
