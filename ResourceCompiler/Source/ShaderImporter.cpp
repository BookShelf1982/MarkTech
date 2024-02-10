#include "ShaderImporter.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <sstream>

const char* get_filename_ext(const char* filename)
{
	const char* dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

int LoadShader(const char* filepath, const char* output, const char* entrypoint)
{
	shaderc::Compiler shCompiler;
	shaderc::CompileOptions shCompileOptions;
	const char* shaderFileExtension = get_filename_ext(filepath);
	shaderc_shader_kind shaderKind;

	std::fstream InputFile;
	InputFile.open(filepath, std::ios::in);
	if (!InputFile.is_open())
	{
		std::cout << "Failed to find shader.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	if (strcmp(shaderFileExtension, "vert") == 0 || strcmp(shaderFileExtension, "vsh") == 0)
	{
		shaderKind = shaderc_vertex_shader;
	}
	else if (strcmp(shaderFileExtension, "frag") == 0 || strcmp(shaderFileExtension, "fsh") == 0)
	{
		shaderKind = shaderc_fragment_shader;
	}
	else
	{
		std::cout << "Failed to find shader type.\nPlease have shader source code file extension be a .vert and .vsh for vertex shaders\nand .frag and .fsh for fragment shaders\n";
	}

	std::stringstream fileStringStream;
	fileStringStream << InputFile.rdbuf();
	std::string sourceBuffer = fileStringStream.str();
	InputFile.close();

	// -- compile shader -- //
	shaderc::SpvCompilationResult compResult = shCompiler.CompileGlslToSpv(sourceBuffer, shaderKind, filepath, entrypoint, shCompileOptions);
	
	if (compResult.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cout << "Failed to compile shader.\n" << compResult.GetErrorMessage() << "\n" << "Press enter to exit...\n";
		std::cin.get();
		return -1;
	}
	
	std::vector<uint32_t> shaderBuffer(compResult.cbegin(), compResult.cend());

	std::cout << "Shader compile succeded!\n";

	std::string Path = output;

	// If the file is not an .mfx file return false
	if (Path.substr(Path.size() - 4, 4) != ".mfx")
	{
		std::cout << "Output file is not an mfx file.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::fstream OutputFile;
	OutputFile.open(output, std::ios::out | std::ios::binary);

	if (!OutputFile.is_open())
	{
		std::cout << "Failed to find output path.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t assetId = dist(rd);

	std::cout << "Asset Id: " << assetId << "\n";
	size_t bytecodeSize = shaderBuffer.size() * sizeof(uint32_t);
	std::cout << "Shader size: " << bytecodeSize << "\n";
	OutputFile.write((char*)&assetId, sizeof(uint64_t));
	OutputFile.write((char*)&bytecodeSize, sizeof(size_t));
	OutputFile.write((char*)shaderBuffer.data(), bytecodeSize);
	OutputFile.close();

	std::cout << "Shader exported!\n";

	return 0;
}
