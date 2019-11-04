#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::ios;
using std::string;
using std::fstream;
using std::vector;

struct huffCell
{
	int Glyph;
	int leftChild;
	int rightChild;
};

void main()
{
	int fileNameSize, numEntries, index, nextBit;
	string file, fileName = "", compressedData;
	char data;
	cout << "Enter name of file to decompress: ";	cin >> file;

	fstream fin(file, ios::in | ios::binary);

	if (!fin.is_open())
	{
		cout << "Could not open file... Program closing" << endl;
	}
	else
	{
		fin.read((char*)& fileNameSize, sizeof(fileNameSize));
		for (int i = 0; i < fileNameSize; i++)
		{
			fin.read((char*)& data, sizeof(data));
			fileName += data;
		}
		//fin.read((char*)& fileName, fileSize);
		fin.read((char*)& numEntries, sizeof(numEntries));

		fstream fout(fileName, ios::out | ios::binary);

		vector<huffCell> cells;

		for (int i = 0; i < numEntries; i++)
		{
			huffCell cell;
			fin.read((char*)& cell.Glyph, sizeof(cell.Glyph));
			fin.read((char*)& cell.leftChild, sizeof(cell.leftChild));
			fin.read((char*)& cell.rightChild, sizeof(cell.rightChild));
			cells.push_back(cell);
		}

		index = 0;
		while(fin.get(data))
		{
			for (int i = 7; i >= 0; i--)
			{
				// If current index is a leaf node, print it out
				if (cells[index].Glyph != -1 && cells[index].Glyph != 256)
				{
					fout.write((char*)& cells[index].Glyph, sizeof(data));
					index = 0;
				}
				else if (cells[index].Glyph == 256)
				{
					break;
				}
				
				nextBit = ((data >> i) & 1);
				if (nextBit == 0)
				{
					index = cells[index].leftChild;
				}
				else
				{
					index = cells[index].rightChild;
				}
			}
		}
		
	}

	fin.close();
}