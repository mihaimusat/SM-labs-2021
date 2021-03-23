#include <bits/stdc++.h>

using namespace std;

int main() 
{
	ifstream cin("data.in");

	const char *states[4] = {"M", "E", "S", "I"};
	const char *types[2] = {"Rd", "Wr"};
	
	int n, i, nr, id;
	string type;
	vector<pair<int, string>> operations;

	cin >> n;

	cin >> nr;
	while(nr--) {
		cin >> id >> type;
		operations.emplace_back(make_pair(id, type));
	}

	vector<pair<int, string>> processors;
	for(i = 0; i < n; i++) {
		processors.emplace_back(make_pair(i + 1, states[3]));
	}
	
	cout << "| t0  |  init  |  ";
	for(auto &p : processors) {
		cout << "P" << p.first << " - " << p.second;
		cout << "  |  ";
	}
	cout << "  -  " << "  |  " << "  -    |";
    	cout << "\n";

	int count = 0;
	for(auto &op : operations) {
		
		int proc_id = op.first;
		string op_type = op.second.substr(2, 2);
		string src = "-";
		string action = "-";
		
		count++;

		// daca am o operatie de tip Rd
		if(op_type == types[0]) {
			
			// verifica daca toate procesoarele se afla in starea I
			bool check_invalid = true; 
			for(auto &p : processors) {
				if(p.second != states[3]) {
					check_invalid = false;
				}
			}

			// in cazul in care toate procesoarele sunt in starea I,
			// il trec pe ultimul in starea E
			if(check_invalid) {
                		processors[proc_id - 1].second = states[1];
                		action = "BusRd";
                		src = "Mem";
			}
			
			// daca exista cel putin un procesor care nu e in starea I
			else {
				// retin id-ul procesorului care se afla in starea E
				int ex_id = 0;
				for(auto &p : processors) {
					if(p.second == states[1]) {
                        			ex_id = p.first;
                    			}
				}

				// trec procesorul aflat in starea E 
				// si ultimul procesor din lista in starea S
				if(ex_id != 0) {
                    			processors[ex_id - 1].second = states[2];
                    			processors[proc_id - 1].second = states[2]; 
                    			action = "Flush";
                    			src = "Cache" + to_string(ex_id);
				}
				
				// daca exista cel putin un procesor care nu e in starea E
				else {
					// retin id-ul procesorului care se afla in starea M
					int mod_id = 0;
                    			for(auto &p : processors) {
                        			if(p.second == states[0]) {
                            				mod_id = p.first;
                        			}
                    			}

					// trec procesorul aflat in starea M in starea S
					// daca are id-ul diferit de al procesorului curent si este nenul
					if(mod_id != 0) {
                     				if(mod_id == proc_id) {
                        			
						}
						else {
                            				processors[mod_id - 1].second = states[2];
                            				action = "BusRd";
                            				src = "Cache" + to_string(mod_id);
                        			}
                    			}
	
					// daca id-ul procesorului curent nu se afla in starea M
					if(mod_id != proc_id) {
                        			
						// retin id-ul procesorului care se afla in starea S
						// deoarece acesta va fi si indexul memoriei cache
						int cache_num = 0;
                        			int num = 0;
                        			for(auto &p : processors) {
                            				if(p.second == states[2]) {
                                				if(cache_num == 0) {
                                    					cache_num = p.first;
                                				}
                                				num++;
                            				}
                        			}
						
						// actualizez sursa datelor si actiunea pe magistrala,
						// daca am obtinut un index al memoriei cache nenul si inca
						// nu am ajuns la ultimul procesor din lista
                        			if(cache_num != 0 && (unsigned int)(num) != processors.size()) {
                            				src = "Cache" + to_string(cache_num);
                            				action = "Flush";
                        			}
			
						// trec ultimul procesor din lista in starea S
                        			processors[proc_id - 1].second = states[2];
                    			}
				}
			}
		}

		// daca am o operatie de tip Wr
		else if(op_type == types[1]) {
			
			// daca alt procesor in afara de cel curent
			// incearca sa scrie, atunci setez starea acestuia ca fiind I 
			for(auto &p : processors) {
                		if(p.first != proc_id) {
                    			p.second = states[3];
                		}
            		}
			
			// trec ultimul procesor din lista in starea M
            		processors[proc_id - 1].second = states[0]; 
            		src = "Flush";
            		action = "BusRdX";
		}

		cout << "| " << "t" << count << "  |  " << op.second << "  |  ";
		for(auto &p : processors) {
			cout << "P" << p.first << " - " << p.second;
			cout << "  |  ";
		}
		
		if(action != "-" && src != "-") {
			string delim = action.size() == 6 ? "" : " ";
            		string aux_delim = src.size() == 3 ? "   " : (src.size() == 6 ? "" : " ");
            		cout << action << delim << " |  " << src << aux_delim << " |";
		}
		else {
			cout << "  -  " << "  |  " << "  -    |";
		}

		cout << "\n";
	}
	
	return 0;
}
