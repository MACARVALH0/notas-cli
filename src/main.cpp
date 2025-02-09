#include <locale>
#include <locale.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <memory>

#include "utils.hpp"
#include "operation_functions.hpp"
#include "db_ops.hpp"

void startKeywordInterface(sqlite3* db, std::string& keyword, int id)
{
    int keyword_id = id;
    entry_map entries = {};
    if(keyword_id > 0){ entries = getKeywordResults(db, keyword_id); }

    // entry_map::iterator entry_map_it = entries.end(); // Why did I add this at all?

    std::cout << "< Iniciando interface para a palavra-chave `" << keyword << "` (id " << keyword_id << ").\n";
    std::string command_input = "";

    while(true)
    {
        std::cout << "\n< Digite o comando\n> ";
        std::getline(std::cin, command_input);

        if(command_input == "EXIT") break;

        std::vector<Token> tokens = tokenize(command_input);
        // TODO: Adicionar um filtro de exclusão de tokens repetidos (talvez).

        // Debug
        std::cout << "Token list:\n";
        for(Token token : tokens)
        { std::cout << "-" << " Tipo: " << enumToString(token.type) << "\t|  " << token.content << "\n"; }
        std::cout << "\n";
    }
    // bool finished = false;
    // do
    // {
    //     bool keyword_exists = keyword_id >= 0;

    //     // Exibe as entradas já existentes antes de cada nova operação realizada.
    //     if( keyword_exists && !entries.empty() ){ showEntries(entries, keyword); }
        
    //     std::cout << "\nO que fazer?\n> ";
    //     std::getline(std::cin, command_input);

    //     std::vector<Token> tokens = tokenize(command_input);
    //     // TODO: Adicionar um filtro de exclusão de tokens repetidos.

    //     // Debug
    //     std::cout << "Token list:\n";
    //     for(Token token : tokens)
    //     { std::cout << "- " << token.content << "   \t\t\t|\tTipo: " << enumToString(token.type) << "\n"; }

    //     Operation operation = getOperation(tokens[0].content);

    //     if(tokens[0].type != OpTokenType::IDENTIFIER || operation == INVALID_op)
    //     { std::cerr << "O comando de operação inserido não é adequado.\n"; }

    //     token_list op_tokens(tokens.begin() + 1, tokens.end());

        // switch(operation)
        // {
        //     case NEW_op:

        //         if(keyword_exists)
        //         {
        //             if(!db_DefineKeyword(db, keyword))
        //             {
        //                 std::cerr << "<# Não foi possível registrar a palavra-chave no banco de dados.\n";
        //                 break;
        //             }
        //         }

        //         try{ newEntry(db, op_tokens, keyword_id); }

        //         catch(std::runtime_error err)
        //         { std::cerr << err.what() << "\n"; }

        //     break;
            
        // }
        
        // 	case NEW_op:
                
        //         if(!keyword_exists)
        //         {
        //             std::cerr << "\n< Registrando keyword.\n";
        //             keyword_id = db_DefineKeyword(db, keyword);

        //             if(keyword_id > 0)
        //             { std::cout << "Nova palavra-chave registrada sob o id [" << keyword_id << "].\n\n"; }
        //             else
        //             { std::cerr << "<# Não foi possível registrar a palavra-chave.\n\n"; }
        //         }

        //         std::cout << "< Iniciando operação de escrita de uma nova entrada.\n";
        //         try
        //         { newEntry(db, tokens, keyword_id); }
        //         catch(const std::runtime_error& e)
        //         { std::cerr << e.what() << '\n'; }

        //         entries = getKeywordResults(db, keyword_id);
        //         std::clog << "< Fim da operação de escrita.\n";
                
        //     break;





        

        

        
		// switch(operation)
		// {
		// 	case NEW_op:

                
        //         if(!keyword_exists)
        //         {
        //             std::cerr << "\n< Registrando keyword.\n";
        //             keyword_id = db_DefineKeyword(db, keyword);

        //             if(keyword_id > 0)
        //             { std::cout << "Nova palavra-chave registrada sob o id [" << keyword_id << "].\n\n"; }
        //             else
        //             { std::cerr << "<# Não foi possível registrar a palavra-chave.\n\n"; }
        //         }

        //         std::cout << "< Iniciando operação de escrita de uma nova entrada.\n";
        //         try
        //         { newEntry(db, tokens, keyword_id); }
        //         catch(const std::runtime_error& e)
        //         { std::cerr << e.what() << '\n'; }

        //         entries = getKeywordResults(db, keyword_id);
        //         std::clog << "< Fim da operação de escrita.\n";
                
        //     break;


        //     case REWRITE_op:

        //         if(keyword_id > 0)
        //         {
        //             std::cout << "< Iniciando operação de reescrita de uma entrada.\n";

        //             try
        //             {
        //                 std::string entry_txt = entries.at(keyword_id);
        //                 std::cout << "entry_text: " << entry_txt << "\n";
        //                 rewriteEntry(db, tokens, entry_txt);
        //             }
        //             catch(const std::runtime_error& err)
        //             { std::cerr << err.what() << '\n'; }
        //             catch(const std::out_of_range& err)
        //             { std::cerr << err.what() << '\n'; }
        //         }
        //         else
        //         {
        //             std::cerr << "<# Não existe um id válido para operações diferentes de NEW.\n";
        //             break;
        //         }


        //         if(keyword_id > 0){ entries = getKeywordResults(db, keyword_id); }

        //     break;


        //     case DELETE_op:

        //         if(keyword_id > 0)
        //         {
        //             char opt = '\b';
        //             std::cout << "Tem certeza de que deseja deletar o(s) elemento(s) apontados? (s/n)";
        //             std::cin >> opt;
        //             if(opt == 'n')
        //             {
        //                 std::cout << "< Cancelando operaçãoo.\n";
        //             }
        //             else
        //             {
        //                 std::cout << "< Iniciando operação de reescrita de uma entrada.\n";
        //                 try
        //                 { DeleteEntry(db, tokens); }
        //                 catch(const std::runtime_error& e)
        //                 { std::cerr << e.what() << '\n'; }
        //             }
        //         }

        //         else
        //         {
        //             std::cerr << "<# Não existe um id válido para operações diferentes de NEW.\n";
        //             break;
        //         }

        //         if(keyword_id > 0){ entries = getKeywordResults(db, keyword_id); }

        //     break;

        //     case HELP_op:
        //         ShowHelpMenu();
        //     break;

        //     case EXIT_op:
        //         std::cout << "< Deixando contexto da palavra-chave \"" << keyword << "\".\n\n";
        //         finished = true;
        //     break;

        //     default:
        //         // Do jeito que o sistema está agora, provavelmente nunca chegaremos aqui, mas vai saber...
        //         ShowHelpMenu();
	    // }
}





int main()
{
    try
    {
        // Eu sinceramente não sei se isso aqui tá funcionando, o encoding nesse computador é um mistério.
        std::locale::global(std::locale(""));
        std::wcout.imbue(std::locale(""));
    }

    catch(const std::runtime_error& err)
    {
        // Sim, captura de exceção pra parte mais inútil do código...
        std::cerr << "<# Não foi possível definir o locale via `std::locale::global(std::locale(""))`.\n";
        std::cout << err.what() << ".\n";

        if(setlocale(LC_ALL, "") == nullptr)
        { std::cerr << "<# Também nãoo foi possível definir o locale com setlocale(LC_ALL, \"\"), puts.\n"; }

        std::cout << "\n";
    }


    // Começo da parte importante.
    db_ptr db = nullptr;

    try
    {
        // Retorna um ponteiro inteligente do tipo `db_ptr`
        db = getDatabasePtr();
        std::cout << "Conexão bem sucedida com o banco de dados.\n\n";
    }

    catch(const std::runtime_error& err)
    {
        // Handler para erro em obter o ponteiro para o banco de dados.
        std::cerr << err.what() << '\n';
        system("pause");
        return -1;
    }

    std::string keyword = ""; // Palavra-chave de busca.
    int keyword_id = -1; // id da palavra-chave no banco.


    std::cout << "Bem-vindo à aplicação de notas.\n";


    while(1)
    {
        std::cout << "\nBusque por uma palavra-chave. (Digite .exit para encerrar)\n> ";
        std::getline(std::cin, keyword);
        std::string trimmed_keyword = trim(keyword);
        std::cout << "\n";
        if(keyword == ".exit"){ break; }

        keyword_id = getKeywordId(db.get(), trimmed_keyword);
        std::clog << "< current keyword id: " << keyword_id << ".\n";

        startKeywordInterface(db.get(), trimmed_keyword, keyword_id);
    }

    std::cout << "Encerrando programa.\n";
    system("pause");
    return 0;
}

// std::cout << "< Entradas sob esta palavra-chave:\n";
// entry_map results = {}; // std::map de resultados.
// if(keyword_id > 0)
// {
// results = getKeywordResults(db.get(), keyword_id); // Vetor de pares [id, texto]
// if(results.empty()){std::cout << "Nenhuma entrada.\n"; }
// }
// else{ std::cout << "A palavra-chave ainda n�o foi registrada.\n"; }