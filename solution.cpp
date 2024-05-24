// Sebastian Galindo

#pragma GCC optimize ("O3")
#pragma GCC target ("sse4")
 
#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef complex<ld> cd;
 
typedef pair<int, int> pi;
typedef pair<ll,ll> pl;
typedef pair<ld,ld> pd;
 
typedef vector<int> vi;
typedef vector<ld> vd;
typedef vector<ll> vl;
typedef vector<pi> vpi;
typedef vector<pl> vpl;
typedef vector<cd> vcd;
 
template<class T> using pq = priority_queue<T>;
template<class T> using pqg = priority_queue<T, vector<T>, greater<T>>;
 
#define forn(i, a) for (int i=0; i<(a); i++)
#define trav(a,x) for (auto& a : x)
#define uid(a, b) uniform_int_distribution<int>(a, b)(rng)
 
#define len(x) (int)(x).size()
#define mp make_pair
#define pb push_back
#define f first
#define s second
#define lb lower_bound
#define ub upper_bound
#define all(x) x.begin(), x.end()
#define ins insert

template<class T> bool ckmin(T& a, const T& b) { return b < a ? a = b, 1 : 0; }
template<class T> bool ckmax(T& a, const T& b) { return a < b ? a = b, 1 : 0; }

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
 
template<typename T, typename V>
void __print(const pair<T, V> &x);
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? ", " : ""), __print(i); cerr << "}";}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}';}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifdef DEBUG
#define debug(x...) cerr << "\e[91m"<<__func__<<":"<<__LINE__<<" [" << #x << "] = ["; _print(x); cerr << "\e[39m" << endl;
#else
#define debug(x...)
#endif
 

const int MOD = 1000000007;
const char nl = '\n';
const int MX = 100001; 

vector<ll> permutation, a;

void generateHours(ll h, int b, int pausas, bool lunch) {
    if (__builtin_popcountll(h) + pausas > 32) return;
    if (__builtin_popcountll(h) + pausas == 32) {
        if (lunch) permutation.pb(h);
        return;
    }
    h |= (15LL << b);
    b+=4;
    for (int j = 0; j <= 4; j++) {
        ll k = (1LL << j)-1;
        
        if (b+j >= 47) continue;
        if (b+j >= 17 && b+j <= 23) {
            generateHours((h | (k << b)), b+(j+6), pausas, true);
            generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
        } else if (b+j == 45) generateHours((h | (k << b)), b + (j+1), pausas, lunch);
        else  generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
    }
}

struct HourSet {
    vector< vector<int> > p;

    void insert(ll h) {
        vector<int> nw;
        for (int i = 0; i < 46; i++) {
            if ((h >> i) & 1) nw.push_back(1);
            else nw.push_back(0);
        }
        p.push_back(nw);
    }


    void printHours() {
        for (auto per: p) {
            for (auto v: per) cout << v;
            cout << endl;
        }
    }

    bool f(int i) {
        for (auto per: p) {
            if (per[i] == 1) return false;
        }
        return true;
    }

    int f2(int i) {
        int res = 0;
        for (auto per: p) res += per[i];
        return max(a[i] - res, 0LL);
    }

    int firstValueFn() {
        int res = 0;
        for (int i = 0; i < 46; i++) {
            res += this->f(i);
        }
        return res;
    }

    int secondValueFn() {
        int res = 0;
        for (int i = 0; i < 46; i++) {
            res += this->f2(i);
        }
        return res;
    }

    int combineValues(int lambda, int phi) {
        return (this->firstValueFn() * lambda) + (this->secondValueFn() * phi);
    }

};

HourSet knapsackNoReptetitions(int W, int lambda, int phi) {
    int n = (int)permutation.size();
    HourSet dp[W+1][n+1];
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            dp[j][i] = HourSet(dp[j][i-1]);
            HourSet refe = HourSet(dp[j-1][i-1]);
            refe.insert(permutation[i-1]);
            if (-refe.combineValues(lambda, phi) > -dp[j][i].combineValues(lambda, phi)) {
                dp[j][i] = HourSet(refe);
            }
        }
    }
    
    return dp[W][n];
}

HourSet knapsackNoRepetitionsDemand(int W, HourSet &init) {
    int n = (int) permutation.size();
    vector< vector< HourSet > > dp(W+1, vector<HourSet>(n+1));
    for (int i = 0; i <= n; i++) dp[0][i] = HourSet(init);
    for (int i = 0; i <= W; i++) dp[i][0] = HourSet(init);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            dp[j][i] = HourSet(dp[j][i-1]);
            HourSet refe = HourSet(dp[j-1][i-1]);
            refe.insert(permutation[i-1]);
            if (-refe.secondValueFn() > -dp[j][i].secondValueFn()) {
                dp[j][i] = HourSet(refe);
            }
        }
    }

    return dp[W][n];
}

void solve() {
 
    int n, m; cin >> n >> m;
    a.resize(n);
    int totalDemand = 0;
    forn(i, n) {
        cin >> a[i];
        totalDemand += a[i];
    }
    for (int i = 0; i <= 10; i++) {
        generateHours(0LL, i, -1, false);
    }

    cout << permutation.size() << endl;

    int maxReduced = 0;
    HourSet ans;
    ans = knapsackNoReptetitions(3, 2, 1);

    ans = knapsackNoRepetitionsDemand(m-3, ans);
    ans.printHours();
    cout << "Total demand: " << totalDemand << endl;
    cout << "Total demand remaining: " << ans.secondValueFn() << endl;
    cout << "Reduced: " << totalDemand - ans.secondValueFn() << endl;
}
 
int main() {
    cin.tie(0)->sync_with_stdio(0); 
    cin.exceptions(cin.failbit);
 
    int T = 1;
    //cin >> T;
    while(T--) {
        solve();
    }
 
	return 0;
}
 
