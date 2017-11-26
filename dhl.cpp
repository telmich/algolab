#include <iostream>
#include <vector>
#include <climits>

/*

2 stacks
n parcels

bulky vs. heavy
a1 at bottom


cost: (Sa-ka) * (Sb-kb)

Sa: sum volume
Sb sum weights
ka / kb: number of parcels

- at least one parcel each time

- minimise cost


t <= 20
n <= 1000 parcels => O(n^2) möglich? 10^6


partial sum?
  sum up all

Monotonität vorhanden?

Maximal 1000 Möglichkeiten links
Maximal 1000 Möglichkeiten links


take/no take!

step 1:
  take at least topmost from each


Algorithmus

from bottom?

- take last
 - (one only) from both
 -

recursion?


sole n <= 6 problem!


links 1, rechts 1...n-1
 links 2..n & rechts n
done



links 1, rechts 1...n-2
 -> links 2, rechts n-1
 -> links 3..n, rechts n

 -> links 2..n, rechts n-1, n





*/

using namespace std;

int solve_recursive(vector<int> &a_stack, vector<int> &b_stack, int a_pos, int b_pos)
{

    /* select 1 ... leftover-1 from the left */
    for(int i=1; i < a_stack.size() - 1; ++i) {
    // (sa-ka) * (sb-kb);

    }


}


void gen_cost_table(vector<int> &a_stack, vector<int> &b_stack, vector<vector <int> > &cost)
{
    int sa = 0;
    int ka = 0;
    for(int a=a_stack.size() -1; a >= 0; --a) {
        sa += a_stack[a];
        ++ka;

        int sb = 0;
        int kb = 0;

        for(int b=b_stack.size()-1; b >= 0; --b) {
            sb += b_stack[b];
            ++kb;

//            cerr << a << " " << b << endl;
            cost[a][b] = (sa-ka) * (sb-kb);
        }
    }
}


int get_cost(int a_start, int a_end, int b_start, int b_end, vector<vector <int> > &cost)
{
    int res;

    if(b_start < a_start) { cerr << "BUG?\n"; return 0;  }

    if(a_start == 0) { /* no diff */
        return cost[a_end][b_end];
    } else {
        return cost[a_end][b_end] - cost[a_start-1][b_start-1];
    }

}

/* try every possibility */
// int get_min_cost(vector<int> &a_stack, vector<int> &b_stack, vector<vector <int> > &cost)
 int get_min_cost(int a, int b, vector<vector <int> > &cost)
{

    int minimum = INT_MAX;

    /* for every A * B range calculate cost */
    for(int a_start = 0; a_start < a_stack.size(); ++a_start) {
        for(int b_start = 0; b_start < a_stack.size(); ++b_start) {
            minimum += get_cost(a_start, ,
            get_min_cost(int a, int b, vector<vector <int> > &cost)
        }
    }

}


int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--)
    {
        int n;
        cin >> n;
        vector<int> a_stack(n);
        vector<int> b_stack(n);

        vector<vector <int> > costs(n, vector<int>(n));

        for(int i=0; i <n; ++i)
            cin >> a_stack[i];

        for(int i=0; i <n; ++i)
            cin >> b_stack[i];

        gen_cost_table(a_stack, b_stack, costs);
        get_min_cost(0, 0, costs);
    }

}
