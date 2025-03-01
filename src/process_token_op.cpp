#include "utils.hpp"
#include "operation_functions.hpp"

void processTokenOp(Keyword& keyword, const token_list& tokens, sqlite3* db)
{
    // Captura o tipo de operação a ser realizada baseado no conteúdo do primeiro token da lista.
    const Operation op = getOperation(tokens[0].content);

    // Tratamento de erro preliminar: Problemas que não devem ser alcançados, mas previne caso aconteçam.
    if(tokens[0].type != OpTokenType::IDENTIFIER || op == UNKNOWN_op)
    {
        ErrorMsg err;
        err << "O comando `" << tokens[0].content << "` não é válido ou não existe.\n";
        throw std::runtime_error(err.get());
    }

    // Tokens de operação (sem o nome da operação).
    token_list op_tokens(tokens.begin()+1, tokens.end()); 




    switch(op)
    {
        case NEW_op:

            if(!keyword.exists())
            {
                keyword.id = db_DefineKeyword(db, keyword.name);

                if(!keyword.exists())
                {
                    ErrorMsg err;
                    err << "Não foi possível registrar a keyword " << keyword.name << ".\n";
                    throw std::runtime_error(err.get());
                }
            }

            try { registerNewEntry(db, tokens, keyword.id); }
            
            catch(const std::exception& err)
            {
                std::cerr << err.what() << '\n';
                // Maybe deal with deleting keyword register in database if it was its first entry.
            }


        break;
    }



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


//     Operation operation = getOperation(tokens[0].content);

//     if(tokens[0].type != OpTokenType::IDENTIFIER || operation == INVALID_op)
//     { std::cerr << "O comando de operação inserido não é adequado.\n"; }

//     token_list op_tokens(tokens.begin() + 1, tokens.end());