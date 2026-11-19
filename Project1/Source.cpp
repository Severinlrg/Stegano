#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>

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
	int height = std::abs(infoheader.biHeight);

	int rowSize = ((width * 3 + 3) / 4) * 4;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::streampos pos = fileheader.bfOffBits + (std::streamoff)y * rowSize + x * 3;
			file.seekp(pos, std::ios::beg);

			if (x < message.size())
				file.write(&message[x], 1);
			else
			{
				// reecrire le pixel original
			}
		}
	}

	file.close();
}
