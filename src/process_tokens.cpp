#include "utils.hpp"
#include "operation_functions.hpp"
#include "process_flags.hpp"

int processTokens(Keyword& keyword, std::vector<Token>& tokens, sqlite3* db)
{
    // 1. Captura o tipo de operação a ser realizada baseado no conteúdo do primeiro token da lista.
    const Operation op = getOperation(tokens[0].content);

    // 2. Tratamento de erro preliminar: Problemas que não devem ser alcançados, mas previne caso aconteçam.
    if(tokens[0].type != OpTokenType::IDENTIFIER || op == UNKNOWN_op)
    {
        ErrorMsg err;
        err << "O comando `" << tokens[0].content << "` não é válido ou não existe.";
        throw std::runtime_error(err.get());
    }

    // 3. Organiza tokens de operação (sem o nome da operação, ou seja, sem o primeiro item).
    std::vector<Token> op_tokens(tokens.begin()+1, tokens.end()); 

    // 4. Recebe lista de configurações de flags definidas.
    flag_setup_map flag_setup = getFlagSetup(op_tokens);

    // 5. Executa a operação adequada baseado em `Operation op`.
    switch(op)
    {
        //////////////////// NEW: Operação de criação de uma nova nota.
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
                    err << "Não foi possível registrar a keyword " << keyword.name << ".";
                    throw std::runtime_error(err.get());
                }
            }
            
            // 2. Tentativa de executar a operação de criação de registro.
            try { registerNewEntry(db, keyword.id, tokens, flag_setup); }
            
            catch(const std::exception& err)
            {
                std::cerr << err.what();
                // TODO Maybe deal with deleting keyword register in database if it was its first entry and it failed.
            }

        break;

        //////////////////// REWRITE: Operação de reescrita de uma nota.
        case REWRITE_op:

            if(!keyword.exists())
            {
                ErrorMsg err;
                err << "A palavra-chave " << keyword.name << " não está registrada e não possui entradas para serem reescritas.";
                throw std::runtime_error(err.get());
            }

            else
            {
                // TODO try-catch
                try
                { rewriteEntry(db, keyword.id, tokens, flag_setup); }

                catch(std::runtime_error& err)
                { std::cerr << err.what(); }
            }

        break;

        //////////////////// DELETE: Operação de deletar uma entrada.
        case DELETE_op:

            if(!keyword.exists())
            {
                ErrorMsg err;
                err << "A palavra-chave " << keyword.name << " não está registrada e não possui itens a serem deletados.";
                throw std::runtime_error(err.get());
            }


            else
            {
                try{ deleteEntry(db, tokens, flag_setup); }

                catch(std::runtime_error& err)
                { std::cerr << err.what(); }
            }

        break;

        //////////////////// HELP: Operação de exibir menu de auxílio para uso.
        case HELP_op: showHelpMenu();
        break;

        //////////////////// EXIT: Operação de sair do contexto da palavra-chave atual.
        // FIXME Atualmente sem utilidade prática neste ponto do código.
        case EXIT_op:
            std::cout << "<! Deixando o contexto da palavra-chave `" << keyword.name << "`.\n";
            return 0;
        break;

        default: std::cerr << "<# Digite o nome de uma operação válida.\n";
    }

    return 1;
}