#include "operation_functions.hpp"

// TODO adicionar ao arquivo de utilitários
str_vector::iterator getIterator(str_vector v, const std::string& flag) { return std::find(v.begin(), v.end(), flag); }

int parseInt(const std::string& value)
{
    try                             { return std::stoi(value); }
    catch(std::invalid_argument& e) { return -1; }
    catch(std::out_of_range& e)     { return -1; }
}

void deleteTempFile(){}

void newEntryLong()
{
    std::string filename = "temp.txt";
    std::string default_insert = "\n< Adicionando entrada ao banco de dados.\n";

    std::string entry_text = "";

    // Lê o conteúdo do arquivo temporário `filename`
    auto readFile = [filename]() -> std::string
    {
        std::ifstream file_r(filename);
        std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
        return content;
    };

    // Abre o arquivo temporário, adiciona o texto de placeholder e fecha o mesmo.
    std::ofstream file_w(filename);
    file_w << "Edite aqui o conteúdo da nota.\n";
    file_w.close();

    std::string command = "notepad.exe " + filename;
    PROCESS_INFORMATION process_info = StartNotepad(command);
    
    // Aguarda a escrita no arquivo temporário.
    // std::string command = "notepad.exe " + filename;
    // system(command.c_str());
    // system("pause");

    // Lê o conteúdo do arquivo temporário.
    entry_text = readFile();

    if(!entry_text.empty() || entry_text != default_insert)
    {
        std::cout << default_insert;
        // Adicionando ao banco de dados.
        if(!db_WriteNote(db, parent_id, entry_text))
        {
            std::cerr << "<# Não foi possível adicionar a nota ao banco de dados.\n";
            throw std::runtime_error("<# A operação de adicionar a nota ao banco de dados n�o foi conclu�da corretamente.\n");
        }
    }

    else{ std::cout << "\n< A nota vazia não será salva.\n"; }

    // Limpando o conteúdo do arquivo temporário.
    file_w << "";
}


void newEntry
(
    sqlite3* db,
    token_list& tokens,
    int parent_id,
    std::optional<def_keyword_function> = std::nullopt
)
{
    flag_map flags = getFlags(tokens);

    const std::string& short_flag_value = std::get<std::string>(flags["-s"]);
    const bool& is_short_insert = !short_flag_value.empty();
    const bool& is_long_insert = std::get<bool>(flags["-l"]);

    if(is_long_insert)
    {
        // Long insert logic.
    }

    else
    {
        // Short insert logic.
    }
    

}





// [ ]
// void newEntry(sqlite3* db, const str_vector& tokens, int parent_id)
// {
//     /*
//         PADRÃO: `-l`, porém:
//             - Caso a flag `-s` seja adicionada, seguida de um texto, é uma operação de escrita curta.
//     */
//     str_vector flags = {"-l", "-s"};
    
//     std::string entry_text = "";
    
//     if(!tokens.empty() && getIterator(tokens, flags[0]) != tokens.end())
//     { // Long entry

//         std::string filename = "temp.txt";
//         std::string default_insert = "\n< Adicionando entrada ao banco de dados.\n";

//         // Lê o conteúdo do arquivo temporário `filename`
//         auto readFile = [filename]() -> std::string
//         {
//             std::ifstream file_r(filename);
//             std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
//             return content;
//         };

//         std::ofstream file_w(filename);
//         file_w << "Edite aqui o conteúdo da nota.\n";
//         file_w.close();

//         std::string command = "notepad.exe " + filename;
//         system(command.c_str());
//         system("pause");

//         // Lendo o conte�do do arquivo tempor�rio.
//         entry_text = readFile();

//         if(!entry_text.empty() || entry_text != default_insert)
//         {
//             std::cout << default_insert;
//             // Adicionando ao banco de dados.
//             if(!db_WriteNote(db, parent_id, entry_text))
//             {
//                 std::cerr << "<# Não foi possível adicionar a nota ao banco de dados.\n";
//                 throw std::runtime_error("<# A operação de adicionar a nota ao banco de dados n�o foi conclu�da corretamente.\n");
//             }
//         }

//         else{ std::cout << "\n< A nota vazia não será salva.\n"; }

//         // Limpando o conteúdo do arquivo temporário.
//         file_w << "";
//     }


//     else
//     {
//         std::cout << "\nDigite o conteúdo da entrada abaixo.\n> ";
//         std::getline(std::cin, entry_text);

//         if(!entry_text.empty())
//         {
//             std::cout << "\n< Adicionando entrada ao banco de dados.\n";
//             // Adicionando ao banco de dados.
//             if(!db_WriteNote(db, parent_id, entry_text))
//             {
//                 std::cerr << "<# N�o foi poss�vel adicionar a nota ao banco de dados.\n";
//                 throw std::runtime_error("<# A opera��o de adicionar a nota ao banco de dados n�o foi conclu�da corretamente.\n");
//             }
//         }

//         else{ std::cout << "\n< A nota vazia n�o ser� salva.\n"; }

//         std::clog << "< Opera��o conclu�da.\n";
//     }
// }



// [x]
// TODO - implementar texto da entrada na edi��o de entradas
// void rewriteEntry(sqlite3* db, const str_vector& tokens)
void rewriteEntry(sqlite3* db, const str_vector& tokens, const std::string& entry_txt)
{
    int entry_id = 0;
    str_vector flags = {"-l"};
    std::string entry_text_input = "";

    auto isNumber = [](const std::string& str)
    {
        for(const char& c : str){if (!isdigit(c)) return false; }
        return true;
    };

    // Getting the id of the entries to rewrite. 
    for(const std::string& str : tokens)
    {
        if(isNumber(str))
        {
            entry_id = parseInt(str);
            break;
        }
    }

    if(!entry_id)
    {
        std::cerr << "<# � necess�rio inserir um id de entrada v�lido.\n";
        throw std::runtime_error("<# N�o foi poss�vel encontrar o id da entrada.\n");
    }


    if(!tokens.empty() && getIterator(tokens, flags[0]) != tokens.end())
    { // In case of a long entry
        std::string filename = "temp.txt";

        // L� o conte�do do arquivo tempor�rio `filename`
        auto readFile = [filename]() -> std::string
        {
            std::ifstream file_r(filename);
            std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
            return content;
        };

        std::ofstream file_w(filename);
        file_w << entry_txt << "\n";
        file_w.close();

        std::string command = "notepad.exe " + filename;
        system(command.c_str());

        // Lendo o conte�do do arquivo tempor�rio.
        entry_text_input = readFile();

        if(!entry_text_input.empty())
        {
            // Adicionando ao banco de dados.
            if(!db_RewriteNote(db, entry_id, entry_text_input))
            {
                std::cerr << "<# N�o foi poss�vel sobrescrever a nota no banco de dados corretamente.\n";
                throw std::runtime_error("O id inserido n�o � v�lido ou n�o existe.");
            }
        }

        else{ std::cout << "\n< A nota vazia n�o ser� salva.\n"; }

        // Limpando o conte�do do arquivo tempor�rio.
        file_w << "";
    }

    else // Rewriting entry in short mode
    {
        std::cout << "Escreva o o texto para sobrescrever o conte�do da entrada original:\n> ";
        std::cout << "Texto original:\n";
        std::cout << entry_txt << "\n> ";

        std::getline(std::cin, entry_text_input);

        if(!entry_text_input.empty())
        {
            // Adicionando ao banco de dados.
            if(!db_RewriteNote(db, entry_id, entry_text_input))
            {
                std::cerr << "<# N�o foi poss�vel sobrescrever a nota no banco de dados corretamente.\n";
                throw std::runtime_error("O id inserido n�o � v�lido ou n�o existe.\n");
            }
        }

        else{ std::cout << "\n< A nota vazia n�o ser� salva.\n"; }
    }
}


void DeleteEntry(sqlite3* db, str_vector tokens)
{
    std::vector<int> id_list = {};

    auto isNumber = [](const std::string& str)
    {
        for(const char& c : str){if (!isdigit(c)) return false; }
        return true;
    };

    // First part: Captura os n�meros de id de cada entrada a ser deletada.
    for(const std::string& token : tokens)
    {
        if(isNumber(token))
        {
            int id = parseInt(token);
            if(!id)
            {
                std::string error_msg = "<# O valor " + token + " n�o � um id v�lido.\n";
                throw std::runtime_error(error_msg);
            }
            else { id_list.push_back(id); }
        }
    }

    // Second part: Chama as fun��es respons�veis por deletar para cada id.

    for(int id : id_list)
    {
        if(!db_DeleteNote(db, id))
        {
            std::string id_str = std::to_string(id);
            std::string delete_error_msg = "<# N�o foi poss�vel deletar o item de id " + id_str + " corretamente.\n";
            throw std::runtime_error(delete_error_msg);
        }
    }
}


void ShowHelpMenu()
{
    std::cout << "\nLista de comandos dispon�veis:\n\n";
    std::cout << 
    R"(
        NEW     | Entra no modo de escrita de uma nova nota sob a �ltima palavra-chave.
        REWRITE | Entra no modo de reescria de uma nota a partir do seu ID.
        DELETE  | Deleta uma ou mais notas.
        HELP    | Exibe interface de apoio.
        EXIT    | Sai do escopo da palavra-chave.

        Uso:

        $ NEW <-l>;
        -   `NEW` -> Nova entrada curta, escrita direto no terminal.
        -   `NEW -l` -> Nova entrada longa, abre o bloco de notas.

        $ REWRITE [id] <-l>;
        -   `REWRITE 13 -l` -> Nova entrada longa, abre o bloco de notas.

        $ DELETE [...id];
        -   `DELETE 12, 15, 26, ...` -> Deleta as notas de ids 12, 15 e 26.

        $ HELP;
        -   `HELP` -> Exibe a interface de apoio. N�o recebe argumentos.

        $ EXIT;
        -   `EXIT` -> Sai do escopo da palavra-chave.
    )";
    
    std::cout << "\n";

}


// std::string command = "notepad.exe " + filename;

// STARTUPINFOA startup_info = { sizeof(STARTUPINFO) };
// PROCESS_INFORMATION process_info; // What we get as an `out` parameter

// ZeroMemory(&startup_info, sizeof(startup_info));
// startup_info.cb = sizeof startup_info; //  `sizeof` as an operator is possible when its operand is a variable and not a type.

// if( !CreateProcessA
// (
//     nullptr,        // lpApplicationName
//     &command[0],    // lpCommandLine
//     nullptr,        // lpProccessAttributes
//     nullptr,        // lpThreadAttributes
//     FALSE,          // bInheritHandles
//     0,              // dwCreationFlags
//     nullptr,        // lpEnvironment
//     nullptr,        // lpCurrentDirectory
//     &startup_info,  // lpStartupInfo
//     &process_info   // lpProccessInfo
// ))
// {
//     std::cerr << "Não foi possível abrir o bloco de notas.";
//     return;
// }