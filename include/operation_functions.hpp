#ifndef OP_FUNCTIONS_HPP
#define OP_FUNCTIONS_HPP

#include <optional>
#include <functional>

#include "utils.hpp"
#include "process_flags.hpp"
#include "db_ops.hpp"

void registerNewEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_set);

void rewriteEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_set);
void deleteEntry(sqlite3* db, const std::vector<Token>& tokens, const flag_setup_map& flag_set);

void showHelpMenu();


#endif