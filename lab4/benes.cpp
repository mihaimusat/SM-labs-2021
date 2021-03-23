#include <bits/stdc++.h>

using namespace std;

int inverse_shuffle(int n, int val) {
	
	int lsb = val & 1;
	val = (val >> 1) | (lsb << (n - 1));
	return val;
}

void print_mat(ofstream &out, int n, bool **mat) {
	
	int i, j;

	int k = int(log2(n));
	int levels = 2 * k - 1;

	out << "Switches' configuration\n";
	out << "=======================\n";

	for(i = 0; i < n / 2; i++) {
    		for(j = 0; j < levels; j++) {
      			if(mat[i][j] == 0) {
        			out << "00" << " ";
			}
      			else {
        			out << "11" << " ";
			}
    		}
    		out << "\n";
  	}
}

void simulate_benes(int k, int l, int c, unordered_map<int, int> &src, unordered_map<int, int> &dst, bool **mat) {

	int i, pos, check;

	int n = 1 << k;
	int levels = 2 * k - 1;

	if(k == 1) {
		mat[l][c] = !(dst[0] == 0 && dst[1] == 1);
	} 
	else {
		unordered_map<int, int> lower_src, lower_dst; 
		unordered_map<int, int> upper_src, upper_dst;
		bool *visited = new bool[n];

		i = 0;
		while(i < n) {
			if(!visited[i]) {
				pos = i;
				check = 0;
				bool ok = true;
				while(!(pos == i && check == 0) || ok) {
					ok = false;
					if(check == 0) {
						visited[pos] = true;
						mat[pos / 2 + l][c] = pos % 2;
						mat[dst[pos] / 2 + l][c + levels - 1] = dst[pos] % 2;
						int in = (pos % 2 == 0) ? pos : pos - 1;
						int out = (dst[pos] % 2 == 0) ? dst[pos] : dst[pos] - 1;
						int new_src = inverse_shuffle(k, in);
						int new_dst = inverse_shuffle(k, out);
						lower_src[new_dst] = new_src;
						lower_dst[new_src] = new_dst;
						pos = (dst[pos] % 2 == 0) ? dst[pos] + 1 : dst[pos] - 1;
					} 
					else {
						visited[src[pos]] = true;
						mat[pos / 2 + l][c + levels - 1] = 1 - pos % 2;
						mat[src[pos] / 2 + l][c] = 1 - src[pos] % 2;
					    	int in = (src[pos] % 2 == 0) ? src[pos] + 1 : src[pos];					
						int out = (pos % 2 == 0) ? pos + 1 : pos;
						int new_src = inverse_shuffle(k, in) - n / 2;
						int new_dst = inverse_shuffle(k, out) - n / 2;
						upper_src[new_dst] = new_src;
						upper_dst[new_src] = new_dst;
						pos = (src[pos] % 2 == 0) ? src[pos] + 1 : src[pos] - 1;
					}
					check = 1 - check;
				}
			}
			i++;
		}
		simulate_benes(k - 1, l, c + 1, lower_src, lower_dst, mat);
		simulate_benes(k - 1, l + n / 4, c + 1, upper_src, upper_dst, mat);
	}	
}

int main() 
{
	ifstream cin("data.in");
	ofstream cout("data.out");

	int n, i, x, y, nr;

	cin >> n;

	int k = int(log2(n));
	int levels = 2 * k - 1;

	bool **mat = new bool *[n / 2];
	for(i = 0; i < n / 2; i++) {
		mat[i] = new bool[levels];
	}

	unordered_map<int, int> src, dst;
	cin >> nr;
	while(nr--) {
		cin >> x >> y;
		dst[x] = y;
		src[y] = x;
	}

  	simulate_benes(k, 0, 0, src, dst, mat);
  	print_mat(cout, n, mat);

  	return 0;
}
