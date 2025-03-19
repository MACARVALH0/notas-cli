#include "operation_functions.hpp"

#define SAVE_ERR std::cerr << err.what() << '\n'; throw std::runtime_error("<# Não foi possível salvar a nota.\n");


inline void handleSaveError(const std::runtime_error& err)
{
    std::cerr << err.what() << '\n';
    throw std::runtime_error("<# Não foi possível salvar a nota.\n");
}

// TODO Deletar isso aqui...
// Pra quê que serve isso aqui mesmo? Eu sequer uso essa parte?
str_vector::iterator getIterator(str_vector v, const std::string& flag) { return std::find(v.begin(), v.end(), flag); }



void deleteTempFile(){}


/**
 * @brief A função universal de configuração contextual de flags envolvidas na operação, seja de escrita (NEW), reescrita (REWRITE), etc.
 * @param flag_set O mapa de itens configuração-flag recuperados da linha de comando.
 * @param ctx O vetor de configurações contextuais da operação (basicamente as tags envolvidas, se são obrigatórias, etc).
 */
void setupFlagSettings(const flag_setup_map& flag_set, std::vector<ContextConfiguration>& ctx)
{
    auto ctx_it = ctx.begin();
    auto ctx_end = ctx.end();

    while(ctx_it != ctx_end)
    {
        // Define a configuração analisada.
        const auto config = ctx_it->configuration;

        if(flag_set.find(config) != flag_set.end())
        {
            /*
                Define a flag para a configuração analisada.
                Esta flag é o valor de flag_set cuja chave é o nome da configuração apontada em `ctx`.
            */
            ctx_it->flag = flag_set.at(config);
        }

        // Caso a configuração não seja encontrada no `flag_set`, mas ainda assim seja obrigatória.
        else if(ctx_it->obligatory)
        {
            ErrorMsg err;
            err << "A propriedade obrigatória `" << toString_Configuration(config) << "` não foi definida.";
            throw std::runtime_error(err.get()); // Lança um erro.
        }

        ctx_it++;
    }
}


void registerNewEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_set)
{
    std::cout << "<! Sucesso ao entrar em `registerNewEntry`.\n";

    std::vector<ContextConfiguration> ctx =
    {
        {Configuration::SIZE, false}
    };

    setupFlagSettings(flag_set, ctx);
    std::cout << "<! setupFlagSettings` executado com sucesso.\n";

    
    // Talvez faça sentido organizar melhor essa parte depois, mas agora serei mais direto.

    const Flag SIZE_FLAG = flag_set.at(Configuration::SIZE);
    std::cout << "<! `SIZE_FLAG` definido.\n";

    // Caso a flag `-s` ou `--short` estejam presentes na linha de comando.
    if(SIZE_FLAG.value == FlagValue::SHORT)
    { newEntryShort(db, parent_id, std::get<std::string>(SIZE_FLAG.arg)); }

    // Caso a flag <`-l`|`--long`> esteja presente na linha de comando
    // ou a tag <`-s`|`--short`> não esteja definida.
    else
    { newEntryLong(db, parent_id); }
}









// [ ]
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


void newEntryLong(sqlite3* db, int parent_id)
{
    const std::string filename = "temp.txt";
    std::string default_insert = "\n< Edite aqui o conteúdo da nota.\n";

    // Lê o conteúdo do arquivo temporário `filename`
    auto readFile = [filename]() -> std::string
    {
        std::ifstream file_r(filename);
        std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
        return content;
    };

    // Abre o arquivo temporário, adiciona o texto de placeholder e fecha o mesmo.
    std::ofstream file_w(filename);
    file_w << default_insert;
    file_w.close();

    // Captura a informação de processo do bloco de notas para lidar com os handles.
    std::string command = "notepad.exe " + filename;
    PROCESS_INFORMATION process_info = StartNotepad(command);

    // Esperando o processo do bloco de notas ser terminado.
    WaitForSingleObject(process_info.hProcess, INFINITE);

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    // Lê o conteúdo do arquivo temporário.
    const std::string& entry_text = readFile();

    if(entry_text.empty() || entry_text == default_insert)
    {
        std::cout << "< A nota vazia não será adicionada ao banco dados.\n";
        return;
    }

    else
    {
        // Adicionando do banco de dados.
        if(!db_WriteNote(db, parent_id, entry_text))
        {
            throw std::runtime_error("Houve um problema na tentativa de adicionar a nota ao banco de dados.\n");
        }
    }

    if(!DeleteFileW(L"temp.txt"))
    {
        std::cerr << "<# Não foi possível excluir o arquivo temporário.";
    }

}


void newEntryShort(sqlite3*db, int parent_id, const std::string& entry_text)
{
    std::cout << "<! Entrou com sucesso em `newEntryShort`.\n";

    std::string filename = "temp.txt";

    if(entry_text.empty())
    {
        std::cout << "< A nota vazia não será adicionada ao banco dados.\n";
        return;
    }

    else
    {
        std::cout << "<! Executando db_WriteNote.\n";
        if(!db_WriteNote(db, parent_id, entry_text))
        {
            throw std::runtime_error("Houve um problema na tentativa de adicionar a nota ao banco de dados.\n");
        }
    }
}