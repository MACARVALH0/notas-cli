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
        // Exibe as entradas já existentes antes de cada nova operação realizada.
        bool keyword_exists = keyword_id >= 0;
        if( keyword_exists && !entries.empty() ){ showEntries(entries, keyword); }

        std::cout << "\n< Digite o comando\n> ";
        std::getline(std::cin, command_input);

        if(command_input == "EXIT") break;

        try
        {
            std::vector<Token> tokens = tokenize(command_input);
            // TODO: Adicionar um filtro de exclusão de tokens repetidos (talvez).

            // Debug
            std::cout << "Token list:\n";
            size_t max_size_a = 12;
            for(Token token : tokens)
            {
                std::string enum_string = enumToString(token.type);
                int blank_space_size = max_size_a - enum_string.size();

                std::cout << "-" << " Tipo: " << enum_string << std::string (blank_space_size, ' ')  << "|  " << token.content << "\n";
            }
            std::cout << "\n";
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        



    }
    // bool finished = false;
    // do
    // {
    //     

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
    // Declara um ponteiro do tipo `db_ptr` (std::unique_ptr<sqlite3, db_ptr_deleter>)
    db_ptr db = nullptr;

    // Tenta acessar o banco de dados
    try
    {
        // Em caso de sucesso, retorna um ponteiro inteligente do tipo `db_ptr`
        db = getDatabasePtr();
        std::cout << "Conexão bem sucedida com o banco de dados.\n\n";
    }

    catch(const std::runtime_error& err)
    {
        // Handler para caso de erro em obter o ponteiro para o banco de dados.
        std::cerr << err.what() << '\n';
        system("pause"); // Pausa o console para leitura da mensagem de erro.
        return -1;
    }

    std::string keyword = "";   // Palavra-chave de busca.
    int keyword_id = -1;        // id da palavra-chave no banco.


    std::cout << "< Bem-vindo à aplicação de notas. >\n";


    while(1)
    {
        std::cout << "\nBusque por uma palavra-chave. (Digite .exit para encerrar)\n> ";
        std::getline(std::cin, keyword); // Lê o input na linha de comando e salva em `keyword`.
        std::string trimmed_keyword = trim(keyword); // Sanitiza o input limpando espaços adjacentes ao texto.
        std::cout << "\n"; // Adiciona quebra de linha.
        if(keyword == ".exit"){ break; } // Termina a execução do programa caso o input seja ".exit".

        // Captura o id da palavra-chave no banco de dados, caso a mesma exista. Passa como argumentos o ponteiro e o nome.
        keyword_id = getKeywordId(db.get(), trimmed_keyword);
        std::clog << "< current keyword id: " << keyword_id << ".\n"; // Log de sistema.

        // Inicia a interface da palavra-chave com o id retornado.
        startKeywordInterface(db.get(), trimmed_keyword, keyword_id);
    }

    // Fim da execução do programa. Pausa o console e aguarda input do usuário para fechamento.
    std::cout << "Encerrando programa.\n";
    system("pause");
    return 0;
}