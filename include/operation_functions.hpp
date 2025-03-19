#ifndef OP_FUNCTIONS_HPP
#define OP_FUNCTIONS_HPP

#include <optional>
#include <functional>

#include "utils.hpp"
#include "process_flags.hpp"
#include "db_ops.hpp"

void registerNewEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_setup);
    void newEntryLong(sqlite3* db, int parent_id);
    void newEntryShort(sqlite3*db, int parent_id, const std::string& entry_text);


void rewriteEntry(sqlite3* db, const str_vector& tokens, const std::string& entry_txt);
void DeleteEntry(sqlite3* db, str_vector tokens);

void ShowHelpMenu();


#endif