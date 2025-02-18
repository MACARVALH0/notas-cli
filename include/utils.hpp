#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <unordered_set>

#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include <windows.h>

#include "db_ops.hpp"


enum Operation
{
    NEW_op,
    REWRITE_op,
    DELETE_op,
    EXIT_op,
    HELP_op,
    INVALID_op
};

enum class OpTokenType
{
    INITIAL,
    UNKNOWN,
    IDENTIFIER,
    FLAG,
    STRING
};

struct Token
{
    std::string content;
    OpTokenType type;

    int col;

    Token(std::string content, OpTokenType type, int col)
    : content(std::move(content)), type(type), col(col) {}
};

class ErrorMsg
{
    std::ostringstream stream;

    public:

        template <typename T>
        ErrorMsg& operator<<(const T& value)
        {
            stream << value;
            return *this;
        }

        ErrorMsg& operator<<(std::ostringstream& (*manip)(std::ostream&))
        {
            stream << manip;
            return *this;
        }

        std::string get() const { return stream.str(); }
};



template <typename T>
bool includes(std::vector<T>& v, T element)
{ return std::find(v.begin(), v.end(), element) != v.end(); }


template <typename T>
std::vector<T> splice(std::vector<T> v, size_t start_index, size_t count)
{
    if(start_index >= v.size()){ return {}; }

    size_t end = std::min(start_index + count, v.size());
    auto start_iterator = v.begin()+start_index;
    auto end_iterator = v.begin()+end;

    std::vector<T> removed_elements(start_iterator, end_iterator);

    v.erase(start_iterator, end_iterator);

    return removed_elements;
};


using str_vector = std::vector<std::string>;
using token_list = std::vector<Token>;
using flag_map = std::map<std::string, std::variant<std::string, bool>>;

std::string& trim(std::string& text);

std::string enumToString(OpTokenType type);

void showEntries(entry_map& entries, const std::string& keyword);

std::string getCommand(std::vector<std::string>& tokens);

Operation getOperation(std::string token);


/**
 * @brief Tokeniza uma linha de comando em tokens.
 * 
 * @param line A linha de comando a ser tokenizada.
 * @return Um vetor de tokens.
 * @throws std::runtime_error Se um caractere inválido for encontrado ou se o buffer não estiver vazio ao final.
 */
std::vector<Token> tokenize(std::string& line);

flag_map getFlags(token_list& tokens);

PROCESS_INFORMATION StartNotepad(std::string& command);

#endif