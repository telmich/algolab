#include <iostream>
#include <set>
#include <vector>

using namespace std;

/* missing rule:
  - if not present in <, > -> then good

 - naive: 1000*1000
   take all coins
   subtract / remove those that are present in >, <
   all others are good

- non-naive:


*/


void mark_coins(vector<int> &coin,
                set<int> &addtothisset,
                set<int> &ifnotinhere,
                set<int> &isgood)
{

    // for(allcoins) { /* 1000 */
    //     if(!check_if_coin_is_in(here) /* <= 1000 */)
    //         isgood.insert(*it);
    //         where here = vector

    //         }

    for(auto it=coin.begin(); it != coin.end(); it++) {
        auto search_other = ifnotinhere.find(*it);
        auto search_good  = isgood.find(*it);

        /* already marked good, ignore */
        if(search_good != isgood.end()) continue;

        /* is in the other candidates set -> is actually good! */
        if(search_other != ifnotinhere.end()) {
            /* add to good */
            isgood.insert(*it);

            /* remove from candidates set */
            ifnotinhere.erase(search_other);

            continue;
        }

        cerr << "Adding " << *it << endl;

        /* is in neither of the other two sets, add to this set */
        addtothisset.insert(*it);
    }

}

void addalltoset(vector<int> &coin, set<int> &markingset)
{
    for(auto it=coin.begin(); it != coin.end(); it++) {
        cerr << "isgood: " << *it << endl;
        markingset.insert(*it);
    }
}

void removefromset(vector<int> &coin, set<int> &markingset)
{
    for(auto it=coin.begin(); it != coin.end(); it++) {
        auto search = markingset.find(*it);
        if(search != markingset.end()) {
            markingset.erase(*search);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, k;

        cin >> n >> k;

        set<int> canbe_heavier;
        set<int> canbe_lighter;
        set<int> isgood;

        for(int i=0; i < k; i++) {
            int coins;
            cin >> coins;
            int tmp;

            vector<int> left_pan;
            vector<int> right_pan;

            for(int j=0; j < coins; j++) {
                cin >> tmp;
                left_pan.push_back(tmp);
                cerr << tmp << " ";
            }
            cerr << endl;

            for(int j=0; j < coins; j++) {
                cin >> tmp;
                right_pan.push_back(tmp);
                cerr << tmp << " ";
            }
            cerr << endl;

            char operation;
            cin >> operation;
            cerr << operation << endl;

            switch(operation) {
            case '>':
                mark_coins(left_pan, canbe_heavier, canbe_lighter, isgood);
                mark_coins(right_pan, canbe_lighter, canbe_heavier, isgood);

                break;
            case '<':
                mark_coins(left_pan,  canbe_lighter, canbe_heavier, isgood);
                mark_coins(right_pan, canbe_heavier, canbe_lighter, isgood);

                break;

            case '=':
                addalltoset(left_pan, isgood);
                addalltoset(right_pan, isgood);

                removefromset(left_pan, canbe_heavier);
                removefromset(right_pan, canbe_heavier);
                removefromset(left_pan, canbe_lighter);
                removefromset(right_pan, canbe_lighter);

                break;
            default:
                cerr << "my code is buggy" << endl;
                exit(42);
                break;
            }
        }

        cerr << "> ";

        for(auto it = canbe_heavier.begin(); it != canbe_heavier.end(); it++) {
            cerr << *it << " ";
        }
        cerr << endl;

        cerr << "< ";
        for(auto it = canbe_lighter.begin(); it != canbe_lighter.end(); it++) {
            cerr << *it << " ";
        }
        cerr << endl;

        cerr << "== ";
        for(auto it = isgood.begin(); it != isgood.end(); it++) {
            cerr << *it << " ";
        }
        cerr << endl;




        int num_candidates = canbe_heavier.size() + canbe_lighter.size();

        int coinnum = 0;

        if(num_candidates == 1) {
            if(canbe_heavier.size() == 1) {
                auto elem = canbe_heavier.begin();
                coinnum = *elem;
            }
            if(canbe_lighter.size() == 1) {
                auto elem = canbe_lighter.begin();
                coinnum = *elem;
            }
        }
        cout << coinnum << endl;
    }
}
