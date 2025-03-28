#include "utils.hpp"
#include "operation_functions.hpp"
#include "process_flags.hpp"

void processTokens(Keyword& keyword, std::vector<Token>& tokens, sqlite3* db)
{
    std::cout << "<! Iniciando `processTokens`.\n";

    // 1. Captura o tipo de operação a ser realizada baseado no conteúdo do primeiro token da lista.
    const Operation op = getOperation(tokens[0].content);

    // 2. Tratamento de erro preliminar: Problemas que não devem ser alcançados, mas previne caso aconteçam.
    if(tokens[0].type != OpTokenType::IDENTIFIER || op == UNKNOWN_op)
    {
        ErrorMsg err;
        err << "O comando `" << tokens[0].content << "` não é válido ou não existe.\n";
        throw std::runtime_error(err.get());
    }

    // 3. Organiza tokens de operação (sem o nome da operação, ou seja, sem o primeiro item).
    std::vector<Token> op_tokens(tokens.begin()+1, tokens.end()); 
    std::cout << "<! `op_tokens` definido.\n";

    // 4. Recebe lista de configurações de flags definidas.
    flag_setup_map flag_setup = getFlagSetup(op_tokens);
    std::cout << "<! `flag_setup` definido.\n";

    // 5. Executa a operação adequada baseado em `Operation op`.
    switch(op)
    {
        //////////////////// NEW: Operação de criação de nova nota.
        case NEW_op:

            // 1. Analisa se já existe um registro no banco de dados com a palavra-chave fornecida.
            if(!keyword.exists())
            {
                // 1a. Registra a palavra-chave no banco de dados.
                keyword.id = db_DefineKeyword(db, keyword.name);

                // 1b. Caso ainda não exista um registro no banco de dados com a palavra-chave após executada a operação de inserção, houve um problema com o acesso ao banco de dados.
                if(!keyword.exists())
                {
                    ErrorMsg err;
                    err << "Não foi possível registrar a keyword " << keyword.name << ".\n";
                    throw std::runtime_error(err.get());
                }
            }

            std::cout << "<! Iniciando `registerNewEntry`.\n";
            
            // 2. Tentativa de executar a operação de criação de registro.
            try { registerNewEntry(db, keyword.id, tokens, flag_setup); }
            
            catch(const std::exception& err)
            {
                std::cerr << err.what();// << '\n';
                // TODO Maybe deal with deleting keyword register in database if it was its first entry and it failed.
            }

        break;

        //////////////////// REWRITE: Operação de reescrita de uma nota.
        case REWRITE_op:

            if(!keyword.exists())
            {
                ErrorMsg err;
                err << "A palavra-chave " << keyword.name << " não está registrada e não possui entradas para serem reescritas.";
            }

            else
            {
                // TODO try-catch
                try
                { rewriteEntry(db, keyword.id, tokens, flag_setup); }

                catch(std::runtime_error& err)
                {
                    std::cerr << err.what();
                }
            }

        break;
    }
}
    

    
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