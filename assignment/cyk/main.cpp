#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

class Word
{
public:
	
	string cont;
	
	Word( string _cont )
	{
		cont = _cont;
	};
	
	bool equal( Word *w )
	{
		if ( w->cont.compare( cont ) == 0 ) return true;		
		else return false;
	};
};

Word* NILWord = new Word("");

class Rule
{
public:
	Word* value;
	Word* key1;
	Word* key2;
	
	Rule( Word* _value, Word* _key1, Word* _key2 )
	{
		value = _value;
		key1 = _key1;
		key2 = _key2;
	};
	
	Rule( Word* _value, Word* _key1 )
	{
		value = _value;
		key1 = _key1;
		key2 = new Word( "" );
	};
	
	bool match( Word* _value, Word* _key1, Word* _key2 )
	{
		if ( value->equal( _value ) == false ) return false;
		
		if ( key1->equal( _key1 ) == false ) return false;
		
		if ( key2->equal( _key2 ) == false ) return false;
		
		return true;
	};
	
	bool match( Word* _value, Word* _key )
	{
		if ( value->equal( _value ) == false ) return false;
		
		if ( key1->equal( _key ) == false ) return false;
		
		if ( key2->equal( NILWord ) == false ) return false;
		
		return true;
	};
};

class CYKTable
{
public:
	vector<Word*>* *table;
	
	Word* sWord;
	
	int len;
	
	vector<Word*> start;
	
	vector<Rule*> rlist;
	
	vector<Word*> wlist;
	
	CYKTable() {
	};
	
	int initTbl();
	
	void input();
	
	int doCYK();
	
	int fill( int idx, Word* w );
	int reverseLookupAndFill( int idx, Word *key1, Word *key2 );
	int lookupWord( int idx, Word* w );
	
	int isValid();
	int printTable();
};

int CYKTable::printTable()
{
	int i, j, k;
	for ( i = 0; i < len; i++ ) {
		for ( j = 0; j < (len-i); j++ ) {
			cout<<"("<<j<<","<<i<<"):";
			for ( k = 0; k < table[j+i*len]->size(); k++ ) {
				cout<<" "<<(*(table[j+i*len]))[k]->cont.c_str();
			}
			cout<<endl;
		}
	}
	
	return 0;
};

void CYKTable::input()
{	
	string wbuff1;
	string wbuff2;
	string wbuff3;
	int nw, nr, ns;
	cout<<"請輸入有幾種字母：";
	cin>>nw;
	cout<<"請輸入幾條文法：";
	cin>>nr;
	cout<<"請輸入字串有幾個字元：";
	cin>>ns;
	
	int i;
	
	Word* cw;
	Rule* cr;
	
	for ( i = 0; i < nw; i++ ) {
		cout<<"請輸入字母：";
		cin>>wbuff1;
		cw = new Word( wbuff1 );
		wlist.push_back( cw );
	}
	
	for ( i = 0; i < nr; i++ ) {
		cout<<"請輸入文法（每個字母中間以空隔隔開）：";
		cin>>wbuff1>>wbuff2>>wbuff3;
		if ( strcmp( wbuff3.c_str(), "null" ) != 0 ) {
			cr = new Rule( new Word( wbuff1 ), new Word( wbuff2 ), new Word( wbuff3 ) );
		} else {
			cr = new Rule( new Word( wbuff1 ), new Word( wbuff2 ) );
		}
		rlist.push_back( cr );
	}
	
	len = ns;
	cout<<"請輸入字串：";
	string str;
	cin>>str;
	for ( i = 0; i < ns; i++ ) {
		string tmp;
		tmp+=str[i];
		cw = new Word( tmp );
		start.push_back( cw );
	}
};

int CYKTable::fill( int idx, Word* w )
{
	int i;
	int found = 0;
	for ( i = 0; i< table[idx]->size(); i++  ) {
		if ( (*(table[idx]))[i]->equal( w ) ) {
			found = 1;
			break;
		}
	}
	
	if ( found == 0 ) {
		table[idx]->push_back( w );
	}
	
	return found;
};

int CYKTable::reverseLookupAndFill( int idx, Word* key1, Word* key2 )
{
	int i, j;
	for ( i = 0; i < wlist.size(); i++ ) {
		for ( j = 0; j < rlist.size(); j++ ) {
			if ( rlist[j]->match( wlist[i], key1, key2 ) ) {
				fill( idx, wlist[i] );
			}
		}
	}
	
	return 0;
};

int CYKTable::lookupWord( int idx, Word* w )
{
	int i;
	for ( i = 0; i < table[idx]->size(); i++ ) {
		if ( (*(table[idx]))[i]->equal( w ) == 1 ) {
			return 1;
		}
	}
	
	return 0;
};

int CYKTable::isValid()
{
	return lookupWord( 0+(len-1)*len, sWord );
};

int CYKTable::initTbl()
{
	int i, j;
	sWord = new Word("S");
	table = (vector<Word*>**) malloc( sizeof( vector<Word*>* ) * len * len );
        memset(table,0,sizeof(vector<Word*>**)*len*len);
	for ( i = 0; i < len; i++ ) {
		for ( j = 0; j < (len-i); j++ ) {
			table[j+i*len] = new vector<Word*>();
		}
	}
	
	return 0;
};

int CYKTable::doCYK()
{
	int i;
	int j;
	int k;
	int a, b;
	
	for(i=0;i<len;i++) reverseLookupAndFill(i+0*len, start[i], NILWord);
	
	int pos1x, pos1y;
	int pos2x, pos2y;
	int idx1, idx2;
	for ( i = 1; i < len; i++ ) {
		for ( j = 0; j < (len-i); j++ ) {
			pos1x = j;
			pos1y = 0;
			pos2x = j+1;
			pos2y = i-1;
			for ( k = 0; k < i; k++ ) {
				idx1 = pos1x+pos1y*len;
				idx2 = pos2x+pos2y*len;
				for ( a = 0; a < table[idx1]->size(); a++ ) {
					for ( b = 0; b < table[idx2]->size(); b++ ) {
						reverseLookupAndFill( j+i*len, (*(table[idx1]))[a], (*(table[idx2]))[b] );
					}
				}
				pos1y++;
				pos2x++;
				pos2y--;
			}
		}
	}
	
	return 0;
};

int main()
{
	CYKTable *ct = new CYKTable();
	
	ct->input();
	
	ct->initTbl();
	ct->doCYK();
	
	ct->printTable();
	
	if(ct->isValid()==1) cout<<"正確"<<endl;
	else cout<<"錯誤"<<endl;
	
	return 0;
};
