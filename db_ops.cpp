#include "db_ops.hpp"


db_ptr getDatabasePtr()
{
    sqlite3* db_raw = nullptr;

    // Trying to open database and bind it to its respective pointer
    const int db_open_result = sqlite3_open("data/data.db", &db_raw);
    if(db_open_result != SQLITE_OK)
    {
        std::string error_msg = "<# Não foi possível abrir o banco de dados corretamente: \n";
        error_msg.append(sqlite3_errmsg(db_raw));
        sqlite3_close(db_raw);
        throw std::runtime_error(error_msg); // Throw runtime_error
    }

    return db_ptr(db_raw);
}



int getKeywordId(sqlite3* db, const std::string& keyword)
{
    const std::string query = "SELECT id FROM keywords WHERE name = ?;";
    statement_ptr statement(nullptr);
    sqlite3_stmt* s_raw = nullptr;

    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &s_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK)
    { return -1; }

    statement.reset(s_raw);

    sqlite3_bind_text(statement.get(), 1, keyword.c_str(), -1, SQLITE_STATIC);

    const int step_result = sqlite3_step(statement.get());
    if(step_result == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement.get(), 0);
        return id;
    }
    
    return -1;
}



entry_map getKeywordResults(sqlite3* db, int keyword_id)
{
    const std::string query = "SELECT id, content FROM results WHERE parent_id = ?";
    statement_ptr statement(nullptr);
    sqlite3_stmt* statement_raw;

    if(keyword_id <= 0){ return entry_map(); }

    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK)
    { return entry_map{{ 0, sqlite3_errmsg(db) }}; }

    statement.reset(statement_raw);

    entry_map results;

    sqlite3_bind_int(statement.get(), 1, keyword_id);
    int step_result = sqlite3_step(statement.get());
    while(step_result == SQLITE_ROW)
    {
        int id = sqlite3_column_int(statement.get(), 0);
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 1));
        if(content){ results.insert({id, content}); }

        step_result = sqlite3_step(statement.get());
    }

    return results;
}



int db_DefineKeyword(sqlite3* db, const std::string& keyword)
{
    // Setting up the query.
    const std::string query = "INSERT INTO keywords (name) VALUES (?);";
    statement_ptr statement(nullptr);
    sqlite3_stmt* statement_raw(nullptr);

    // Preparing statement.
    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK){ return 0; }

    // Restarting smart pointer with raw pointer.
    statement.reset(statement_raw);

    // Replacing empty spaces with content.
    sqlite3_bind_text(statement.get(), 1, keyword.c_str(), -1, SQLITE_STATIC);

    // Run query on database.
    const int step_result = sqlite3_step(statement.get());
    if(step_result != SQLITE_DONE){ return 0; }

    return sqlite3_last_insert_rowid(db);
}



int db_WriteNote(sqlite3* db, int parent_id, const std::string& txt)
{
    // Setting up query and statement pointers.
    const std::string query = "INSERT INTO results (content, parent_id) VALUES (?, ?);";
    statement_ptr statement(nullptr);
    sqlite3_stmt* statement_raw(nullptr);

    // Preparing statement.
    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK){ return 0; }

    statement.reset(statement_raw);

    // Binding up values to the query string.
    sqlite3_bind_text(statement.get(), 1, txt.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement.get(), 2, parent_id);

    // Running query on database.
    const int step_result = sqlite3_step(statement.get());
    if(step_result != SQLITE_DONE){ return 0; }

    return sqlite3_last_insert_rowid(db);
}



int db_RewriteNote(sqlite3* db, int entry_id, const std::string& txt)
{
    const std::string query = "UPDATE results SET content = ? WHERE id = ?";
    statement_ptr statement(nullptr);
    sqlite3_stmt* statement_raw(nullptr);

    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK) { return 0; }

    statement.reset(statement_raw);

    sqlite3_bind_text(statement.get(), 1, txt.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement.get(), 2, entry_id);

    const int step_result = sqlite3_step(statement.get());
    if(step_result != SQLITE_DONE){ return 0; }

    // Retornando o número de linhas alteradas.
    return sqlite3_changes(db);
}



int db_DeleteNote(sqlite3* db, int entry_id)
{
    const std::string query = "DELETE FROM results WHERE id = ?";
    statement_ptr statement(nullptr);
    sqlite3_stmt* statement_raw(nullptr);

    const int statement_prepare_result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement_raw, nullptr);
    if(statement_prepare_result != SQLITE_OK) { return -1; }

    statement.reset(statement_raw);

    sqlite3_bind_int(statement.get(), 1, entry_id);

    const int step_result = sqlite3_step(statement.get());
    if(step_result != SQLITE_DONE){ return -1; }

    // Retornando o número de linhas deletadas.
    return sqlite3_changes(db);
}