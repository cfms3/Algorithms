typedef tree<int, null_type, less<int>, rb_tree_tag,
tree_order_statistics_node_update> ordered_set;

/*
Without collisions. If there are collisions, it might be possible to use less_equal as a comparator (the delete does not work as expected) 
*/
int solve(vector<pair<int, int>> all) {
  sort(all.begin(), all.end());
  reverse(all.begin(), all.end());
  int ans = 0;
  ordered_set S;
  for (int i = 0; i < n; i++) {
    ans += S.order_of_key(all[i].second);
    S.insert(all[i].second);
  }
  return ans;
}
  