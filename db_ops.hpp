#ifndef DB_OP_H
#define DB_OP_H

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <map>
#include <sqlite3.h>


struct db_ptr_deleter { void operator()(sqlite3* db) const { if(db){ sqlite3_close(db); } } };
struct statement_deleter { void operator()(sqlite3_stmt* statement) const { if(statement){ sqlite3_finalize(statement); } } };

using statement_ptr = std::unique_ptr<sqlite3_stmt, statement_deleter>;
using db_ptr = std::unique_ptr<sqlite3, db_ptr_deleter>;
using entry_map = std::map<int, std::string>;

db_ptr getDatabasePtr();
int getKeywordId(sqlite3* db, const std::string& keyword);
entry_map getKeywordResults(sqlite3* db, int id);

int db_DefineKeyword(sqlite3* db, const std::string& keyword);
int db_WriteNote(sqlite3* db, int parent_id, const std::string& txt);
int db_RewriteNote(sqlite3* db, int entry_id, const std::string& txt);
int db_DeleteNote(sqlite3* db, int entry_id);

#endif