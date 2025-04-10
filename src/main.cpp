#include "utils.hpp"
#include "db_ops.hpp"


void startKeywordInterface(sqlite3* db, Keyword& keyword)
{
    // Mapeia as entradas em pares chave-valor no padrão id-título.
    entry_map entries = keyword.exists() ? getKeywordResults(db, keyword.id) : entry_map{};

    // std::cout << "< Iniciando interface para a palavra-chave `" << keyword.name << "` (id " << keyword.id << ").\n"; // DEBUG
    std::string command_input = "";

    while(true)
    {
        // Exibe as entradas já existentes antes de cada nova operação realizada, caso haja alguma.
        if(!entries.empty()){ showEntries(entries, keyword.name); }

        std::cout << "\n< Digite o comando:\n> ";
        std::getline(std::cin, command_input); // Lê a linha de comando no console.

        if(command_input == "EXIT") break; // Encerra a interface caso o comando seja "EXIT".

        try
        {
            std::vector<Token> tokens = tokenize(command_input);
  
            // TODO: Adicionar um filtro de exclusão de tokens repetidos (talvez).

            // Caso a função retorne zero, a operação de saída de contexto foi acionada; encerra o loop.
            if(!processTokens(keyword, tokens, db))
            { break; }

            // Captura resultados de busca da palavra-chave novamente na variável de entrada.
            entries = getKeywordResults(db, keyword.id);
        }

        catch(const std::exception& e)
        { std::cerr << e.what() << '\n'; }
    }
}





int main()
{
    // Declara um ponteiro do tipo `db_ptr` (std::unique_ptr<sqlite3, db_ptr_deleter>)
    db_ptr db = nullptr;

    // Caminho do diretório do banco de dados.
    const std::string db_directory = "data";
    const std::string db_filename = "data";
    const std::string full_path = db_directory+"/"+db_filename+".db";

    // Verifica se o diretório existe; se não, cria um.
    if (!std::filesystem::exists(db_directory))
    { std::filesystem::create_directories(db_directory); }

    // Tenta acessar o banco de dados
    try
    {
        // Em caso de sucesso, retorna um ponteiro inteligente do tipo `db_ptr`
        db = getDatabasePtr(full_path);
        std::cout << "Conexão bem sucedida com o banco de dados.\n\n";
    }

    catch(const std::runtime_error& err)
    {
        // Handler para caso de erro em obter o ponteiro para o banco de dados.
        std::cerr << err.what() << '\n';
        system("pause"); // Pausa o console para leitura da mensagem de erro.
        return 1;
    }


    std::cout << "< Bem-vindo à aplicação de notas. >\n";


    while(1)
    {
        std::cout << "\nBusque por uma palavra-chave. (Digite .exit para encerrar)\n> ";

         // Lê o input na linha de comando e salva em `kw_name`.
        std::string kw_name = "";   // Palavra-chave de busca.
        std::getline(std::cin, kw_name);
        std::cout << "\n"; // Adiciona quebra de linha à stream de saída, para organização.
        
        // Sanitiza o input limpando espaços adjacentes ao texto.
        std::string trimmed_kw_name = trim(kw_name); 

        if(trimmed_kw_name == ".exit"){ break; } // Termina a execução do programa caso o input seja ".exit".

        // Captura o id da palavra-chave no banco de dados, caso a mesma exista. Passa como argumentos o ponteiro e o nome.
        int kw_id = getKeywordId(db.get(), trimmed_kw_name);
        std::clog << "< current keyword id: " << kw_id << ".\n"; // Log de sistema.

        Keyword keyword(trimmed_kw_name, kw_id);

        // Inicia a interface da palavra-chave com o id retornado.
        startKeywordInterface(db.get(), keyword);
    }

    // Fim da execução do programa. Pausa o console e aguarda input do usuário para fechamento.
    std::cout << "Encerrando a aplicação.\n";
    system("pause");
    return 0;
}