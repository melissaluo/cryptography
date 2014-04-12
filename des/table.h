#ifndef TABLE_H
#define TABLE_H

class Table
{
public:
	int IP_table[64];
	int IP_inverse_table[64];
	int E_table[48];
	int P_table[32];
	int S_table[8][4][16];
	int V_table[16];
	int PC1_table[2][28];
	int PC2_table[48];
	Table();
	~Table();
	void tableCheck(char* tablefile_name);
	void IPcheck(char * str);
	void IPinverse(char * str);
	void Echeck(char * str);
	void Pcheck(char * str);
	void Scheck(char * str, int table_index);
	void Vcheck(char * str);
	void PC1check(char * str);
	void PC2check(char * str);
};

#endif // TABLE_H
