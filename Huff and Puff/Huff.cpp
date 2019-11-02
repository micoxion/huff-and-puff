#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::vector;
using std::sort;

const int MAX_GLYPH = 256;

struct huffCell{
	huffCell(char glyph = 0, int frequency = 0, int leftChild = 0, int rightChild = 0) {
		this->glyph = glyph;
		this->frequency = frequency;
		this->leftChild = leftChild;
		this->rightChild = rightChild;
	}
	char glyph;
	int frequency;
	int leftChild;
	int rightChild;
};

bool compareHuffCell(huffCell huffCell1, huffCell huffCell2) {
	return huffCell1.frequency < huffCell2.frequency;
}

int main() {
	string fileName;
	cout << "Name of file to compress: ";
	cin >> fileName;

	fstream fin(fileName, ios::in | ios::binary);

	if (fileName.find('.') != string::npos) {
		fileName = fileName.substr(0, fileName.find('.'));
	}

	fstream fout(fileName + ".huf", ios::out | ios::binary);
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

		/*nextCell.frequency = 1;
		nextCell.glyph = 256;
		nextCell.leftChild = -1;
		nextCell.rightChild = -1;
		huffmanTable.push_back(nextCell);*/

		sort(huffmanTable.begin(), huffmanTable.end(), compareHuffCell);
		int originalSize = huffmanTable.size();
		h = huffmanTable.size() - 1;

		while (h >= 1) {
			if (huffmanTable[1].frequency <= huffmanTable[2].frequency) {
				m = 1;
			}
			else if (h != 2) {
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

				if (huffmanTable[cell].frequency > huffmanTable[rightChild].frequency &&
					(huffmanTable[rightChild].frequency < huffmanTable[leftChild].frequency || huffmanTable[leftChild].frequency == 0) &&
					huffmanTable[rightChild].frequency != 0) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[rightChild];
					huffmanTable[rightChild] = temp;
				}

				if (huffmanTable[cell].frequency > huffmanTable[leftChild].frequency &&
					(huffmanTable[leftChild].frequency <= huffmanTable[rightChild].frequency || huffmanTable[rightChild].frequency == 0) &&
					huffmanTable[leftChild].frequency != 0) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[leftChild];
					huffmanTable[leftChild] = temp;
				}
			}
			huffmanTable.pop_back();

			huffmanTable[h] = huffmanTable[0];
			huffmanTable[0].frequency = 0;
			temp.frequency = huffmanTable[h].frequency + huffmanTable[m].frequency;
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

				if (huffmanTable[cell].frequency > huffmanTable[rightChild].frequency &&
					(huffmanTable[rightChild].frequency < huffmanTable[leftChild].frequency || huffmanTable[leftChild].frequency == 0) &&
					huffmanTable[rightChild].frequency != 0) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[rightChild];
					huffmanTable[rightChild] = temp;
				}

				if (huffmanTable[cell].frequency > huffmanTable[leftChild].frequency &&
					(huffmanTable[leftChild].frequency <= huffmanTable[rightChild].frequency || huffmanTable[rightChild].frequency == 0) &&
					huffmanTable[leftChild].frequency != 0) {
					temp = huffmanTable[cell];
					huffmanTable[cell] = huffmanTable[leftChild];
					huffmanTable[leftChild] = temp;
				}
			}
			huffmanTable.pop_back();
			h--;
		}
	}

	fin.close();
}