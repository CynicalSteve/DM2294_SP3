#include "Map.h"
#include <fstream>

short ** parse(const char *fileName)
{
	std::ifstream file;
	std::string input;
	size_t index[2];
	short **result;

	file.open(fileName);
	if (!file)
	{
		std::cout << "Error opening " << fileName << ".\n";
		return nullptr;
	}

	if (file.good())
	{
		getline(file, input);
		index[0] = 1;
		for (short i = 0; i < input.size(); ++i)
			if (input[i] == ',')
				++index[0];
	}

	result = new short *[index[0]];
	for (short x = 0; x < index[0]; ++x)
		result[x] = new short[index[0]];

	index[1] = 0;
	for (short x = 0; x < input.size(); x += 2)
		result[index[1]++][0] = input[x] - 48;

	for (short y = 1; file.good() && y < index[0]; ++y)
	{
		getline(file, input);
		index[1] = 0;
		for (short x = 0; x < input.size(); x += 2)
			result[index[1]++][y] = input[x] - 48;
	}

	if (0) //failsafe demo map
	{
		for (short y = 0; y < 11; ++y)
			for (short x = 0; x < 11; ++x)
				result[x][y] = 0;

		for (short y = 0; y < 11; ++y)
		{
			for (short x = 0; x < 11; ++x)
			{
				if (y == 0 || y == 10)
					result[x][y] = 1;
				else if (y % 2)
				{
					result[0][y] = 1;
					result[10][y] = 1;
				}
				else if (!(x % 2))
					result[x][y] = 1;
			}
		}
	}

	file.close();
	return result;
}