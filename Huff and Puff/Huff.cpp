#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::vector;
using std::sort;
using std::bitset;

const int MAX_GLYPH = 256;

struct bitCode {
	bitCode(int glyph, string code) {
		this->glyph = glyph;
		this->code = code;
	}
	int glyph;
	string code;
};

struct huffCell{
	huffCell(int glyph = 0, int frequency = 0, int leftChild = 0, int rightChild = 0) {
		this->glyph = glyph;
		this->frequency = frequency;
		this->leftChild = leftChild;
		this->rightChild = rightChild;
	}
	int glyph;
	int frequency;
	int leftChild;
	int rightChild;
};

void createBitCodes(string& currentCode, const vector<huffCell>& table, vector<bitCode>& bitCodes, int index) {
	if (table[index].glyph != -1) {
		bitCode code(table[index].glyph, currentCode);
		bitCodes.push_back(code);
		currentCode.pop_back();
		return;
	}

	createBitCodes(currentCode += "0", table, bitCodes, table[index].leftChild);
	createBitCodes(currentCode += "1", table, bitCodes, table[index].rightChild);
	if (currentCode.length() > 0) 
		currentCode.pop_back();

}

bool compareHuffCell(huffCell huffCell1, huffCell huffCell2) {
	return huffCell1.frequency < huffCell2.frequency;
}

int main() {
	string fileName, fileNameSub;
	cout << "Name of file to compress: ";
	cin >> fileName;

	fstream fin(fileName, ios::in | ios::binary);

	if (fileName.find('.') != string::npos) {
		fileNameSub = fileName.substr(0, fileName.find('.'));
	}
	else {
		fileNameSub = fileName;
	}

	fstream fout(fileNameSub + ".huf", ios::out | ios::binary);

	if (!fin.is_open()) {
		cout << "Ah it didn't open darn it";
	}
	else {
		vector<huffCell> huffmanTable;
		huffCell nextCell;

		int glyphFrequencies[MAX_GLYPH]{ 0 };
		int length = 0;

		int m, f, h;

		fin.seekg(0, ios::end);
		length = fin.tellg();
		fin.seekg(0, ios::beg);

		char nextGlyph;

		for (int i = 0; i < length; i++) {
			fin.read((char*)& nextGlyph, sizeof(nextGlyph));

			glyphFrequencies[nextGlyph]++;
		}

		for (int i = 0; i < MAX_GLYPH; i++) {
			if (glyphFrequencies[i] > 0) {
				nextCell.frequency = glyphFrequencies[i];
				nextCell.glyph = i;
				nextCell.leftChild = -1;
				nextCell.rightChild = -1;
				huffmanTable.push_back(nextCell);
			}
		}

		nextCell.frequency = 1;
		nextCell.glyph = 256;
		nextCell.leftChild = -1;
		nextCell.rightChild = -1;
		huffmanTable.push_back(nextCell);

		sort(huffmanTable.begin(), huffmanTable.end(), compareHuffCell);
		int originalSize = huffmanTable.size();
		h = huffmanTable.size() - 1;

		while (h >= 1) {
			if (huffmanTable[1].frequency <= huffmanTable[2].frequency) {
				m = 1;
			}
			else if (h > 2) {
				m = 2;
			}
			else {
				m = 1;
			}

			huffmanTable.push_back(huffmanTable[m]);
			huffmanTable[m].frequency = 0;

			if (m < h) {
				huffmanTable[m] = huffmanTable[h];
				huffmanTable[h].frequency = 0;
			}

			huffCell temp;
			int leftChild, rightChild;
			huffmanTable.push_back(temp);
			for (int cell = 0; cell < h / 2 + 1; cell++) {
				leftChild = cell * 2 + 1;
				rightChild = cell * 2 + 2;

				if (huffmanTable[leftChild].frequency >= huffmanTable[cell].frequency &&
					huffmanTable[rightChild].frequency >= huffmanTable[cell].frequency) {
					continue;
				}

				//check right child
				if (huffmanTable[cell].frequency > huffmanTable[rightChild].frequency &&
					(huffmanTable[rightChild].frequency < huffmanTable[leftChild].frequency || huffmanTable[leftChild].frequency == 0) &&
					huffmanTable[rightChild].frequency != 0 && rightChild < h) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[rightChild];
					huffmanTable[rightChild] = temp;
				}

				//check left child
				if (huffmanTable[cell].frequency > huffmanTable[leftChild].frequency &&
					(huffmanTable[leftChild].frequency <= huffmanTable[rightChild].frequency || huffmanTable[rightChild].frequency == 0) &&
					huffmanTable[leftChild].frequency != 0 && leftChild < h) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[leftChild];
					huffmanTable[leftChild] = temp;
				}
			}
			huffmanTable.pop_back();

			huffmanTable[h] = huffmanTable[0];
			huffmanTable[0].frequency = 0;

			//creating frequency node
			temp.frequency = huffmanTable[h].frequency + huffmanTable[huffmanTable.size() - 1].frequency;
			temp.glyph = -1;
			temp.leftChild = h;
			temp.rightChild = huffmanTable.size() - 1;

			huffmanTable[0] = temp;

			temp.frequency = 0;
			huffmanTable.push_back(temp);
			for (int cell = 0; cell < h / 2 + 1; cell++) {
				leftChild = cell * 2 + 1;
				rightChild = cell * 2 + 2;

				if (huffmanTable[leftChild].frequency >= huffmanTable[cell].frequency &&
					huffmanTable[rightChild].frequency >= huffmanTable[cell].frequency) {
					continue;
				}

				//check right child
				if (huffmanTable[cell].frequency > huffmanTable[rightChild].frequency &&
					(huffmanTable[rightChild].frequency < huffmanTable[leftChild].frequency || huffmanTable[leftChild].frequency == 0) &&
					huffmanTable[rightChild].frequency != 0 && rightChild < h) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[rightChild];
					huffmanTable[rightChild] = temp;
				}

				//check left child
				if (huffmanTable[cell].frequency > huffmanTable[leftChild].frequency &&
					(huffmanTable[leftChild].frequency <= huffmanTable[rightChild].frequency || huffmanTable[rightChild].frequency == 0) &&
					huffmanTable[leftChild].frequency != 0 && leftChild < h) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[leftChild];
					huffmanTable[leftChild] = temp;
				}
			}
			huffmanTable.pop_back();
			h--;
		}
		int tableSize = huffmanTable.size();
		const char* fileNameCStr = fileName.c_str();
		fout.write((char*)&length, sizeof(length));
		fout.write((char*)fileNameCStr, sizeof(fileNameCStr));
		fout.write((char*)&tableSize, sizeof(tableSize));

		vector<bitCode> bitCodes;

		for (huffCell const &cell : huffmanTable) {

			fout.write((char*)&cell.glyph, sizeof(cell.glyph));
			fout.write((char*)&cell.leftChild, sizeof(cell.leftChild));
			fout.write((char*)&cell.rightChild, sizeof(cell.rightChild));
		}
		string code = "";

		createBitCodes(code, huffmanTable, bitCodes, 0);

		fin.seekg(0, ios::beg);
		char next;
		code = "";
		string compressed = "";

		while (fin.tellg() < length) {
			fin.read((char*)& next, sizeof(next));
			for (int i = 0; i < bitCodes.size(); i++) {
				if (bitCodes[i].glyph == next) {
					compressed += bitCodes[i].code;
					break;
				}
			}
		}

		for (int i = 0; i < bitCodes.size(); i++) {
			if (bitCodes[i].glyph == MAX_GLYPH) {
				compressed += bitCodes[i].code;
				break;
			}
		}

		while (compressed.length() % 8 != 0) {
			compressed += "0";
		}

		string bitsToEncode = "";

		for (int i = 0; i <= compressed.length() - 7; i += 8) {
			bitsToEncode = compressed.substr(i, 8);
			bitset<8> encoded(bitsToEncode);
			unsigned long longConvert = encoded.to_ulong();
			unsigned char next = static_cast<unsigned char>(longConvert);
			fout.write((char*)& next, sizeof(next));
			fout.flush();
		}
	}

	fin.close();
}