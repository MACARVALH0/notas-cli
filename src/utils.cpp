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


std::string enumToString(OpTokenType type)
{
    switch (type)
    {
        case OpTokenType::UNKNOWN: return "Unknown";
        case OpTokenType::IDENTIFIER: return "Identifier";
        case OpTokenType::STRING: return "String";
        case OpTokenType::FLAG: return "Flag";
        default: return "Inesperado";
    };
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
		{"EXIT", EXIT_op}
	};

    std::string op_name = token;

    const size_t len = op_name.length();
    if(len > 10){ return HELP_op; }

    // Conversion to uppercase.
    for(char& c : op_name) if(c >= 97 && c <= 122) c = c-32;

    // Checa se o comando existe.
    auto op_iterator = op_map.find(op_name);
    if(op_iterator != op_map.end()){ return op_iterator->second; }
    else{ return INVALID_op; }
}





std::vector<Token> tokenize(std::string& line)
{
    std::vector<Token> tokens{};
    int col = 1;

    std::vector<char> allowed_punctuation =
    {',', '.', '_', '?', '!', ':', ';', '(', ')', '[', ']', '{', '}'};

    auto isalnum = [] (char c) -> bool 
    { return std::isalnum(c); };

    auto contains = [](std::vector<char> v, char c) -> bool
    { return std::find(v.begin(), v.end(), c) != v.end(); };

    std::string buffer = "";
    OpTokenType state = OpTokenType::UNKNOWN;
    std::string::iterator char_iterator = line.begin();
    
    while(char_iterator < line.end())
    {
        switch(state)
        {
            case OpTokenType::UNKNOWN:

                if(*char_iterator == ' ')
                {
                    // Adição no iterador + reinício do loop para evitar a criação de Token to tipo UNKNOWN
                    char_iterator++;
                    continue;
                }

                if(*char_iterator == '\"')
                {
                    state = OpTokenType::STRING;
                    char_iterator++;
                    continue;
                }

                if(isalnum(*char_iterator))
                {
                    state = OpTokenType::IDENTIFIER;
                    continue;
                }


                if(*char_iterator == '-')
                {
                    state = OpTokenType::FLAG;
                    buffer += *char_iterator;
                    char_iterator++;
                    continue;
                }

            break;


            case OpTokenType::STRING:
            
                if(*char_iterator == '\"')
                {
                    if(!buffer.empty())
                    {
                        tokens.emplace_back(buffer, state, std::distance(line.begin(), char_iterator));
                        buffer.clear();
                        state = OpTokenType::UNKNOWN;

                        char_iterator++;
                        continue;
                    }

                    else
                    {
                        state = OpTokenType::UNKNOWN;
                        continue;
                    }
                }

                if(isalnum(*char_iterator) || contains(allowed_punctuation, *char_iterator) || *char_iterator == ' ')
                {
                    buffer += *char_iterator;
                }

                else
                {
                    state = OpTokenType::UNKNOWN;
                    tokens.emplace_back(buffer, state, std::distance(line.begin(), char_iterator));
                    buffer.clear();
                    continue;
                }

            break;

            case OpTokenType::IDENTIFIER:
            
                if(isalnum(*char_iterator))
                {
                    buffer += *char_iterator;
                }

                else
                {
                    tokens.emplace_back(buffer, state, std::distance(line.begin(), char_iterator));
                    state = OpTokenType::UNKNOWN;
                    buffer.clear();
                    continue;
                }

            break;



            case OpTokenType::FLAG:
                
                if(isalnum(*char_iterator))
                {
                    buffer += *char_iterator;
                }

                else
                {
                    tokens.emplace_back(buffer, state, std::distance(line.begin(), char_iterator));
                    state = OpTokenType::UNKNOWN;
                    continue;
                }

            break;
        }
        
        char_iterator++;
    }

    if(!buffer.empty())
    {
        tokens.emplace_back(buffer, OpTokenType::UNKNOWN, std::distance(line.begin(), char_iterator-1));
    }

    return tokens;
}




flag_map getFlags(token_list& tokens)
{
    flag_map flags =
    {
        {"-s", ""},
        {"-l", true}
    };

    size_t tokens_len = tokens.size();
    int i = 0;

    while(i < tokens_len)
    {
        Token token = tokens[i];

        if(token.type == OpTokenType::FLAG)
        { // Should only check for flags

            if(token.content == "-s")
            {
                try
                {
                    i++;

                    if(i > tokens_len && tokens[i].type != OpTokenType::FLAG)
                    throw std::out_of_range("Não foi possível encontrar um argumento para a flag `-s`.");

                    flags["-s"] = tokens[i].content;
                }

                catch (const std::range_error & err)
                {
                    std::cerr << "<# A flag `-s` exige um argumento.\n" << err.what() << '\n';
                    flags["-l"] = true;
                }
            }

            else if (token.content == "-l")
            {
               flags["-l"] = true; 
            }
        }
    }

    return flags;
    /*
        -> LEMBRAR
        Funções importantes para objetos `std::variant<a, b>`:

            1. `std::holds_alternative<T>(variant : value)`:
            - Verifica o valor armazenado.
            - Checa se o valor armazenado em `variant` é do tipo T.

            2. `std::get<T>(variant: T value)`:
            - Acessa o valor mas lança exceção se o tipo estiver incorreto.

            3. `std::get_if<T>(&variant: &value)`:
            - Acessa o valor de forma mais segura, retornado um nullptr se o tipo estiver incorreto.
        
            4. `std::visit(visitor: function, variant: auto value)`:
            - Aplica uma lógica automaticamente, com base no tipo armazenado.
    */
}


PROCESS_INFORMATION StartNotepad(std::string& command)
{
    STARTUPINFOA startup_info = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION process_info; // What we get as an `out` parameter

    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof startup_info; //  `sizeof` as an operator is possible when its operand is a variable and not a type.

    if( !CreateProcessA
    (
        nullptr,        // lpApplicationName
        &command[0],    // lpCommandLine
        nullptr,        // lpProccessAttributes
        nullptr,        // lpThreadAttributes
        FALSE,          // bInheritHandles
        0,              // dwCreationFlags
        nullptr,        // lpEnvironment
        nullptr,        // lpCurrentDirectory
        &startup_info,  // lpStartupInfo
        &process_info   // lpProccessInfo
    ))
    {
        std::cerr << "Não foi possível abrir o bloco de notas.";
        exit(EXIT_FAILURE);
    }

    return process_info;
}