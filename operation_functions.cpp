#include "operation_functions.hpp"

str_vector::iterator getIterator(str_vector v, const std::string& flag) { return std::find(v.begin(), v.end(), flag); }

int parseInt(const std::string& value)
{
    try                             { return std::stoi(value); }
    catch(std::invalid_argument& e) { return -1; }
    catch(std::out_of_range& e)     { return -1; }
}

// [x]
void newEntry(sqlite3* db, const str_vector& tokens, int parent_id)
{
    str_vector flags = {"-l"};
    
    std::string entry_text = "";


    if(!tokens.empty() && getIterator(tokens, flags[0]) != tokens.end())
    { // Long entry

        std::string filename = "temp.txt";

        // Lê o conteúdo do arquivo temporário `filename`
        auto readFile = [filename]() -> std::string
        {
            std::ifstream file_r(filename);
            std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
            return content;
        };

        std::ofstream file_w(filename);
        file_w << "Edite aqui o conteúdo da nota.\n";
        file_w.close();

        std::string command = "notepad.exe " + filename;
        system(command.c_str());
        system("pause");

        // Lendo o conteúdo do arquivo temporário.
        entry_text = readFile();

        if(!entry_text.empty())
        {
            std::cout << "\n< Adicionando entrada ao banco de dados.\n";
            // Adicionando ao banco de dados.
            if(!db_WriteNote(db, parent_id, entry_text))
            {
                std::cerr << "<# Não foi possível adicionar a nota ao banco de dados.\n";
                throw std::runtime_error("<# A operação de adicionar a nota ao banco de dados não foi concluída corretamente.\n");
            }
        }

        else{ std::cout << "\n< A nota vazia não será salva.\n"; }

        // Limpando o conteúdo do arquivo temporário.
        file_w << "";
    }


    else
    {
        std::cout << "\nDigite o conteúdo da entrada abaixo.\n> ";
        std::getline(std::cin, entry_text);

        if(!entry_text.empty())
        {
            std::cout << "\n< Adicionando entrada ao banco de dados.\n";
            // Adicionando ao banco de dados.
            if(!db_WriteNote(db, parent_id, entry_text))
            {
                std::cerr << "<# Não foi possível adicionar a nota ao banco de dados.\n";
                throw std::runtime_error("<# A operação de adicionar a nota ao banco de dados não foi concluída corretamente.\n");
            }
        }

        else{ std::cout << "\n< A nota vazia não será salva.\n"; }

        std::clog << "< Operação concluída.\n";
    }
}



// [x]
// TODO - implementar texto da entrada na edição de entradas
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
        std::cerr << "<# É necessário inserir um id de entrada válido.\n";
        throw std::runtime_error("<# Não foi possível encontrar o id da entrada.\n");
    }


    if(!tokens.empty() && getIterator(tokens, flags[0]) != tokens.end())
    { // In case of a long entry
        std::string filename = "temp.txt";

        // Lê o conteúdo do arquivo temporário `filename`
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

        // Lendo o conteúdo do arquivo temporário.
        entry_text_input = readFile();

        if(!entry_text_input.empty())
        {
            // Adicionando ao banco de dados.
            if(!db_RewriteNote(db, entry_id, entry_text_input))
            {
                std::cerr << "<# Não foi possível sobrescrever a nota no banco de dados corretamente.\n";
                throw std::runtime_error("O id inserido não é válido ou não existe.");
            }
        }

        else{ std::cout << "\n< A nota vazia não será salva.\n"; }

        // Limpando o conteúdo do arquivo temporário.
        file_w << "";
    }

    else // Rewriting entry in short mode
    {
        std::cout << "Escreva o o texto para sobrescrever o conteúdo da entrada original:\n> ";
        std::cout << "Texto original:\n";
        std::cout << entry_txt << "\n> ";

        std::getline(std::cin, entry_text_input);

        if(!entry_text_input.empty())
        {
            // Adicionando ao banco de dados.
            if(!db_RewriteNote(db, entry_id, entry_text_input))
            {
                std::cerr << "<# Não foi possível sobrescrever a nota no banco de dados corretamente.\n";
                throw std::runtime_error("O id inserido não é válido ou não existe.\n");
            }
        }

        else{ std::cout << "\n< A nota vazia não será salva.\n"; }
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

    // First part: Captura os números de id de cada entrada a ser deletada.
    for(const std::string& token : tokens)
    {
        if(isNumber(token))
        {
            int id = parseInt(token);
            if(!id)
            {
                std::string error_msg = "<# O valor " + token + " não é um id válido.\n";
                throw std::runtime_error(error_msg);
            }
            else { id_list.push_back(id); }
        }
    }

    // Second part: Chama as funções responsáveis por deletar para cada id.

    for(int id : id_list)
    {
        if(!db_DeleteNote(db, id))
        {
            std::string id_str = std::to_string(id);
            std::string delete_error_msg = "<# Não foi possível deletar o item de id " + id_str + " corretamente.\n";
            throw std::runtime_error(delete_error_msg);
        }
    }
}


void ShowHelpMenu()
{
    std::cout << "\nLista de comandos disponíveis:\n\n";
    std::cout << 
    R"(
        NEW     | Entra no modo de escrita de uma nova nota sob a última palavra-chave.
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
        -   `HELP` -> Exibe a interface de apoio. Não recebe argumentos.

        $ EXIT;
        -   `EXIT` -> Sai do escopo da palavra-chave.
    )";
    
    std::cout << "\n";

}