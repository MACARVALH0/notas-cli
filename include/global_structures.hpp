#pragma once

#include "types.hpp"

struct Keyword
{
    std::string name;
    int id;

    Keyword(std::string name, int id)
    : name(name), id(id) {}

    // Retorna `true` se a keyword possui um id válido registrado no banco de dados. Caso contrário, retorna `false`.
    bool exists(){ return id >= 0; }
};


struct Token
{
    std::string content;
    OpTokenType type;

    int col;

    Token(std::string content, OpTokenType type, int col)
    : content(std::move(content)), type(type), col(col) {}
};