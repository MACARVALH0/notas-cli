#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <unordered_set>
#include <variant>
#include <optional>
#include <memory>

typedef unsigned int u_int;

// GLOBAL ENUMERATORS

enum Operation
{
    NEW_op,
    REWRITE_op,
    DELETE_op,
    EXIT_op,
    HELP_op,
    UNKNOWN_op
};

enum class OpTokenType
{
    INITIAL,
    UNKNOWN,
    IDENTIFIER,
    FLAG,
    STRING
};


// GLOBAL ALIASES

using str_vector = std::vector<std::string>;
// using token_list = std::vector<Token>;