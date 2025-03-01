#ifndef OP_FUNCTIONS_HPP
#define OP_FUNCTIONS_HPP

#include <optional>
#include <functional>
#include "utils.hpp"
#include "db_ops.hpp"


void registerNewEntry(sqlite3* db, const token_list& tokens, int parent_id);
void rewriteEntry(sqlite3* db, const str_vector& tokens, const std::string& entry_txt);
void DeleteEntry(sqlite3* db, str_vector tokens);
void ShowHelpMenu();


////////// HELPER FUNCTIONS

void newEntryLong(sqlite3* db, int parent_id);
void newEntryShort(sqlite3*db, int parent_id, const std::string& entry_text);

#endif