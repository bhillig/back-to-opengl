#pragma once

#include <string>

namespace fileUtils
{

	bool ReadTextFromFile(const std::string& filepath, std::string& text);

} // namespace fileUtils