/* 
 * @author 陳賢安(9903089A)
 * @encode UTF-8
 *
 * 全部是自己寫的
 *
 * 功能：
 * #指令碼&位址對應
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
#include <algorithm>

using std::istringstream;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::map;
using std::cout;
using std::endl;
using std::hex;
using std::showbase;
using std::uppercase;
using std::setw;

enum syntaxType{
	AssemblerDirectives = 1,
	Instruction,
	NOT
};

class HashTable{
public:
    struct record{
        enum syntaxType type;
        string mnemonic;
        unsigned char opcode;
        unsigned int format;
    };
    
    class keyToValue{
    public:
        keyToValue(const string& first):first_(first){}
        bool operator() (const map<string, struct record>::value_type& value)
        {
            return value.first == first_ ? true : false;
        }
    private:
        const string& first_;
    };
    
    HashTable();
    
    void insert(syntaxType type,
                string mnemonic,
                unsigned char opcode,
                unsigned int format);
    void Delete(string key);
    void readOptabFromFile(const char * fileName);
    const record& search(string first);
    string parseInstruction(string, unsigned int&);
private:
    map<string, struct record> table_;
    record nullRecord;
};

HashTable::HashTable()
{
	nullRecord.type = NOT;
	nullRecord.format = 0;
	nullRecord.mnemonic = "";
	nullRecord.opcode = 0;
    
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
	this->insert(AssemblerDirectives, "START", 0x01, NULL);
	this->insert(AssemblerDirectives, "END", 0x02, NULL);
	this->insert(AssemblerDirectives, "BYTE", 0x03, NULL);
	this->insert(AssemblerDirectives, "WORD", 0x04, NULL);
	this->insert(AssemblerDirectives, "RESB", 0x05, NULL);
	this->insert(AssemblerDirectives, "RESW", 0x06, NULL);
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

const HashTable::record& HashTable::search(string first)
{
	map<string, struct record>::const_iterator it = std::find_if(table_.begin(), table_.end(), keyToValue(first));
	return it != table_.end() ? it->second : nullRecord;
}

string HashTable::parseInstruction(string buffer, unsigned int& location)
{
	record temp;
	stringstream ss, tmpISS;
	unsigned int locationPlus;
	string::size_type pos;
	unsigned int num;
	string result, tmpSearch;
    
	tmpISS.str(buffer);
	for(tmpISS >> tmpSearch; !tmpISS.eof(); tmpISS >> tmpSearch)
	{
		temp = this->search(tmpSearch);
		if(temp.type != NOT) break;
	}
    
	if(temp.type == Instruction)
	{
		locationPlus = 0x3;
		ss << uppercase << hex << setw(4) << std::setfill('0') << location << ", ";
		result.append(ss.str());
		ss.str("");
		ss.clear();
		ss << temp.mnemonic << ", ";
		result.append(ss.str());
		ss.str("");
		ss.clear();
		ss << hex << setw(2) << std::setfill('0') << (temp.opcode + 0);
		result.append(ss.str());
		location += locationPlus;
		return result;
	}
	else if(temp.type == AssemblerDirectives)
	{
		if(temp.mnemonic == "BYTE")
		{
			string data;
			pos = buffer.find(temp.mnemonic);
			buffer.erase(0, pos+4);
			pos = 0;
			while(buffer[pos]==' ') pos++;
			buffer.erase(0, pos);
			tmpISS.str(buffer);
			tmpISS >> data;
			if(data[0]=='C') num = data.size()-3;
			else if(data[0]=='X') num = (data.size()-3)/2;
			else
			{
				tmpISS.str(data);
				tmpISS >> num;
			}
			locationPlus = num;
			ss << uppercase << hex << location << ", ";
			result.append(ss.str());
			ss.str("");
			ss.clear();
			ss << temp.mnemonic;
			result.append(ss.str());
			location += locationPlus;
			return result;
		}
		else if(temp.mnemonic == "WORD")
		{
			locationPlus = 0x3;
			ss << uppercase << hex << location << ", ";
			result.append(ss.str());
			ss.str("");
			ss.clear();
			ss << temp.mnemonic;
			result.append(ss.str());
			location += locationPlus;
			return result;
		}
		else if(temp.mnemonic == "RESB")
		{
			pos = buffer.find(temp.mnemonic);
			buffer.erase(0, pos+4);
			pos = 0;
			while(buffer[pos]==' ') pos++;
			buffer.erase(0, pos);
			tmpISS.str(buffer);
			tmpISS >> num;
			locationPlus = num * 0x1;
			ss << uppercase << hex << location << ", ";
			result.append(ss.str());
			ss.str("");
			ss.clear();
			ss << temp.mnemonic;
			result.append(ss.str());
			location += locationPlus;
			return result;
		}
		else if(temp.mnemonic == "RESW")
		{
			pos = buffer.find(temp.mnemonic);
			buffer.erase(0, pos+4);
			pos = 0;
			while(buffer[pos]==' ') pos++;
			buffer.erase(0, pos);
			tmpISS.str(buffer);
			tmpISS >> num;
			locationPlus = num * 0x3;
			ss << uppercase << hex << location << ", ";
			result.append(ss.str());
			ss.str("");
			ss.clear();
			ss << temp.mnemonic;
			result.append(ss.str());
			location += locationPlus;
			return result;
		}
		else
		{
			ss << uppercase << hex << location << ", END";
			result.append(ss.str());
			return result;
		}
	}
	else
	{
		result = " *";
		return result;
	}
}

int main()
{
	HashTable table;
    
	ifstream sourceFile("example2-1.asm");
	ofstream outputFile("hw2-3.tmp");
	int line_count = 1;
	string buffer;
	string::size_type pos;
	stringstream ss;
	unsigned int location;
    
	std::getline(sourceFile, buffer);
	pos = buffer.find("START");
	buffer.erase(0, pos+5);
	pos = 0;
	while(buffer[pos] == ' ') pos++;
	buffer.erase(0, pos);
	ss.str(buffer);
	ss >> std::hex >> location;
	outputFile << setw(2) << line_count << ": ";
	line_count++;
	outputFile.setf(std::ios_base::hex, std::ios::basefield);
	outputFile << location << ", START" << endl;
	outputFile.unsetf(std::ios::hex);
	while( std::getline(sourceFile, buffer) )
	{
		outputFile << setw(2) << line_count << ": ";
		outputFile << table.parseInstruction(buffer, location) << endl;
		line_count++;
	}
    
	sourceFile.close();
	outputFile.close();
	return 0;
}
