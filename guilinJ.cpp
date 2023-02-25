#include <bits/stdc++.h>

using namespace std;

int n;

void tarjan(int u, int fa, vector<vector<int>>& g, vector<int>& low, vector<int>& dfn, vector<int>& co, stack<int>& st, int& t, int& d){
	low[u] = dfn[u] = t++;
	st.push(u);
	for (auto v : g[u]){
		if (!dfn[v]){
			tarjan(v, u, g, low, dfn, co, st, t, d);
			low[u] = min(low[u], low[v]);
		}
		else if (!co[v])
			low[u] = min(low[u], dfn[v]);
	}
	if (low[u] == dfn[u]){
		while (st.top() != u){
			co[st.top()] = d;
			st.pop();
		}
		co[st.top()] = d;
		st.pop();
		d++;
	}
}

void dfs(int u, int fa, vector<vector<int>>& g, vector<vector<int>>& g1, vector<int>& ma, vector<int>& mi, vector<int>& ans, set<int>& snum, int& flag){
	for (auto v : g1[u]){
		ma[u] = min(ma[u], v - 1);
	}
	ma[u] = min(ma[u], ma[fa]);
	for (auto v : g[u]){
		//cout << v << endl;
		if (v <= n || ans[v - n]){
			if (v <= n)
				mi[u] = max(mi[u], v + 1);
			else
				mi[u] = max(mi[u], mi[v]);
			continue;
		}
		dfs(v, u, g, g1, ma, mi, ans, snum, flag);
		mi[u] = max(mi[u], mi[v]);
	}
	int l = mi[u], r = ma[u];
	auto it = snum.lower_bound(l);
	//cout << l << ' ' << r << ' ' << u << ' ' << *it << endl;
	if (r < l){
		//cout << l << ' ' << r << ' ' << *it << endl;
		flag = 1;
	}
	if(it != snum.end() && *it <= r && *it >= l) {
		ans[u - n] = *it; 
		snum.erase(it);
	} else {
		flag = 1;
	}
}




int main(){
	int t;
	cin>>t;
	while (t--){
		//cout << "sdg" << ' ' << t << endl;
		int m;
		cin>>n>>m;
		vector<int> a(n + 10);
		for (int i = 1; i <= n; i++)
			cin>>a[i];
		vector<vector<int>> g(2 * n + 10), g1(2 * n + 10);
		for (int i = n + 1; i <= 2 * n; i++){
			g[2 * n + 1].push_back(i);
		}
		int flag = 0;
		for (int i = 1; i <= m; i++){
			int u, v;
			cin>>u>>v;
			if (a[u] == 0 && a[v] == 0){
				g[v + n].push_back(u + n);
				//cout << u << ' ' << v << endl;
			}
			else if (a[u] == 0){
				g[a[v]].push_back(u + n);
				g1[u + n].push_back(a[v]);
			}
			else if (a[v] == 0){
				g[v + n].push_back(a[u]);
			}
			else{
				u = a[u];
				v = a[v];
				if (u > v)
					flag = 1;
			}
		}
		if (flag){
			cout << -1 << endl;
			continue;
		}
		for (int i = n; i > 1; i--)
			g[i].push_back(i - 1);
		int t1 = 1, d = 1;
		stack<int> st;
		vector<int> low(2 * n + 10), dfn(2 * n + 10), co(2 * n + 10);
		for (int i = 1; i <= 2 * n; i++)
			if (!dfn[i])
				tarjan(i, 0, g, low, dfn, co, st, t1, d);
		//cout << d << ' ' << 2 * n + 1 << endl;
		if (d != 2 * n + 1){
			cout << -1 << endl;
			continue;
		}
		vector<int> ans(n + 10);
		set<int> snum;
		map<int, int> ma;
		for (int i = 1; i <= n; i++)
			ma[a[i]]++;
		for (int i = 1; i <= n; i++)
			if (!ma[i])
				snum.insert(i);
		for (int i = 1; i <= n; i++)
			ans[i] = a[i];
		vector<int> ma1(2 * n + 10, n), mi(2 * n + 10, 1);
		for (int i = 1; i <= n; i++){
			for (auto v : g[i]){
				if (v <= n || ans[v - n])
					continue;
				dfs(v, 0, g, g1, ma1, mi, ans, snum, flag);
			}
		}
		for (auto v : g[2 * n + 1]){
			if (v <= n || ans[v - n])
				continue;
			dfs(v, 0, g, g1, ma1, mi, ans, snum, flag);
		}
		if (flag){
			cout << -1 << endl;
		}
		else{
			for (int i = 1; i <= n; i++)
				cout << ans[i] << " \n"[i == n];
		}
	}

	return 0;
}