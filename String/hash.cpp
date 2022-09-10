using ull = uint64_t;

const ull MOD = (1LL << 61) - 1;
const int base = 31;
const int MAX_POT = 1000100;

ull p[MAX_POT];

ull modMul(uint64_t a, uint64_t b) {
  ull l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
  ull l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
  ull ret = (l & MOD) + (l >> 61) + (h << 3) + (m >> 29) + ((m << 35) >> 3) + 1;
  ret = (ret & MOD) + (ret >> 61);
  ret = (ret & MOD) + (ret >> 61);
  return ret - 1;
}

void buildHashPot() {
  p[0] = 1;
  for (int i = 1; i < MAX_POT; i++) p[i] = modMul(base, p[i - 1]);
}

struct Hash {
  ull val;
  int size;
  Hash(ull val = 0, int size = 0): val(val), size(size) {}
};

// maior pot vai ficar no l
Hash combine(Hash l, Hash r) {
  ull val = modMul(p[r.size], l.val);
  return Hash(val + r.val, l.size + r.size);
}

struct StringHash {
  // 0 based
  // pref[i] = hash da string 0..i, maior pot ta no 0
  // suff[i] = hash da string i..n-1, maior pot ta no n-1
  int n;
  vector<Hash> pref, suff;

  ull getInt(char c) {
    return c - 'a' + 1;
  }

  StringHash(string &s) {
    // remember to call it in the main
    // buildHashPot();
    n = s.size();
    pref.resize(n); 
    suff.resize(n);
    pref[0] = Hash(getInt(s[0]), 1);
    for (int i = 1; i < n; i++) {
      Hash cur = Hash(getInt(s[i]), 1);
      pref[i] = combine(pref[i - 1], cur);
    }
    suff[n - 1] = Hash(getInt(s[n - 1]), 1);
    for (int i = n - 2; i >= 0; i--) {
      Hash cur = Hash(getInt(s[i]), 1);
      suff[i] = combine(suff[i + 1], cur);
    }
  }

  // [l, r]
  // maior pot vai estar no l
  Hash getHash(int l, int r) { 
    ull res = pref[r].val;
    if(l > 0) res = (res + MOD - modMul(p[r - l + 1], pref[l - 1].val)) % MOD;
    return Hash(res, r - l + 1);
  }

  // [l, r]
  // maior pot vai estar no r
  Hash getRevHash(int l, int r) {
    ull res = suff[l].val;
    if (r < n - 1) res = (res + MOD - modMul(p[r - l + 1], suff[r + 1].val)) % MOD;
    return Hash(res, r - l + 1);
  }
};