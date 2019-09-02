#include <bits/stdc++.h>
using namespace std;

#define SIZE 1000010
#define EXP_SIZE 2

const int mod = 20190601;

int primes[SIZE], minFac[SIZE], pfx[SIZE][EXP_SIZE], primesPt, lim;
long long int g[SIZE * EXP_SIZE][EXP_SIZE], dsc[SIZE * EXP_SIZE], inv2, inv3;
pair<int, int> indx[SIZE];  // indx[x]: index of <x, n / x>
const pair<int, int> csts[EXP_SIZE] = {make_pair(-2, 0), make_pair(1, 1)};

long long int fastPow(long long int a, long long int n, long long int mod) {
    long long int ret = 1; a %= mod;
    while (n > 0) {
        if (n & 1)
            ret = (ret * a) % mod;
        a = (a * a) % mod; n >>= 1;
    }
    return ret;
}

void initPrimes(int siz) {
    inv2 = fastPow(2, mod - 2, mod); inv3 = fastPow(3, mod - 2, mod);
    fill(minFac + 0, minFac + siz + 1, 0); primesPt = 0;
    for (int i = 2; i <= siz; i++) {
        if (minFac[i] == 0)
            minFac[i] = i, primes[primesPt++] = i;
        for (int j = 0; j < primesPt && primes[j] <= min(minFac[i], siz / i); j++)
            minFac[i * primes[j]] = primes[j];
    }

    for (int e = 0; e < EXP_SIZE; e++)
        pfx[0][e] = fastPow(primes[0], csts[e].second, mod);
    for (int i = 1; i < primesPt; i++)
        for (int e = 0; e < EXP_SIZE; e++)
            pfx[i][e] = (pfx[i - 1][e] + fastPow(primes[i], csts[e].second, mod)) % mod;
}

const auto sum = [](long long int n, long long int exp) {
    n %= mod;
    if (exp == 0)
        return n;
    long long int ret = n * (n + 1) % mod * inv2 % mod;
    if (exp == 2)
        return ret * ((n << 1) + 1) % mod * inv3 % mod;
    return ret;
};

long long int min25(long long int n) {
    lim = sqrt(n); initPrimes(lim); int dscPt = 0;
    for (long long int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l); long long int v = n / l; dsc[dscPt] = v;
        for (int e = 0; e < EXP_SIZE; e++)
            g[dscPt][e] = sum(dsc[dscPt], csts[e].second) - 1;
        v <= lim ? indx[v].first = dscPt : indx[n / v].second = dscPt; dscPt++;
    }

    long long int ret = 0;
    for (int i = 0; i < primesPt; i++) {
        long long int prev = g[0][0];
        for (int j = 0; j < dscPt && 1ll * primes[i] * primes[i] <= dsc[j]; j++) {
            long long int v = dsc[j] / primes[i];
            int k = v <= lim ? indx[v].first : indx[n / v].second;
            for (int e = 0; e < EXP_SIZE; e++)
                g[j][e] = (g[j][e] - fastPow(primes[i], csts[e].second, mod) * (g[k][e] - (i == 0 ? 0 : pfx[i - 1][e]) + mod) % mod + mod) % mod;
        }
        ret = (ret + 1ll * (primes[i] - 1) * (prev - g[0][0]) % mod) % mod;
    }

    for (int e = 0; e < EXP_SIZE; e++)
        ret = (ret + csts[e].first * g[0][csts[e].second] % mod + mod) % mod;
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    long long int num; cin >> num;
    cout << min25(num) << '\n';

    return 0;
}