#pragma once
#include <string>


// origen puede ser "CLIENTE", "SERVIDOR", etc.
void log(const std::string& mensaje, const std::string& origen = "GENERAL");
