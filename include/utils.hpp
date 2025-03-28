#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <filesystem>

#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include <windows.h>

#include "global_structures.hpp"
#include "db_ops.hpp"




class ErrorMsg
{
    std::ostringstream stream;
    std::string errFlag = "<# ";

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

        std::string get() const { return errFlag + stream.str() + '\n'; }
};


template <typename T>
inline bool includes(const std::vector<T>& v, const T& element)
{ return std::find(v.begin(), v.end(), element) != v.end(); }


void DEBUG_showTokens(const std::vector<Token>& tokens);


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



std::string& trim(std::string& text);

bool isNumber(const std::string& value);

int parseInt(const std::string& value);

std::string toString_OpTokenType(OpTokenType type);

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


/**
 * @brief Processa operações específicas de cada comando.
 */
void processTokens(Keyword& keyword, std::vector<Token>& tokens, sqlite3* db);


void StartNotepad(std::string& command);

#endif