#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <bitset>

int main(int argc, char** argv)
{
	std::string message = "Hello world !";

	{
		std::ifstream src(argv[1], std::ios::binary);
		std::ofstream dst(argv[2], std::ios::binary);
		dst << src.rdbuf();
	}

	std::fstream file(argv[2], std::ios::binary | std::ios::in | std::ios::out);

	BITMAPFILEHEADER fileheader = {};
	BITMAPINFOHEADER infoheader = {};

	file.read(reinterpret_cast<char*>(&fileheader), sizeof(fileheader));
	file.read(reinterpret_cast<char*>(&infoheader), sizeof(infoheader));

	int width = infoheader.biWidth;
	int rowSize = ((width * 3 + 3) / 4) * 4;

	std::streampos pos = fileheader.bfOffBits;

	for (char binaire : message) {
		std::bitset<8> bits(binaire);
		std::cout << bits << ' ';

		for (int i = 7; i >= 0; --i) {       
			file.seekg(pos);
			char byte;
			file.read(&byte, 1);              

			byte = (byte & 0xFE) | bits[i];  

			file.seekp(pos);
			file.write(&byte, 1);
			pos += 1;
		}
	}

	file.close();
}
