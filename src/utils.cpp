#include "utils.hpp"

void DEBUG_showTokens(const std::vector<Token>& tokens)
{
     // Debug
    std::cout << "\nToken list:\n";
    size_t max_size_a = 12; // Tamanho máximo que deve ser mantido entre a string do tipo de token e o caractere de separação `|`.
    for(Token token : tokens)
    {
        std::string enum_string = toString_OpTokenType(token.type); // Recebe uma string nomeando o tipo de token.
        int blank_space_size = max_size_a - enum_string.size(); // Encontra a quantidade de espaços a serem adicionados entre o tipo de token e o caractere `|`.

        // Informa o token.
        std::cout << "-" << " Tipo: " << enum_string << std::string (blank_space_size, ' ')  << "|  " << token.content << "\n";
    }
    std::cout << "\n";
}


void showEntries(entry_map& entries, const std::string& keyword)
{
    std::cout << "\n";
	const std::string kw_output = "Entradas relacionadas a \'" + keyword + "\':";
	std::cout << kw_output << "\n";
	std::cout << std::string(kw_output.length(), '=') << "\n";

    for(const auto& entry : entries)
	{ std::cout << "- (" << entry.first << ") \"" << entry.second << "\"" << "\n\n"; }
}


std::string& trim(std::string& text)
{
    size_t start = text.find_first_not_of(" \t\n\r\f\v");
    size_t end = text.find_last_not_of(" \t\n\r\f\v");

    text = (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
    
    return text;
}


int parseInt(const std::string& value)
{
    try                             { return std::stoi(value); }
    catch(std::invalid_argument& e) { return -1; }
    catch(std::out_of_range& e)     { return -1; }
}


std::string toString_OpTokenType(OpTokenType type)
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
    else{ return UNKNOWN_op; }
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