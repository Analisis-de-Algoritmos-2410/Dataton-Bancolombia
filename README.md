# Dataton 2023 - Bancolombia

https://www.kaggle.com/datasets/cvelas24/dataton-2023/data

Solucion en excel: https://livejaverianaedu-my.sharepoint.com/:x:/g/personal/sfelipe_galindor_javeriana_edu_co/EZ9yPIPtb0NKtCGL3khvqEkB_yqKtOFV6vNZBBjy7lxwKw?e=CQSfYA

## Etapa 1

Tener en cuenta los estados posibles de los trabajadores

1. __Trabaja:__ El empleado está disponible para atender clientes.
2. __Pausa Activa:__ El empleado no está disponible para atender clientes.
3. __Almuerza:__ El empleado está tomando su tiempo de almuerzo (no disponible para atender clientes)
4. __Nada:__ El empleado no ha comenzado su jornada laboral o ya la terminó.

Formalmente, el problema consiste:

Defina $a_i$ como la demanda en la franja horaria $i$. Hay $m$ franjas horarias disponibles en el día.

Defina $f(i, j)$ como la disponibilidad del trabajador $j$ en la franja horaria $i$. Hay $n$ trabajadores y $f(i, j) \in \{0, 1\}$ donde $1$ indica que el trabajador esta trabajando y $0$ indica que el trabajador esta descansando, almorzando o en pausa activa. Hay $n$ trabajadores.

Hay que encontrar los valores para $f(i, j)$ para todo $1\le j \le n, 1\le i \le m$ tal que:

$$\min\limits_{1 \le i \le m} \{a_i- \sum\limits_{j=1}^{n} f(i, j) \}$$

Para cada uno de los valores de $f(i, j)$ deben cumplir con algunas restricciones:

- Los empleados deben trabajar minimo $1$ hora continua, equivale a $4$ franjas horarias continuas.
- Los empleados deben trabajar máximo $2$ horas de forma continua, equivale a $8$ franjas horarias. En la franja siguiente debe tomar si bien almuerzo o pausa activa.
- El almuerzo es $1$ hora y media de forma continua, equivale a $6$ franjas horarias continuas.
- La hora minima para tomar almuerzo son las 11.30 am ($i=17$) y la hora máxima para salir de almuerzo es 1.30 pm ($i = 25$).
    - Una persona __NO__ puede salir de almuerzo a las 11.15 am ($i=16$).
    - Una persona __NO__ puede salir de almuerzo a las 1.45 pm ($i = 26$).
    - Es valido que una persona tome almuerzo a las 1.30 pm ($i=25$).
- La jornada laboral de todas las personas es de $8$ horas diarias $i=32$, el almuerzo no constituye la hora laboral ($i=38$). 


### Posible solución

Tenga en cuenta que las franjas horarias se pueden representar como un numero de $46$ bits, teniendo en cuenta esto y las restricciones, es posible generar todas las posibles asignaciones de horario para los empleados, con ayuda de operaciones como el _bitwise OR_ y el _left shifting_. Aproximadamente hay $6336$ posibles horarios para los trabajadores, defina la cantidad de posibles horarios como $n \approx 6\cdot10^3$.

Teniendo los posibles horarios, podemos ver el problema como un problema de la mochila, un problema clasico de programación dinámica. Haciendo la analogía con el problema de la mochila (Knapsack 0/1) tome como $W=8$, $W$ en el problema original va a ser la capacidad de la mochila, si tomamos todos los posibles horarios con un peso de $w_i = 1$ entonces podemos decir que la mochila va a tener una capacidad de $W=8$ ya que son $8$ trabajadores.

Tomando en cuenta esto, podemos saber cual subset de $8$ elementos podemos tener en la mochila tal que se minimice la demanda, sin embargo, el valor para cada uno de los elementos se va a tener que calcular de forma dinámica para poder saber que horario podría minimizar la demanda.

Con el Knapsack iterativo podemos tener en cuenta los elementos que se eligieron para cada tamaño, debido a esto, vamos a almacenar cual fue la demanda que se calculo para cada _tamaño_ de empleados, de esta forma, cuando se vaya a evaluar el valor de un horario, se tiene que sumar ese horario a los horarios ya escogidos por el algoritmo. Otra de las restricciones importantes para el algoritmo, puede ser que hay que tener en cuenta las soluciones que no cubren todas las franjas horarias, entonces debemos interesarnos en las soluciones que si cumplan con dicha condición, esto va a significar una nueva dimensión en la tabulación de resultados de la programación dinámica. Defina $dp_{ijk}$ como la minima demanda para $i$ empleados teniendo en cuenta los primeros $j$ posibles horarios, siendo $k$ el indicador que la solucion de $dp_{ij}$ cumple la condición de llenar todos los horarios.

De igual manera, para poder calcular la demanda cubierta y la restante, por cada iteración del algoritmo de Knapsack se tiene que verificar las $42$ zonas horarias que se estan verificando para la solución que se este calculando. Teniendo en cuenta esto y que los posibles horarios es $n \approx 6\cdot 10^3$ podemos concluir que la complejidad total del algoritmo es de $O(n + nWk\cdot 42)$. En este caso se toma en cuenta cosas como por ejemplo el tener que precalcular los posibles horarios y las comparaciones hechas para todas las franjas horarias en cada iteración del Knapsack y se toma en cuenta $k$ que solo va a ser $1$ o $0$ en caso de que cumpla o no.

```cpp!
// Sebastian Galindo

#include <bitset>
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

vector<ll> permutation;

void printHours(ll g) {
    for (int i = 0; i <= 45; i++) {
        if ((g >> i) & 1) cout << 1;
        else cout << 0;
    }
    cout << endl;
}

void printTable(ll g) {
    int c = 7;
    int m = 30;
    forn(i, 46) {
        cout << c << ":" << (m==0 ? "00" : to_string(m)) << " -> " << ((g >> i)&1 ? "Trabajando" : "Pausa activa") << endl;
        m += 15;
        if (m == 60) {
            c++;
            m = 0;
        }
    }
    cout << __builtin_popcountll(g) << endl;
}

bool deb = false;

void generateHours(ll h, int b, int pausas, bool lunch) {
//    if (deb) cout << bitset<46>(h) << endl;
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
        if (b+j >= 15 && b+j <= 23) {
            generateHours((h | (k << b)), b+(j+6), pausas, true);
            generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
        } else if (b+j == 45) generateHours((h | (k << b)), b + (j+1), pausas, lunch);
        else  generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
    }
}


vector<ll> a;

ll f(vector<ll>& d) {
    ll ans = 0;
    forn(i, 46) {
        ans += (d[i]-a[i]);
    }
    return ans;
}

void sumArray(vector<ll> &curr, ll b, vector<ll>&t) {
    for (int i = 0; i <= 45; i++) {
        if ((b >> i) & 1) t[i] = curr[i]+1;
        else t[i] = curr[i];
    }
}

vector<ll> knapsackNoReptetitions(int W) {

    int n = permutation.size();
    vector< vector< vector<ll> > > dp(W+1, vector< vector<ll> >(n+1, vector<ll>(46, 0)));
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            dp[j][i] = dp[j][i-1];
            vector<ll> ns(46, 0);
            sumArray(dp[j-1][i-1], permutation[i-1], ns);
            if (f(dp[j][i]) < f(ns)){
                dp[j][i] = vector<ll>(all(ns));
            }
        }
    }
    
    return dp[W][n];
}

void solve() {
 
    int n; cin >> n;
    a.resize(n);
    forn(i, n) cin >> a[i];
    for (int i = 0; i <= 10; i++) {
        generateHours(0LL, i, -1, false);
    }
    cout << permutation.size() << endl;
    
    for (auto u: knapsackNoReptetitions(8)) cout << u << " ";
    cout << endl;
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
 


```

Input:

```
46
2 2 4 7 8 8 9 8 6 8 9 9 9 12 16 16 16 13 12 11 13 14 10 9 9 11 8 9 10 8 9 9 5 5 4 6 8 4 5 1 3 2 2 2 2 2
```


Sin heuristica:
```
4 4 4 4 4 4 7 5 8 8 8 8 8 7 5 6 4 4 4 4 4 6 6 4 4 4 4 4 6 5 7 8 8 8 8 8 5 7 4 4 4 4 4 4 4 0 
```

Una de las primeras observaciones que se tiene que hacer es que debe existir minimo una persona en cada franja de horario, dicha restriccion se puede llegar a cumplir si hacemos primero un Knapsack para minimizar horarios vacios. Es importante extraer un conjunto que sea pequeño y que pueda cubrir las restricciones indicadas. Se puede llegar a minimizar aún mas el horario dependiendo de la demanda, sin embargo, hay que segurar primero la restricción.

Teniendo en cuenta esto, primero se tiene que hacer una función para poder evaluar el valor ponderado de la permutación que se esta calculando.

Para poder cumplir la restricción entonces se propone la siguiente función de ponderación:

\begin{align}
\mathrm{firstValueFn} &= (\sum_{i = 1}^{46}f(i))\\
\mathrm{secondValueFn} &= (\sum_{i = 1}^{46}f2(i))
\end{align}

$$f(i) \begin{cases}
    1 & \text{if} \sum\limits_{j=1}^np_{ji} = 0 \\
    0 & \text{otherwise}
\end{cases}$$

$$f2(i) = \max(a_i-\sum_{j = 1}^np_{ji}, 0)$$

Donde $n$ es la cantidad de horarios que se estan evaluando, $p_{ji}$ es el estado del horario $j$ en la franja horaria $i$ y $a_i$ es la demanda en la franja horaria $i$. En este caso se debe tener en cuenta la cantidad de horarios, ya que si queremos minimizar dicha función, la permutación de horarios que minimice dicha función va a tener en cuenta el tamaño para minimizarlo igualmente. **Se define de una vez la función para minimar la demanda total**.

En este caso, la idea fue probar con cuantos es lo minimo que se puede lograr cubrir todas las zonas horarias, esta primera fase del Knapsack es para poder asegurar que podemos obtener una permutacion de horarios los cuales cumplan con la restricción, se concluyó que se debe tener minimo $3$ horarios para cubrir todos los horarios, pero surgieron algunas ideas que pudieron dar con la minimizacion:

### ¿Y si usamos una función combinada?

En cierta parte del desarrollo del algoritmo, se tenia la idea en un principio de asegurar primero todos los horarios, sin embargo, ¿Que pasaría si ponderamos los horarios vacíos y agregamos un peso a la demanda total para que la minimice de una vez?

En ese caso se elabora una funcion:

$$\mathrm{combineValues} = \mathrm{firstValueFn} + \mathrm{secondValueFn}$$

En ese caso, dicha minimizacion resulto en los siguientes horarios:
```
1111110111111110000001111111101111111100000000
0111101111110111111110000001111011111110000000
0011110111101111100000011111111011111111000000
0001111110111111110000001111111101111111100000
0001111111101111111100000011111111011111100000
0000011111101111111100000011111111011111111000
0000001111011111110000001111111101111011111110
0000001111011111110000001111111101111101111110
Total demand: 355
Total demand remaining: 139
Reduced -> 216
```

Pero se puede visualizar que la ultima franja horaria (La ultima columna) no cumple con las restricciones del problema, el cual indica que debe existir un trabajador en todas las franjas como minimo. Para contrarrestar esto se decidió darle una ponderación mayor a las franjas horarias vacías, entonces ahora la función queda de la siguiente manera:

$$\mathrm{combineValues} = \mathrm{firstValueFn} \cdot \lambda + \mathrm{secondValueFn} \cdot \phi$$

Donde $\lambda, \phi$ son factores que alteran la ponderación de las funciones. En este caso, si se toma $\lambda = 8, \phi = 1$ tomando con mas importancia las franjas vacías, entonces se obtiene el siguiente resultado:

```
1111110111111110000001111111101111111100000000
0000111111110111111110000001111110111111110000
0000111111110111111110000001111111011111110000
0000111111110111111110000001111111101111110000
0000011110111101111000000111101111011111111000
0000001111011110000001111011110111101111011110
0000001111011111110000001111111101111011111110
0000000011110111100000011110111101111011111111
Total demand: 355
Total demand remaining: 150
Reduced: 205
```

Donde se puede ver que se reduce la demanda $11$ puntos menos, este puede ser un tradeoff entre las restricciones de franjas horarias, sin embargo, si se toma un factor diferente para $\lambda = 2$ se obtienen los siguientes resultados:

```
1111110111111110000001111111101111111100000000
0001111011110111111110000001111101111111100000
0001111110111111110000001111111101111111100000
0001111111011111110000001111111101111111100000
0001111111101111111100000011111111011111100000
0000111111011111111000000111111110111111110000
0000001111011110000001111011110111101111011110
0000000011110111111000000111111110111101111111
Total demand: 355
Total demand remaining: 141
Reduced: 214

```

Este reduce la diferencia de demanda reducida cumpliendo las restricciones, entonces surgió la siguiente pregunta:

### ¿Cuales valores para $\lambda$ y $\phi$ son mejores?

Para realizar esto, se prueba con todos los posibles pares de $1 \le \lambda, \phi \le 10$, iterando todos los posibles valores para los factores se decide que los mejores factores son: $\lambda = 4, \phi = 3$

```
1111110111111110000001111111101111111100000000
0001111011110111111110000001111101111111100000
0001111110111111110000001111111101111111100000
0001111111011111110000001111111101111111100000
0001111111101111111100000011111111011111100000
0000111111011111111000000111111110111111110000
0000001111011110000001111011111110111101111110
0000000011110111111000000111111110111101111111
Total demand: 355
Total demand remaining: 140
Reduced: 215
Lambda: 4 , Phi: 3
```

En este caso se pudo reducir solamente en $1$ punto cumpliendo las restricciones. El codigo completo antes de la siguiente estrategia es el siguiente:

```cpp!
// Sebastian Galindo

#include <algorithm>
#include <bitset>
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

vector<ll> permutation;

void printHours(ll g) {
    for (int i = 0; i <= 45; i++) {
        if ((g >> i) & 1) cout << 1;
        else cout << 0;
    }
    cout << endl;
}

void printTable(ll g) {
    int c = 7;
    int m = 30;
    forn(i, 46) {
        cout << c << ":" << (m==0 ? "00" : to_string(m)) << " -> " << ((g >> i)&1 ? "Trabajando" : "Pausa activa") << endl;
        m += 15;
        if (m == 60) {
            c++;
            m = 0;
        }
    }
    cout << __builtin_popcountll(g) << endl;
}

bool deb = false;

void generateHours(ll h, int b, int pausas, bool lunch) {
//    if (deb) cout << bitset<46>(h) << endl;
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
        if (b+j >= 15 && b+j <= 23) {
            generateHours((h | (k << b)), b+(j+6), pausas, true);
            generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
        } else if (b+j == 45) generateHours((h | (k << b)), b + (j+1), pausas, lunch);
        else  generateHours((h | (k << b)), b + (j+1), pausas+1, lunch);
    }
}


vector<ll> a;

ll f(vector<ll>& d) {
    ll ans = 0;
    forn(i, 46) {
        ans += (d[i]-a[i]);
    }
    return ans;
}

void sumArray(vector<ll> &curr, ll b, vector<ll>&t) {
    for (int i = 0; i <= 45; i++) {
        if ((b >> i) & 1) t[i] = curr[i]+1;
        else t[i] = curr[i];
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

void solve() {
 
    int n; cin >> n;
    a.resize(n);
    int totalDemand = 0;
    forn(i, n) {
        cin >> a[i];
        totalDemand += a[i];
    }
    for (int i = 0; i <= 10; i++) {
        generateHours(0LL, i, -1, false);
    }

    int maxReduced = 0, ansLambda, ansPhi;
    HourSet ans;
    ans = knapsackNoReptetitions(8, 4, 3);
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
 

```

### Volver a la estrategia inicial

En teoría, esta heuristica fue un experimento, la estrategia inicial es:

1. Hacer un Knapsack para asegurar los espacios vacíos de los horarios.
2. Hacer un Knapsack para minimizar la demanda.

Pero entonces el primer paso lo podemos hacer con estos parametros y con la funcion combinada para poder obtener los $3$ horarios que sean mejores para llenar los espacios de los horarios, entonces primero se hace un knapsack con los parametros $W=3, \lambda=4, \phi=3$ para obtener dichos horarios de la major manera:

```
1111110111111110000001111111101111111100000000
0000001111111011111111000000111111110111111110
0000000011111111000000111111110111111110111111
Total demand: 355
Total demand remaining: 264
Reduced: 91
```
_Casualmente, para $W=3$, la variacion de los parametros en $\lambda$ y $\phi$ dan el mismo resultado._

Ya con este conjunto de horarios se puede hacer un knapsack para minimizar la demanda, comenzando con estos horarios calculados.

Con la ejecución del alogoritmo en estas dos fases, se obtuvieron los siguientes resultados:

```
1111110111111110000001111111101111111100000000
0000001111111011111111000000111111110111111110
0000000011111111000000111111110111111110111111
1111110111111110000001111111101111111100000000
0111101111101111111100000011111111011110000000
0111111110111111110000001111111101111110000000
0011111101111011110000001111111101111111000000
0001111101111011110000001111111101111111100000
Total demand: 355
Total demand remaining: 135
Reduced: 220
```

Superando por $5$ puntos de demanda a el resultado calculado con la heuristica de darle ponderación a cada uno de las funciones de valor.

### Descripción general del algoritmo

El algoritmo se basa en el hecho de que existen $n \approx 6\cdot 10^3$ horarios posibles válidos siendo $46$ franjas horarias. Tomando esto en cuenta, se puede llegar a tomar un algoritmo en tiempo hasta cuadratico con respecto a $n$.

Cada uno de los horarios se puede definir como $h$ siendo una colección de $0$s y $1$s donde $\mathcal{h}_i = 1$ indica que se esta trabajando en esa franja horaria $i$ y $\mathcal{h}_i = 0$ que dicha franja horaria $i$ esta en _Pausa activa_ o en _Almuerzo_. Defina el conjunto de todos estos horarios como $\mathcal{H}$, ($h \in \mathcal{H}, |\mathcal{H}|=n$ )

Con los posibles horarios generados entonces se puede usar una estrategia de programación dinámica para minimizar la demanda. Defina la demanda como $a$, donde $a_i$ es la demanda en la franja horaria $i$.

Especificamente se puede usar una analogía con el problema de la mochila (Knapsack). Suponga que se tiene una mochila con capacidad $W$ y varios items, que cada uno de los items tienen un valor $v_k$ y un peso $w_k$ (Donde $k$ es el indice del item). Si nosotros tomamos que la mochila es de alguna u otra manera la sucursal y tiene una capacidad de $W$ **trabajadores**, entonces podemos tomar que cada uno de los items son **los horarios** $\mathcal{h}$, los cuales tienen un peso $w_k = 1$ y un valor $v_k$ dinámico con respecto a los horarios $\mathcal{h}$ que ya estan en la mochila.

Para calcular cada valor $v_k$ defina $P$ como una combinación de $\mathcal{H}$ donde $P_j$ es el $j$ horario $h$ de la combinación. Entonces para cualquier horario $h$, se toma $v_k$ como la siguiente función tomando en cuenta una combinación $P$ esta dentro de la mochila:

$$v_k=\sum_{i = 1}^{46}\max(a_i-\sum_{j=1}^{|P|}P_{ji}, 0)$$

Se tiene que tomar en cuenta que antes de calcular a $v_k$, temporalmente se agrega el horario $h$ a el conjunto $P$. Para efectos de la descripción del algoritmo, defina P
para el conjunto $P$ defina las siguientes funciones:

$$f2(i)=\max(a_i-\sum_{j=1}^{|P|}P_{ji}, 0)$$

$$\mathrm{secondValueFn} = \sum_{i = 1}^{46}f2(i)$$

Siendo $i$ la zona horaria.

Para poder asegurar que se cumpla la restricción de que todas las zonas horarias hay almenos un trabajador, se propone la siguiente heuristica:

Se puede hacer una nueva analogía con el problema de la mochila, pero que los valores $v_k$ representen la cantidad de franjas horarias las cuales no estan ocupadas por ningún trabajador. Entonces, se dan las siguientes funciones en el conjunto $P$.

\begin{align}
v_k =\mathrm{firstValueFn} &= (\sum_{i = 1}^{46}f(i))\
\end{align}

$$f(i) \begin{cases}
    1 & \text{if} \sum\limits_{j=1}^{|P|}p_{ji} = 0 \\
    0 & \text{otherwise}
\end{cases}$$

Una observación clave, es que según la respuesta del algoritmo solución del problema de la mochila, se deben tener como minimo $3$ trabajadores para poder cubrir todas las franjas horarias. En ese caso, existen varias posibilidades de $3$ horarios y algunas de ellas minimizan mas la demanda que otras. Para tomar en cuenta esto en la solución de la primera fase de tomar un subset de $3$ horarios, se establece que la mochila debe tener la siguiente función:

$$v_k = \mathrm{combineValues} = \mathrm{firstValueFn} \cdot \lambda + \mathrm{secondValueFn} \cdot \phi$$

Donde $\lambda, \phi$ son factores que alteran la ponderación de las funciones. Entonces se concluye que es mejor tomar un $\lambda > \phi$ para que se pueda satisfacer la restriccion de los horarios.

Una vez obtenidos los $3$ horarios que minimizan la demanda y cubren todas las franjas horarias, entonces se puede tomar el problema de la mochila con el $v_k$ inicial y minimizar la demanda para $5$ horarios mas en la mochila. Con el segundo knapsack se asegura que se puede minimizar mas la demanda tomando en cuenta las restricciones de las franjas horarias.

El algoritmo ejecuta $3$ fases identificadas:

1. Generacion de los posibles horarios.
2. Ejecución del algoritmo de Knapsack para los $3$ primeros horarios, cumpliendo la restricción de que todas las franjas de horarios tienen que estar ocupadas.
3. Ejecución del algoritmo de Knapsack para complementar los $5$ horarios faltantes minimizando solamente la demanda.

La complejidad total del algoritmo es:

\begin{align}
O(n) + &O(n\mathcal{C}\cdot46) + O(nW\cdot46)\\
&O(n\cdot(\mathcal{C} + W))
\end{align}

Donde $\mathcal{C}$ es la cantidad de horarios necesarios para llenar todas las franjas de horarios y $W$ es la cantidad de trabajadores restantes por asignar horario. Hay que tener en cuenta que esta complejidad tiene una constante alta, ya que se omiten algunos pasos que pueden llegar a resultar en $10^2$ operaciones, aumentando el tiempo de ejecución. En promedio el algoritmo en C++ corre en $\approx 0,69s$.

#### Input

En la primera linea del input, se tienen dos enteros $n, m$ donde $n$ es la cantidad de franjas horarias y $m$ la cantidad de trabajadores en la sucursal.

En la segunda linea hay $n$ enteros indicando la demanda por cada zona horaria.

En el caso de los datos de Kaggle, se tiene el siguiente input.

```
46 8
2 2 4 7 8 8 9 8 6 8 9 9 9 12 16 16 16 13 12 11 13 14 10 9 9 11 8 9 10 8 9 9 5 5 4 6 8 4 5 1 3 2 2 2 2 2
```

El resultado del programa y los horarios calculados son:

```
1111110111111110000001111111101111111100000000
0000001111111011111111000000111111110111111110
0000000011111111000000111111110111111110111111
1111110111111110000001111111101111111100000000
0111101111101111111100000011111111011110000000
0111111110111111110000001111111101111110000000
0011111101111011110000001111111101111111000000
0001111101111011110000001111111101111111100000
Total demand: 355
Total demand remaining: 135
Reduced: 220
```

El codigo final se puede visualizar en el siguiente link: https://ide.usaco.guide/NyW5f5TMMl8uYbX_ZoL
