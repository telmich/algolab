#include <iostream>
#include <vector>

using namespace std;

int solve(int jid, vector<vector<int>> &job) {

}

int solve_one(vector<int> &zp, vector<int> &reward, vector<vector<int>> &job)
{
    vector<int> zonereward(zp.size(), 0);

    for(int i=0; i < job.size(); i++) {
        if(job[i].size() == 0) continue;

        int cur_zone = job[i][0];
        int cur_reward = reward[i];

        zonereward[cur_zone] += cur_reward;
    }

    int res = 0;

    for(int i=0; i < zp.size(); i++) {
        int cur_reward = zonereward[i] - zp[i];

        if(cur_reward > 0) {
            res += cur_reward;
        }
    }

    return res;

}

int main()
{
    int t;
    cin >> t;

    while(t--) {
        int z, j;

        cin >> z >> j;

        vector<int> zoneprice(z);
        for(int i=0; i<z; i++) {
            cin >> zoneprice[i];
        }

        vector<int> reward(j);
        for(int i=0; i<j; i++) {
            cin >> reward[i];
        }

        vector<vector<int>> job(j);

        int maxlen = 0;
        int rewardplus = 0;

        for(int i=0; i<j; i++) {
            int tmp;
            cin >> tmp;

            if(tmp == 0) { /* filter out */
                rewardplus += reward[i];
                continue;
            }

            maxlen = max(tmp, maxlen);

            job[i].resize(tmp);
//            cerr << "job " << i << " size " << tmp << endl;

            for(int k=0; k<tmp; k++) {
                cin >> job[i][k];
            }
        }

        int res = 0;

        if(maxlen == 1) {
            res = solve_one(zoneprice, reward, job);
        }

        res += rewardplus;

        cout << res << endl;


    }

}
