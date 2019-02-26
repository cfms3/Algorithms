template <typename T>
vector<int> Zfunc(T s) {
  int n = s.size(), a = 0, b = 0;
  vector<int> z(n, 0);
  z[0] = n;
  for (int i = 1; i < n; i++) {
    int end = i;
    if (i < b) {
      end = min(i + z[i - a], b);
    }
    while (end < n && s[end] == s[end - i]) {
      end++;
    }
    z[i] = end - i;
    if (end > b) {
      a = i;
      b = end;
    }
  }
  return z;
}