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


void DEBUG_showTokens(const std::vector<Token>& tokens);



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
int processTokens(Keyword& keyword, std::vector<Token>& tokens, sqlite3* db);


void StartNotepad(std::string& command);

#endif