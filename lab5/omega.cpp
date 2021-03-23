#include <bits/stdc++.h>

using namespace std;

int main() 
{	
	ifstream cin("data.in");
	ofstream cout("data.out");

	int n, nr, in, out;
	int levels, height;
	pair<int, pair<int, int>> **block;
	vector<pair<int, int>> connections;

	cin >> n;
	
	levels = int(log2(n));
	height = n / 2;
	block = new pair<int, pair<int, int>> *[levels];
	for(int i = 0; i < levels; i++) {
		block[i] = new pair<int, pair<int, int>>[height];
	}

	cin >> nr;

	while(nr--) {
		cin >> in >> out;
		connections.emplace_back(make_pair(in, out));
	}
	
	for(auto &conn : connections) {
		cout << conn.first << " -> ";
		int msb = (conn.first & (1 << (levels - 1))) ? 1 : 0;
		conn.first = conn.first & ~(1 << (levels - 1));
		int in = (conn.first << 1) | msb;
		for(int level = levels - 1; level >= 0; level--) {
			int index = in / 2;
			int next = (conn.second >> level) & 1;
			int mod = (next == 0 ? 0 : 3);

			cout << "level " << level << ":" << "block " << index << " (C1="
			     << (mod >> 1) << " C0=" << (mod & 1)
			     << ") -> ";

			pair<int, pair<int, int>> aux = block[level][index];
			if(aux.first && aux.first != mod + 1) {
				cout << "Conflict between " << "("
				     << aux.second.first << " " << aux.second.second << ")"
				     << " and (" << conn.first << " " << conn.second  << ")"<< '\n';
			}
			block[level][index] = make_pair(mod + 1, conn);
			int index_aux = index * 2 + next; 
			int msb_aux = (index_aux & (1 << (levels - 1))) ? 1 : 0;
			index_aux = index_aux & ~(1 << (levels - 1));
			in = (index_aux << 1) | msb_aux;
		}
		cout << conn.second << '\n';
	}

	return 0;
}
