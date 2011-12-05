#include "model.h"

//This can't be in a header, but it's pretty lame to have an entire object file
//for a single variable
std::unordered_map<std::string, model> models;
