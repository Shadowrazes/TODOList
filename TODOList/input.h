#pragma once

#include "configTask.h"
#include <vector>

std::vector<task> readTasksFromFile();
void writeTasksInFile(std::vector<task> tasks);