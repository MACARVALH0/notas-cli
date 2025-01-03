#include "utils.hpp"



void showEntries(entry_map& entries, const std::string& keyword)
{
    std::cout << "\n";
	const std::string kw_output = "Entradas relacionadas a \'" + keyword + "\':";
	std::cout << kw_output << "\n";
	std::cout << std::string(kw_output.length(), '=') << "\n";

    for(const auto& entry : entries)
	{ std::cout << "- (" << entry.first << ") \"" << entry.second << "\"" << "\n\n"; }
}


std::vector<std::string> split(const std::string& txt, char delimiter)
{
    std::vector<std::string> tokens;

    auto start = 0UL;
    auto end = txt.find(delimiter, start);

    while(end != std::string::npos)
    {
        tokens.push_back(txt.substr(start, end-start));
        start = end+1;
        end = txt.find(delimiter, start);
    }
    tokens.push_back(txt.substr(start));

    return tokens;
}


void brush(str_vector& tokens, const std::string& excludent) // se isso fosse um programa sério eu retornaria algo, provavelmente...
{
    const size_t len = tokens.size();
    str_vector::iterator it = tokens.begin();

    for(int i = 0; i < len; i++)
    {
        if(tokens[i] == excludent){ tokens.erase(it+i); } else { continue; }
    }
}


std::string getCommand(std::vector<std::string>& tokens) // Esse trecho isola a string de comando
{
    /*
        1. Recebe um vetor de strings `tokens`
        2. Salva um ponteiro `iterator` para o primeiro elemento do vetor (onde deveria estar o comando, na prática)
        3. Salva o valor apontado por `iterator` em `cmd`
        4. Apaga do vetor `tokens` o elemento apontado por `iterator`
        5. Retorna a string `cmd`
    */

    auto iterator = tokens.begin();
    std::string cmd = *iterator;
    tokens.erase(iterator);
    return cmd;
}


Operation getOperation(std::string token)
{
    /*
        1. Recebe uma string `token` com o nome da operação
        2. Declara um mapa `op_map` com par chave-valor de string e enumerator do tipo `Operation`
        3. Analisa o tamanho da string e, caso esta tenha mais de 10 caracteres, retorna o enumerator para `HELP_op`
        4. Converte o token em maiúsculas
        5. Busca no mapa `op_map` o valor do tipo `Operation` correspondente à string
        6. Retorna o enumerator correspondente, caso encontre, senão retorna o enumarator para `HELP_op`
    */

	std::map<std::string, Operation> op_map = 
	{
		{"NEW", NEW_op},
		{"REWRITE", REWRITE_op},
		{"DELETE", DELETE_op},
		{"HELP", HELP_op},
		{"EXIT", EXIT_op},
	};

    std::string op_token = token;

    const size_t len = op_token.length();
    if(len > 10){ return HELP_op; }

    // Convert to uppercase
    for(int i = 0; i < token.length(); i++)
    {
        char c = token[i];
        if(c >= 97 && c <= 122)
        {
            c = c == ' ' ? c : c-32;
            op_token[i] = c;
        }
    }

    // Checar se o comando existe.
    auto op_iterator = op_map.find(op_token);
    if(op_iterator != op_map.end()){ return op_iterator->second; }
    else{ return HELP_op; }
}