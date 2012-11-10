#include <iostream>
#include <vector>
#include <iomanip>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ios;
using std::setfill;
using std::setw;
using std::hex;
using std::showbase;
using std::uppercase;

class HashTable{
	public:
		enum syntaxType{
			AssemblerDirectives,
			Instruction
		};

		struct record{
			syntaxType type;
			string mnemonic;
			unsigned char opcode;
			unsigned int format;
		};

		HashTable();
		~HashTable();
		void insert(syntaxType type,
				    string mnemonic,
					unsigned char opcode,
					unsigned int format);
		void traversal();
	private:
		int size_;
		vector<struct record> table_;
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
	this->insert(AssemblerDirectives, "START", NULL, NULL);
	this->insert(AssemblerDirectives, "END", NULL, NULL);
	this->insert(AssemblerDirectives, "BYTE", NULL, NULL);
	this->insert(AssemblerDirectives, "WORD", NULL, NULL);
	this->insert(AssemblerDirectives, "RESB", NULL, NULL);
	this->insert(AssemblerDirectives, "RESW", NULL, NULL);
}

HashTable::~HashTable()
{
}


void HashTable::insert(syntaxType type, string mnemonic, unsigned char opcode, unsigned int format)
{
	record temp;
	temp.type = type;
	temp.mnemonic = mnemonic;
	temp.opcode = opcode;
	temp.format = format;
	table_.push_back(temp);
}

void HashTable::traversal()
{
	vector<struct record>::iterator index = table_.begin();
	for(index; index != table_.end(); ++index)
	{
		cout << index->mnemonic << " -> ";
		//cout.setf(ios::hex, ios::basefield);
		//cout.setf(ios::showbase);
		//cout << showbase << setfill('0') << setw(2) << hex << (index->opcode + 0) << " -> ";
		cout << "0x";
		cout.width(2);
		cout.fill('0');
		cout << uppercase << hex << (index->opcode + 0) << " -> ";
		cout.unsetf(ios::hex | ios::showbase);
		cout << index->format << endl;
	}
}

int main()
{
	HashTable table;
	table.traversal();
	return 0;
}
