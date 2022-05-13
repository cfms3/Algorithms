#include <bits/stdc++.h>
using namespace std;

/*
  fib(m + n) = fib(m) * fib(n + 1) + fib(m - 1) * fib(n)
  gcd(fib(n), fib(m)) = fib(gcd(n, m))
*/

/*
Dirichlet Convolution

(f * g)(n) = sum{d | n}{f(d) * g(n / d)}
mi * 1 = e => (g = f * 1 <=> f = g * mi)
phi * 1 = Id
sf(n) = (s(f*g)(n) - sum{2 <= d <= n}{sf(floor(n / d)) * g(d)}) / g(1)
*/
struct ModularArithmeticFunctionPrefixSum {
  unordered_map<long long, long long> dp;
  vector<long long> precalculatedPrefixSumF;
  long long mod;

  ModularArithmeticFunctionPrefixSum(vector<long long> precalculatedPrefixSumF, long long mod) : precalculatedPrefixSumF(precalculatedPrefixSumF), mod(mod) {}

  long long fexp(long long b, long long e = -1) {
    if (e < 0) e += mod - 1;
    long long ans = 1;
    while (e > 0) {
      if (e & 1) ans = ans * b % mod;
      b = b * b % mod;
      e >>= 1; 
    }
    return ans;
  }

  long long prefixG(long long x) {
    // implement here s_g(x)
    // example: 
    // g = 1
    if (x <= 0) {
      return 0;
    }
    return x % mod;
  }

  const long long inv2 = fexp(2);

  long long prefixFG(long long x) {
    // implement here s_(f * g)(x)
    // example: 
    // f = phi
    // g = 1
    // f * g = Id
    if (x <= 0) {
      return 0;
    }
    x %= mod;
    return x * (x + 1) % mod * inv2 % mod;
  }

  long long prefixF(long long x) {
    if (x < precalculatedPrefixSumF.size()) {
      return precalculatedPrefixSumF[x];
    }
    if (dp.count(x)) {
      return dp[x];
    }
    long long ans = prefixFG(x);
    for (long long lowerBound = 2, upperBound; lowerBound <= x; lowerBound = upperBound + 1) {
      upperBound = x / (x / lowerBound);
      long long intervalG = (prefixG(upperBound) - prefixG(lowerBound - 1) + mod) % mod;
      ans = (ans - prefixF(x / lowerBound) % mod * intervalG % mod + mod) % mod;
    }
    long long g1 = (prefixG(1) - prefixG(0) + mod) % mod;
    ans = ans * fexp(g1) % mod;
    assert(g1 * fexp(g1) % mod == 1);
    dp[x] = ans;
    return ans;
  }
};

const long long mod = 998244353L;
long long acc[2][2], base[2][2], temp[2][2];

long long fib(long long x) {
  if (x == 0) return 0;

  base[0][0] = 1;
  base[0][1] = 1;
  base[1][0] = 1;
  base[1][1] = 0;
  acc[0][0] = 1;
  acc[0][1] = 0;
  acc[1][0] = 0;
  acc[1][1] = 1;

  x--;
  while (x > 0) {
    if (x & 1) {
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          temp[i][j] = 0;
          for (int k = 0; k < 2; k++) {
            temp[i][j] = (temp[i][j] + base[i][k] * acc[k][j] % mod) % mod;
          }
        }
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          acc[i][j] = temp[i][j];
        }
      }
    }

    x >>= 1;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        temp[i][j] = 0;
        for (int k = 0; k < 2; k++) {
          temp[i][j] = (temp[i][j] + base[i][k] * base[k][j] % mod) % mod;
        }
      }
    }
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        base[i][j] = temp[i][j];
      }
    }
  }

  return acc[0][0];
}

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  const int mx = 5000000;
  vector<int> primes, phi(mx);
  vector<bool> isComposite(mx, false);

  phi[1] = 1;

  for (int i = 2; i < mx; i++) {
    if (!isComposite[i]) {
      primes.push_back(i);
      phi[i] = i - 1;
    }
    //if (i < 30) cout << i << ' ' << phi[i] << endl;
    for (int p: primes) {
      if (i * p >= mx) break;
      isComposite[i * p] = true;
      if (i % p == 0) {
        phi[i * p] = p * phi[i];
        break;
      } else {
        phi[i * p] = phi[i] * phi[p];
      }
    }
  }

  vector<long long> sumPhi(mx);
  sumPhi[0] = phi[0];
  for (int i = 1; i < mx; i++) {
    sumPhi[i] = (1ll * phi[i] + sumPhi[i - 1]) % mod;
  }

  ModularArithmeticFunctionPrefixSum phiCalculator(sumPhi, mod);

  //cout << phiCalculator.prefixF(1ll * 10000000000) << endl;

  long long n;
  cin >> n;

  long long ans = 0;
  for (long long lowerBound = 1, upperBound; lowerBound <= n; lowerBound = upperBound + 1) {
    upperBound = n / (n / lowerBound);
    long long fibSum = (fib(upperBound + 2) - 1 - fib(lowerBound + 1) + 1 + mod) % mod;
    long long lim = n / lowerBound;
    assert(n / lowerBound == n / upperBound);
    
    ans = (ans + (2 * phiCalculator.prefixF(lim) - 1 + mod) % mod * fibSum % mod) % mod;
  }

  cout << ans << endl;

  return 0;
}