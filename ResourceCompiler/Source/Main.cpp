#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}

	std::ofstream file("C:/MarkTech/bin/Release-windows-x86_64/ResourceCompiler/out/data.dat");
	file << "ufdnfd";
	file.close();

	return 0;
}