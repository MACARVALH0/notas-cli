#include "utils.hpp"

static bool bisalnum(char c)
{ return std::isalnum(static_cast<unsigned char>(c)) != 0; };

static bool contains(std::unordered_set<char>& set, char c)
{ return set.count(c) > 0; };

static int col(const std::string::iterator& line_start, const std::string::iterator& it)
{ return std::distance(line_start, it); };

static void to_initial(std::string& buffer, OpTokenType& state)
{ buffer.clear(); state = OpTokenType::INITIAL; };

template <typename Iterator, typename ...Args>
static void log_err(std::string& line, Iterator it, Args... args)
{
    const int pos = col(line, it);
    std::cerr  << "<# ";
    (std::cerr << ... << args) << "\n";
    std::cerr << " (pos. " << pos << ")\n";
}
static void log_err(const std::string& err)
{ std::cerr << "<# " << err; }


/**
 * @brief Processa a situação de caso inicial/padrão do tokenizador.
 * 
 * @param it Iterador para a linha de comando.
 * @return O estado `OpTokenType` identificado no iterador.
 */
static const OpTokenType processInitialCase(std::string::iterator& it)
{
    // Ignora espaços em branco.
    while(*it == ' ') it++;

    // Se o caractere for alfanumérico, retorna o estado IDENTIFIER.
    if(bisalnum(*it)) return OpTokenType::IDENTIFIER;

    // Se o caractere for `-`, retorna o estado FLAG.
    if(*it == '-') return OpTokenType::FLAG;

    // Se o caractere for `"`, retorna o estado STRING.
    if(*it == '\"')
    {
        it++; // Pula o caractere que abre aspas.
        return OpTokenType::STRING;
    }

    // Caractere não reconhecido encontrado, retorna state UNKNOWN.
    return OpTokenType::UNKNOWN;
}


/**
 * @brief Processa uma flag a partir da linha do comando.
 * 
 * @param line A linha de comando completa (usada apenas para mensagens de erro).
 * @param it Iterador para a linha de comando. Deve apontar para um hífen (`-`).
 * @return A flag processada.
 * @throws std::runtime_error Caso haja algum erro no processamento da flag.
 */
static std::string processFlagCase(std::string& line, std::string::iterator& it)
{
    std::string::iterator line_start = line.begin();
    std::string::iterator line_end = line.end();

    const std::string::iterator start = it;
    const int max_dash_count = 2; // Número máximo de hífens permitidos.
    int dash_count = 0;           // Contador de hífens.
    std::string buffer;           // Buffer para armazenar a flag.

    // Passo 1: Processa os hífens iniciais.
    // Como o iterador já aponta para um hífen, não precisamos verificar se a linha está vazia.
    while (it != line_end && *it == '-')
    {
        buffer += '-';
        dash_count++;
        it++;
    }

    // Passo 2: Valida o número de hífens.
    // Se houver mais hífens do que o permitido, retorna uma string vazia para indicar erro.
    if (dash_count > max_dash_count)
    {
        // Flag inválida.
        ErrorMsg err;
        err << "(Coluna " << col(line_start, start) << ") Flag `" << buffer << "` inválida.\n";
        throw std::runtime_error(err.get());
    }

    // Passo 3: Processa os caracteres alfanuméricos da flag.
    while (it != line_end && bisalnum(*it))
    {
        buffer += *it;
        it++;
    }

    // Passo 4: Retorna a flag processada.
    // Se não houver caracteres alfanuméricos após os hífens, a flag é inválida.
    if (buffer.size() == dash_count)
    {
        // Flag inválida (apenas hífens, sem identificador).
        ErrorMsg err;
        err << "(Coluna " << col(line_start, start) << ") Flag `" << buffer << "` inválida (apenas hífens, sem identificador).\n";
        throw std::runtime_error(err.get());
    }

    return buffer;
}


/**
 * @brief Processa uma string a partir da linha do comando.
 * 
 * @param line A linha de comando completa (usada apenas para mensagens de erro).
 * @param it Iterador para a linha de comando. Deve apontar para um caractere de aspas (`"`).
 * @return A string processada.
 * @throws std::runtime_error Caso haja algum erro no processamento da string.
 */
static std::string processStringCase(std::string& line, std::string::iterator& it)
{
    std::string::iterator line_start = line.begin();
    std::string::iterator line_end = line.end();

    std::string buffer; // Define o buffer da string.
    const std::string::iterator start = it; // Salva o iterador do ponto de partida da análise.

    // Passo 1: Acumula caracteres no buffer até encontrar o próximo caractere `"` ou chegar no fim da linha.
    while(it != line_end && *it != '\"')
    {
        buffer += *it;
        it++;
    }

    // Passo 2: Confere se a string está fechada ou se alcançou o fim da linha de leitura.
    if(it == line_end)
    {
        ErrorMsg err; // Logger de erro.
        err << "(Coluna " << col(line_start, start) << ") Caractere `\"` inválido." << " (*it: " << *it << ")\n";
        throw std::runtime_error(err.get()); // Lança exceção.
    }

    // Passo 3: Confere se a string no buffer está vazia ou contém apenas espaços.
    const std::string trimmed_buffer = trim(buffer);
    if(trimmed_buffer.empty())
    {
        ErrorMsg err; // Logger de erro.
        err << "(Coluna " << col(line_start, start) << ") Strings vazias não são aceitas.\n";
        throw std::runtime_error(err.get()); // Lança exceção.
    }

    // Passo 4: Caso nenhuma exceção seja lançada na análise, retorna o buffer.
    return buffer;
}


/*
    PONTOS A MELHORAR:

    - [x] Reduzir a densidade e tornar mais legível;
    - [x] Implementar um mecanismo mais robusto de tratamento de erros;
    - [x] Utilizar uma estrutura de dados mais eficiente para vetores e verificações de pertinência;
    - [x] Refatorar trechos de código para reduzir redundância;
    - [x] Implementar um padrão e aumentar consistênca nas mensagens de log;
    - [x] Aumentar clareza no processo de encerramento do buffer.
*/
std::vector<Token> tokenize(std::string& line)
{
    std::cout << "<! Iniciando tokenizador do comando.\n";

    // Vetor que armazenará os tokens gerados.
    std::vector<Token> tokens{};

    OpTokenType state = OpTokenType::INITIAL;   // Estado inicial da máquina de estados.
    std::string buffer = "";

    // std::cout << "<! Iniciando laço principal da máquina de estados.\n";

    // Laço principal da máquina de estados.
    auto line_start = line.begin();
    auto char_iterator = line.begin(); // Iterador que percorrerá a linha.
    while(char_iterator != line.end())
    {
        switch(state)
        {
            case OpTokenType::INITIAL:

                state = processInitialCase(char_iterator);

                if(state == OpTokenType::UNKNOWN)
                {
                    ErrorMsg err;
                    err << "(Coluna " << col(line_start, char_iterator) << ") Caractere `" << *char_iterator << "` não identificado.\n" ;
                    throw std::runtime_error(err.get());
                }

            break;


            case OpTokenType::IDENTIFIER:

                // Acumula os caracteres alfanuméricos no buffer.
                while(char_iterator != line.end() && bisalnum(*char_iterator))
                {
                    buffer += *char_iterator;
                    char_iterator++;
                }

                // Adiciona o token do tipo IDENTIFIER ao vetor de tokens.
                tokens.emplace_back(buffer, state, col(line_start, char_iterator));
                to_initial(buffer, state); // Retorna ao estado inicial.
                
            break;
            

            case OpTokenType::FLAG:
                buffer = processFlagCase(line, char_iterator);

                tokens.emplace_back(buffer, state, col(line_start, char_iterator));
                to_initial(buffer, state);
            break;


            case OpTokenType::STRING:
                buffer = processStringCase(line, char_iterator);

                tokens.emplace_back(buffer, state, col(line_start, char_iterator));
                to_initial(buffer, state);
            break;
        }
    }


    // Verifica se o buffer está vazio ao final da análise.
    if(!buffer.empty())
    {
        ErrorMsg err;
        err << "(Coluna " << col(line_start, char_iterator) << ") Buffer não vazio ao final do tokenizador: " << buffer << "\n";
        throw std::runtime_error(err.get());
    }

    std::cout << "<! A função do tokenizador foi concluída com sucesso.\n";
    return tokens;
}