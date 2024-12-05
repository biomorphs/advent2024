#include "utils.h"
#include <fstream>

std::string LoadTextFile(const std::filesystem::path& textPath)
{
	std::string resultBuffer;
	std::ifstream fileStream(textPath.string().data(), std::ios::in);
	if (fileStream.is_open())
	{
		fileStream.seekg(0, fileStream.end);
		const size_t fileSize = fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);

		resultBuffer.resize(fileSize);
		fileStream.read(resultBuffer.data(), fileSize);

		size_t actualSize = strlen(resultBuffer.data());
		resultBuffer.resize(actualSize);
	}
	return resultBuffer;
}