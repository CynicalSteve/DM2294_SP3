#include "Map.h"
#include <fstream>

short ** parse(const char *fileName)
{
	std::ifstream file;
	std::string input;
	size_t index[2];

	file.open(fileName);
	if (!file)
	{
		std::cout << "Error opening " << fileName << ".\n";
		return nullptr;
	}
	getline(file, input);
	//for (; index[1] != input.npos;)
	file.close();
	return nullptr;
}