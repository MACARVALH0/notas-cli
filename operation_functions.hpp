#ifndef OP_FUNCTIONS_HPP
#define OP_FUNCTIONS_HPP

#include "utils.hpp"
#include "db_ops.hpp"

void newEntry(sqlite3* db, const str_vector& tokens, int parent_id);
void rewriteEntry(sqlite3* db, const str_vector& tokens, const std::string& entry_txt);
void DeleteEntry(sqlite3* db, str_vector tokens);
void ShowHelpMenu();

#endif