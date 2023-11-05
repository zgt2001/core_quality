#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 1e5 + 10;
vector<vector<int>> g(N), g1(N);
vector<int> depth(N), siz(N), son(N), f(N), top(N), dfsn(N);
//vector<ll> c(N, 1e17);
vector<int> mark(N);
int cnt = 1;


struct rmq_st {
	vector<vector<int>> st;
	vector<int> lg;
	int n;
	rmq_st(int n, int m, vector<int>& a): st(n + 1, vector<int>(m + 1)), lg(n + 2), n(n) {
		for (int i = 1; i <= n; i++) {
			if (i != 1) {
				lg[i] = lg[i / 2] + 1;
			}
			st[i][0] = a[i - 1];
		}
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j + (1 << (i - 1)) <= n; j++) {
				st[j][i] = min(st[j][i - 1], st[j + (1 << (i - 1))][i - 1]);
			}
		}
	}

	int get(int l, int r) {
		int len = lg[r - l + 1];
		return min(st[l][len], st[r - (1 << len) + 1][len]);
	}
};


void dfs1(int u, int fa) {
	depth[u] = depth[fa] + 1;
	siz[u] = 1;
	son[u] = 0;
	dfsn[u] = cnt++;
	f[u] = fa;
	for (auto v : g[u]) {
		if (v == fa)
			continue;
		dfs1(v, u);
		siz[u] += siz[v];
		if (siz[v] > siz[son[u]]) {
			son[u] = v;
		}
	}
}

void dfs2(int u, int topx) {
	top[u] = topx;
	if (son[u]) 
		dfs2(son[u], topx);
	else
		return;
	for (auto v : g[u]) {
		if (v == f[u] || v == son[u]) {
			continue;
		}
		dfs2(v, v);
	}
}

int LCA(int x, int y) {
	while (top[x] != top[y]) {
		if (depth[top[x]] < depth[top[y]]) {
			swap(x, y);
		}
		x = f[top[x]];
	}
	if (depth[x] > depth[y]) {
		swap(x, y);
	}
	return x;
}

bool cmp(int a, int b) {
	return dfsn[a] < dfsn[b];
}


void insert(int x, stack<int>& st) {
	if (st.size() == 1) {
		st.push(x);
		return;
	}
	int lca = LCA(x, st.top());
	if (lca == st.top()) {
		st.push(x);
		return;
	}
	int d = st.top();
	st.pop();
	while (!st.empty() && dfsn[lca] <= dfsn[st.top()]) {
		g1[st.top()].push_back(d);
		d = st.top();
		st.pop();
	}
	if (d != lca) {
		g1[lca].push_back(d);
	}
	st.push(lca);
	st.push(x);
}

ll dfs(int u, int dep, vector<int>& a, rmq_st& rmq) {
	//cout << "****" << endl;
	if (depth[u] == dep) {
		return a[0];
	}
	ll ans = 0;
	for (auto v : g1[u]) {
		ll val = dfs(v, dep, a, rmq);
		val = min(val, 1LL * rmq.get(dep - depth[v] + 1, dep - depth[u] + 1));
		//cout << val << ' ' << dep - depth[v] + 1 << ' ' << dep - depth[u] + 1 << endl;
		ans += val;
	}
	ans = min(ans, 1LL * a[dep - depth[u]]);
	//cout << "***" << dep - depth[u] << ' ' << dep << ' ' << depth[u] << ' ' << u << endl;
	return ans;
}

void dfs3(int u) {
	//cout << u << endl;
	for (auto v : g1[u]) {
		dfs3(v);
	}
	if (g1[u].size() != 0)
		g1[u].clear();
}




int main() {
	ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
	int t;
	cin >> t;
	while (t--) {
		cnt = 1;
		int n;
		cin >> n;
		vector<int> a(n);
		for (int i = 0; i < n; i++) {
			cin >> a[i];
		}
		rmq_st rmq(n, 19, a);
		for (int i = 1; i < n; i++) {
			int u, v;
			cin >> u >> v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		dfs1(1, 1);
		dfs2(1, 1);
		int numd = 0;
		for (int i = 1; i <= n; i++) {
			numd = max(numd, depth[i]);
		}
		vector<vector<int>> dep(numd + 1);
		for (int i = 2; i <= n; i++) {
			int d = depth[i];
			dep[d].push_back(i);
		}
		//cout << "st" << endl;
		// for (int i = 2; i <= numd; i++) {
		// 	dep[i].push_back(1);
		// }
		ll ans = 0;
		//cout << numd << endl;
		for (int i = 1; i <= numd; i++) {
			sort(dep[i].begin(), dep[i].end(), cmp);
			stack<int> st;
			st.push(1);
			for (int j = 0; j < dep[i].size(); j++) {
				insert(dep[i][j], st);
				//mark[dep[i][j]] = 1;
			}
			//cout << "sc" << endl;
			while (!st.empty()) {
				int d = st.top();
				st.pop();
				if (!st.empty()) {
					g1[st.top()].push_back(d);
				}
			}
			//cout << "dsf" << endl;
			ans += dfs(1, i, a, rmq);
			//cout << "ans: " <<  ans << endl;
			//cout << "dfs3" << endl;
			dfs3(1);

		}
		cout << ans << endl;
		for (int i = 1; i <= n; i++) {
			if (g[i].size() != 0) {
				g[i].clear();
			}
			depth[i] = 0;
		}

	}



	return 0;
}

/*
3
6
10 15 40 1 1 1
1 2
2 3
2 6
3 5
1 4
5
10 5 1 100 1000
1 2
2 3
2 4
4 5
4
1000 200 10 8
1 2
2 3
3 4



*/