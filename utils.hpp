#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <memory>

#include "db_ops.hpp"


enum Operation
{
    NEW_op,
    REWRITE_op,
    DELETE_op,
    EXIT_op,
    HELP_op
};


using str_vector = std::vector<std::string>;



std::vector<std::string> split(const std::string& txt, char delimiter);
void brush(str_vector& tokens, const std::string& excludent);

void showEntries(entry_map& entries, const std::string& keyword);

std::string getCommand(std::vector<std::string>& tokens);

Operation getOperation(std::string token);


#endif