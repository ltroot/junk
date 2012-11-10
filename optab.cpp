/* 
 * @author 陳賢安(9903089A)
 * @encode UTF-8
 *
 * 功能：
 * #從檔案讀取opcode table
 * #search
 * #insert
 * #delete
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>

using std::istringstream;
using std::ifstream;
using std::string;
using std::map;
using std::cout;
using std::endl;
using std::hex;
using std::showbase;
using std::uppercase;

enum syntaxType{
	AssemblerDirectives,
	Instruction
};

class HashTable{
	public:
		struct record{
			enum syntaxType type;
			string mnemonic;
			unsigned char opcode;
			unsigned int format;
		};

		HashTable();

		void search(string key);
		void insert(syntaxType type,
				    string mnemonic,
					unsigned char opcode,
					unsigned int format);
		void Delete(string key);
		void traversal();
		void readOptabFromFile(const char * fileName);
	private:
		map<string, struct record> table_;
};

HashTable::HashTable()
{
	this->insert(Instruction, "ADD", 0x18, 3);
	this->insert(Instruction, "ADDF", 0x58, 3);
	this->insert(Instruction, "ADDR", 0x90, 2);
	this->insert(Instruction, "AND", 0x40, 3);
	this->insert(Instruction, "CLEAR", 0xB4, 2);
	this->insert(Instruction, "COMP", 0x28, 3);
	this->insert(Instruction, "COMPF", 0x88, 3);
	this->insert(Instruction, "COMPR", 0xA0, 2);
	this->insert(Instruction, "DIV", 0x24, 1);
	this->insert(Instruction, "DIVF", 0x64, 1);
	this->insert(Instruction, "DIVR", 0x9C, 1);
	this->insert(Instruction, "FIX", 0xC4, 3);
	this->insert(Instruction, "FLOAT", 0xC0, 3);
	this->insert(Instruction, "HIO", 0xF4, 3);
	this->insert(Instruction, "J", 0x3C, 3);
	this->insert(Instruction, "JEQ", 0x30, 3);
	this->insert(Instruction, "JGT", 0x34, 3);
	this->insert(Instruction, "JLT", 0x38, 3);
	this->insert(Instruction, "JSUB", 0x48, 3);
	this->insert(Instruction, "LDA", 0x00, 3);
	this->insert(Instruction, "LDB", 0x68, 3);
	this->insert(Instruction, "LDCH", 0x50, 3);
	this->insert(Instruction, "LDF", 0x70, 3);
	this->insert(Instruction, "LDL", 0x08, 3);
	this->insert(Instruction, "LDS", 0x6C, 3);
	this->insert(Instruction, "LDT", 0x74, 3);
	this->insert(Instruction, "LDX", 0x04, 3);
	this->insert(Instruction, "LPS", 0xE0, 3);
	this->insert(Instruction, "MUL", 0x20, 3);
	this->insert(Instruction, "MULF", 0x60, 3);
	this->insert(Instruction, "MULR", 0x98, 2);
	this->insert(Instruction, "NORM", 0xC8, 1);
	this->insert(Instruction, "OR", 0x44, 3);
	this->insert(Instruction, "RD", 0xD8, 3);
	this->insert(Instruction, "RMO", 0xAC, 2);
	this->insert(Instruction, "RSUB", 0x4C, 3);
	this->insert(Instruction, "SHIFTL", 0xA4, 2);
	this->insert(Instruction, "SHIFTR", 0xA8, 2);
	this->insert(Instruction, "SIO", 0xF0, 1);
	this->insert(Instruction, "SSK", 0xEC, 3);
	this->insert(Instruction, "STA", 0x0C, 3);
	this->insert(Instruction, "STB", 0x78, 3);
	this->insert(Instruction, "STCH", 0x54, 3);
	this->insert(Instruction, "STF", 0x80, 3);
	this->insert(Instruction, "STI", 0xD4, 3);
	this->insert(Instruction, "STL", 0x14, 3);
	this->insert(Instruction, "STS", 0x7C, 3);
	this->insert(Instruction, "STSW", 0xE8, 3);
	this->insert(Instruction, "STT", 0x84, 3);
	this->insert(Instruction, "STX", 0x10, 3);
	this->insert(Instruction, "SUB", 0x1C, 3);
	this->insert(Instruction, "SUBF", 0x5C, 3);
	this->insert(Instruction, "SUBR", 0x94, 2);
	this->insert(Instruction, "SVC", 0xB0, 2);
	this->insert(Instruction, "TD", 0xE0, 3);
	this->insert(Instruction, "TIO", 0xF8, 1);
	this->insert(Instruction, "TIX", 0x2C, 3);
	this->insert(Instruction, "TIXR", 0xB8, 2);
	this->insert(Instruction, "WD", 0xDC, 3);
	/*this->insert(AssemblerDirectives, "START", 0x01, NULL);
	this->insert(AssemblerDirectives, "END", 0x02, NULL);
	this->insert(AssemblerDirectives, "BYTE", 0x03, NULL);
	this->insert(AssemblerDirectives, "WORD", 0x04, NULL);
	this->insert(AssemblerDirectives, "RESB", 0x05, NULL);
	this->insert(AssemblerDirectives, "RESW", 0x06, NULL);*/
}

void HashTable::search(string key)
{
	record temp;
	map<string, struct record>::iterator it = table_.find(key);
	
	if(it != table_.end())
	{
		temp = it->second;
		if(temp.type == Instruction)
		{
			cout << key << ", opcode=";
			cout.width(2);
			cout.fill('0');
			cout << uppercase << hex << (temp.opcode + 0)
				 << ", format=" << temp.format << endl;
		}
		else cout << key << ", Assembler Directive" << endl;
	}
	else cout << key << ", not found" <<endl;
}

void HashTable::insert(syntaxType type, string mnemonic, unsigned char opcode, unsigned int format)
{
	record temp;
	temp.type = type;
	temp.mnemonic = mnemonic;
	temp.opcode = opcode;
	temp.format = format;
	table_.insert(map<string, struct record>::value_type(mnemonic, temp));
}

void HashTable::traversal()
{
	map<string, struct record>::const_iterator index = table_.begin();
	record temp;
	for(index; index != table_.end(); ++index)
	{
		temp = index->second;
		cout << temp.mnemonic << " -> ";
		cout << "0x";
		cout.width(2);
		cout.fill('0');
		cout << uppercase << hex << (temp.opcode + 0) << " -> ";
		cout << temp.format << endl;
	}
}

void HashTable::Delete(string key)
{
	if(table_.erase(key)) cout << key << " removed" <<endl;
	else cout << "error : " << key << " not found" << endl;
}

void HashTable::readOptabFromFile(const char * fileName)
{
	ifstream inputFile(fileName);
	string buffer;
	string mnemonic;
	unsigned char opcode;
	unsigned int c;
	unsigned int format;

	while( std::getline(inputFile, buffer) )
	{
		istringstream iss(buffer);
		iss >> mnemonic;
		iss >> hex >> c;
		iss >> std::dec >> format;
		opcode = c;
		this->insert(Instruction, mnemonic, opcode, format);
	}

	inputFile.close();
}

int main()
{
	HashTable table;
	//table.readOptabFromFile("optab.txt");
	table.insert(AssemblerDirectives, "START", 0x01, NULL);
	table.insert(AssemblerDirectives, "END", 0x02, NULL);
	table.insert(AssemblerDirectives, "BYTE", 0x03, NULL);
	table.insert(AssemblerDirectives, "WORD", 0x04, NULL);
	table.insert(AssemblerDirectives, "RESB", 0x05, NULL);
	table.insert(AssemblerDirectives, "RESW", 0x06, NULL);

	table.search("LDA");
	table.search("STA");
	table.search("JSUB");
	table.search("COMP");
	table.search("LDX");
	table.search("START");
	table.search("END");
	table.search("RESW");
	table.search("RESR");
	
	table.Delete("LDA");
	table.search("LDA");

	return 0;
}
