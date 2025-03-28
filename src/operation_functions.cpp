#include "operation_functions.hpp"

using context_map = std::map<Configuration, CtxConfig>;

static std::string readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    if(!file)
    {
        ErrorMsg err;
        err << "Não foi possível abrir o arquivo `" << filename << "`.";
        throw std::runtime_error(err.get());
    }

    // Lida com o conjunto de bytes do BOM (Byte Order Mark).
    size_t bom_size = 3;        // Número de bytes do BOM.
    char bom[bom_size] = {0};   // Buffer para armazenar o BOM.
    file.read(bom, bom_size);   // lê o número de bytes do BOM e os armazena em `bom`.

    /*
        Caso o arquivo esteja condificado em BOM, seus três primeiros bytes serão:
        - \xEF (239, em decimal)
        - \xBB (187, em decimal)
        - \xBF (191, em decimal)

        Se não, os três primeiros bytes devem fazer parte do conteúdo real do arquivo.

        O trecho abaixo verifica se os três primeiros bytes são indicadores de um BOM,
        ao invés de caracteres de conteúdo que deve ser lido do arquivo.

        Caso o arquivo tenha BOM, a leitura continua normalmente, uma vez que já avançamos três casas.
        Caso o arquivo não tenha BOM, retornamos ao início do arquivo.

        -> Se os três primeiros bytes não forem o Byte Order Mark, significa que lemos dados válidos sem necessidade.

        Assim, `seekg` reposiciona o cursor de leitura no início do arquivo,
        para garantir que o conteúdo a ser lido não se perca.
    */
    if (!(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF'))
    {
        file.seekg(0); // Reposiciona o cursor de leitura no início do arquivo.
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}




/**
 * @brief A função universal de configuração contextual de flags envolvidas na operação, seja de escrita (NEW), reescrita (REWRITE), etc.
 * @param flag_set O mapa de itens configuração-flag recuperados da linha de comando.
 * @param ctx O vetor de configurações contextuais da operação (basicamente as tags envolvidas, se são obrigatórias, etc).
 */
static void setupFlagSettings(const flag_setup_map& flag_set, context_map& ctx)
{
    std::cout << "<! Entrando em setupFlagSettings.\n"; // DEBUG

    // Encerra a função caso o mapa de flags esteja vazio.
    if(flag_set.empty()){ return; }

        // DEBUG
        std::cout << "\n<! Elementos contidos em `flag_set`:\n";
        auto set_it = flag_set.begin();
        while (set_it != flag_set.end())
        {
            std::cout << "{ Configuração: " << toString_Configuration(set_it->first) << ", Flag: " << "[ algum FlagValue ]" << " }\n";
            set_it++;
        }
        std::cout << "<! Laço concluído!\n";
    
    auto ctx_it = ctx.begin();

    while(ctx_it != ctx.end())
    {

        const auto config = ctx_it->first; // Atributo de configuração definido pela chave do elemento em `context_map`

        if(flag_set.find(config) != flag_set.end())
        {
            /*
                Define a flag para a configuração analisada.
                Esta flag é o valor de flag_set cuja chave é o nome da configuração apontada em `ctx`.
            */

            ctx_it->second.flag = flag_set.at(config);
            ctx_it->second.exists = true;
        }

        else if(ctx_it->second.obligatory)
        {
            ErrorMsg err;
            err << "A propriedade obrigatória `" << toString_Configuration(config) << "` não foi definida.";
            throw std::runtime_error(err.get()); // Lança um erro.
        }

        ctx_it++;
    }
}



static void newEntryLong(sqlite3* db, int parent_id)
{
    std::cout << "<! Entrando em `newEntryLong`.\n";

    const std::string filename = "temp.txt";
    std::string default_insert = "\n< Edite aqui o conteúdo da nota.\n";

    // Lê o conteúdo do arquivo temporário `filename`
    auto readFile = [filename]() -> std::string
    {
        std::ifstream file_r(filename);
        std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
        return content;
    };

    std::cout << "<! Abrindo o arquivo de edição de texto e adicionando o texto placeholder.\n";


    // Captura a informação de processo do bloco de notas para lidar com os handles.
    std::cout << "< Escreva o conteúdo da nota no editor de texto.\n";
    std::string command = "notepad.exe " + filename;
    // PROCESS_INFORMATION process_info = StartNotepad(command);
    // std::cout << "<! `process_info` da operação capturado com sucesso.\n";

    StartNotepad(command);
    // Esperando o processo do bloco de notas ser terminado.
    // WaitForSingleObject(process_info.hProcess, INFINITE);

    // CloseHandle(process_info.hProcess);
    // CloseHandle(process_info.hThread);

    std::cout << "<! Handle fechado com sucesso.\n";

    // Lê o conteúdo do arquivo temporário.
    const std::string& entry_text = readFile();

    std::cout << "<! Conteúdo do arquivo de texto capturdo com sucesso.\n";

    if(entry_text.empty() || entry_text == default_insert)
    {
        std::cout << "<! A nota vazia não será adicionada ao banco dados.\n";
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

static void newEntryShort(sqlite3*db, int parent_id, const std::string& entry_text)
{
    std::cout << "<! Entrou com sucesso em `newEntryShort`.\n";

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

void registerNewEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_set)
{
    std::cout << "<! Sucesso ao entrar em `registerNewEntry`.\n";

    if(flag_set.empty())
    {
        /*
            Caso nenhuma flag esteja acompanhando o comando `new`,
            apenas executa uma nova operação de escrita com o editor de texto.
        */

        newEntryLong(db, parent_id);
    }

    else
    {
        context_map ctx = 
        {
            {Configuration::SIZE, CtxConfig(false)}
        };

        // setupFlagSettings(flag_set, ctx);
        std::cout << "<! setupFlagSettings` executado com sucesso.\n";


        // FIXME Talvez faça sentido organizar melhor essa parte depois, mas agora serei mais direto.
        const Flag SIZE_FLAG = ctx.at(Configuration::SIZE).flag;
        std::cout << "<! `SIZE_FLAG` definido.\n";

        // Caso a flag `-s` ou `--short` estejam presentes na linha de comando.
        if(SIZE_FLAG.value == FlagValue::SHORT)
        { newEntryShort(db, parent_id, std::get<std::string>(SIZE_FLAG.arg)); }

        /*
            Caso a flag <`-l`|`--long`> esteja presente na linha de comando
            ou a tag <`-s`|`--short`> não esteja definida.
        */
        else
        { newEntryLong(db, parent_id); }
    }

    
}



static std::string rewriteLong(sqlite3* db, u_int entry_id)
{
    std::cout << "<! Entrando em `rewriteLong`.\n"; // DEBUG

    // Captura o atual conteúdo da nota identificada pelo ID encontrado.
    const std::string cur_content = getEntryContent(db, entry_id);

    // O nome do arquivo temporário onde a nota será editada.
    const std::string FILENAME = "temp.txt";
    std::ofstream file(FILENAME);
    file << cur_content;
    file.close();

    std::string command = "notepad.exe " + FILENAME;
    StartNotepad(command);

    std::string temp_file_content = readFile(FILENAME);

    return temp_file_content;
}

static void rewrite_Save(sqlite3* db, u_int entry_id, const std::string& content)
{
    if(content.empty())
    {
        std::cout << "<# Não é possível substituir o conteúdo de uma entrada por uma string vazia.\n";
        return;
    }

    if(!db_RewriteNote(db, entry_id, content))
    {
        ErrorMsg err;
        err << "Houve um problema na tentativa de reescrever a entrada sob o id `" << entry_id << "`.";
        throw std::runtime_error(err.get());
    }
}

static std::string rewrite_getContent(sqlite3* db, u_int entry_id, const Flag& SIZE_FLAG)
{
    switch(SIZE_FLAG.value)
    {
        // Caso as flags `-s` ou `--short` estejam presentes na linha de comando.
        case FlagValue::SHORT: return std::get<std::string>(SIZE_FLAG.arg);
        break;

        // Caso as flags `-l` ou `--long` estejam presentes na linha de comando.
        case FlagValue::LONG: return rewriteLong(db, entry_id);
        break;

        // Caso nenhuma outra flag válida de tipo de entrada seja identificada.
        default: return rewriteLong(db, entry_id);
        break;
    }
}

static std::optional<u_int> getEntryId(const std::vector<Token>& tokens)
{
    auto it = tokens.begin();

    while(it != tokens.end())
    {
        if(it->type == OpTokenType::IDENTIFIER && isNumber(it->content))
        { return parseInt(it->content); }

        else { it++; continue; }
    }

    // Caso o ID não seja encontrado, retorna um ponteiro nulo.
    return std::nullopt;
}


// TODO Reescrever essa função.
void rewriteEntry(sqlite3* db, int parent_id, const std::vector<Token>& tokens, const flag_setup_map& flag_set)
{
    std::cout << "<! Entrando em `rewriteEntry`.\n"; // DEBUG
    // Capturando o ID da entrada a ser reescrita.
    const std::optional<u_int> opt_entry_id = getEntryId(tokens);

    // Lança um erro caso não encontre um id de busca.
    if(!opt_entry_id.has_value()) 
    { throw std::runtime_error("Não foi encontrado um ID de entrada válido.\n"); }

    const u_int entry_id = opt_entry_id.value();
    std::cout << "<! `enty_id` capturado com sucesso.\n"; // DEBUG

    // FIXME Solução provisória para diferenciação de quando há ou não flags na linha de comando.
    if(flag_set.empty())
    {
        std::cout << "<! Não existem flags definidas na linha de comando.\n"; // DEBUG
        const std::string content = rewriteLong(db, entry_id);

        std::cout << "Conteúdo da nota: \"" << content << "\".\n";

        rewrite_Save(db, entry_id, content);

        return;
    }

    // Definindo as configurações relevantes neste contexto.
    context_map ctx = 
    {
        {Configuration::SIZE, CtxConfig(false)}
    };

    // Atribui as flags às suas respectivas configurações.
    setupFlagSettings(flag_set, ctx); // TODO Consertar interação de `setupFlagSettings` com o novo tipo de `ctx`.
    // std::cout << "<! setupFlagSettings` executado com sucesso.\n"; // DEBUG

    const Flag SIZE_FLAG = ctx.at(Configuration::SIZE).flag;

    const std::string content = rewrite_getContent(db, entry_id, SIZE_FLAG);

    rewrite_Save(db, entry_id, content);
}




std::unordered_set<u_int> getEntryIdList(const std::vector<Token>& tokens)
{
    std::unordered_set<u_int> id_list;

    auto it = tokens.begin();

    while(it != tokens.end())
    {
        if(it->type == OpTokenType::IDENTIFIER && isNumber(it->content))
        { id_list.insert(parseInt(it->content)); }

        it++;
    }

    return id_list;
}


void deleteEntry(sqlite3* db, const std::vector<Token>& tokens, const flag_setup_map& flag_set)
{
    std::unordered_set<u_int> id_list = getEntryIdList(tokens);

    // Se a lista estiver vazia, nenhum id válido foi encontrado, retorna um erro.
    if(id_list.empty())
    { throw std::runtime_error("<# Nenhum id válido foi encontrado na linha de comando.\n"); }

    // Interação com flags no futuro, caso haja alguma.

    // TODO O ideal é que uma só função seja chamada, passando como argumento toda a lista de ids.
    for(u_int id : id_list)
    {
        if(!db_DeleteNote(db, id))
        {
            ErrorMsg err;
            err << "Não foi possível deletar o item com  o id `" << id << "` do banco de dados.";
            throw std::runtime_error(err.get());
        }
    }
}


void showHelpMenu()
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