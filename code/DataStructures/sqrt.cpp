#include <bits/stdc++.h>
using namespace std;

const int block = 500;

struct Node_t {
  vector<char> value;
  int size;

  Node_t() {
    size = 0;
  }
};

int n;
string s;
vector<Node_t> buffer;
vector<int> pos;

void rebuild() {
  pos.clear();
  int i = 0;
  Node_t cur = Node_t();
  while (i < n) {
    if (i % block == 0 && i > 0) {
      pos.push_back(buffer.size());
      buffer.push_back(cur);
      cur = Node_t();
    }
    cur.size++;
    cur.value.push_back(s[i]);
    i++;
  }
  if (cur.size > 0) {
    pos.push_back(buffer.size());
    buffer.push_back(cur);
  }
}

void take() {
  s = "";
  for (int i = 0; i < (int) pos.size(); i++) {
    for (int j = 0; j < buffer[pos[i]].size; j++) {
      s.push_back(buffer[pos[i]].value[j]);
    }
  }
}

int split(int id) {
  int i = 0, on = 0;
  while (i < (int) pos.size() && on < id) {
    on += buffer[pos[i]].size;
    i++;
  }
  if (on == id) {
    return i;
  }
  i--;
  on -= buffer[pos[i]].size;
  Node_t new_l = Node_t(), new_r = Node_t();
  int x = 0;
  while (on < id) {
    new_l.value.push_back(buffer[pos[i]].value[x++]);
    new_l.size++;
    on++;
  }
  while (x < buffer[pos[i]].size) {
    new_r.value.push_back(buffer[pos[i]].value[x++]);
    new_r.size++;
  }
  pos[i] = buffer.size();
  buffer.push_back(new_l);
  pos.insert(pos.begin() + i + 1, buffer.size());
  buffer.push_back(new_r);
  return i + 1;
}

//[l, r], [0...]
void rotate(int l, int r, int k) {
  int id_l = split(l);
  int id_mid = split(l + k);
  int id_r = split(r + 1);
  vector<int> to_put;
  for (int i = id_l; i < id_mid; i++) {
    to_put.push_back(pos[i]);
  }
  for (int i = id_mid; i < id_r; i++) {
    pos[id_l + i - id_mid] = pos[i];
  }
  for (int i = 0; i < id_mid - id_l; i++) {
    pos[id_l + id_r - id_mid + i] = to_put[i];
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cin >> s;
  n = (int) s.size();
  rebuild();
  int m;
  cin >> m;
  vector<int> l(m), r(m), k(m);
  for (int i = 0; i < m; i++) {
    cin >> l[i] >> r[i] >> k[i];
    l[i]--;
    r[i]--;
  }
  for (int i = m - 1; i >= 0; i--) {
    if (i % block == 0) {
      take();
      rebuild();
    }
    rotate(l[i], r[i], k[i]);
  }
  take();
  cout << s << endl;
  return 0;
}
