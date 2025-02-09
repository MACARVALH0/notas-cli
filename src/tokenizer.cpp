#include "utils.hpp"

static bool bisalnum(char c)
{ return std::isalnum(static_cast<unsigned char>(c)) != 0; };

static bool contains(std::unordered_set<char>& set, char c)
{ return set.count(c) > 0; };

static int col(std::string& line, auto& it)
{ return std::distance(line.begin(), it); };

void to_initial(std::string& buffer, OpTokenType& state)
{ buffer.clear(); state = OpTokenType::UNKNOWN; };

template <typename Iterator, typename... Args>
static void log_err(std::string& line, Iterator it, Args... args)
{
    const int pos = col(line, it);
    std::cerr  << "<# ";
    (std::cerr << ... << args) << "\n";
    std::cerr << " (pos. " << pos << ")\n";
}





/*
    PONTOS A MELHORAR:

    - [ ] Reduzir a densidade e tornar mais legível;
    - [ ] Implementar um mecanismo mais robusto de tratamento de erros;
    - [x] Utilizar uma estrutura de dados mais eficiente para vetores e verificações de pertinência;
    - [ ] Refatorar trechos de código para reduzir redundância;
    - [x] Implementar um padrão e aumentar consistênca nas mensagens de log;
    - [ ] Aumentar clareza no processo de encerramento do buffer.
*/

std::vector<Token> tokenize(std::string& line)
{
    std::cout << "<! Iniciando tokenizador do comando.\n";

    // Vetor que armazenará os tokens gerados.
    std::vector<Token> tokens{};

    // Caracteres de pontuação permitidos.
    std::unordered_set<char> allowed_punctuation =
    {',', '.', '-', '_', '?', '!', ':', ';', '(', ')', '[', ']', '{', '}'};

    OpTokenType state = OpTokenType::UNKNOWN;   // Estado inicial da máquina de estados.
    std::string buffer = "";

    std::cout << "<! Iniciando laço principal da máquina de estados.\n";

    // Laço principal da máquina de estados.
    auto char_iterator = line.begin(); // Iterador que percorrerá a linha.
    while(char_iterator != line.end())
    {
        switch(state)
        {
            case OpTokenType::UNKNOWN:

                // Ignora espaços em branco.
                if(*char_iterator == ' '){ char_iterator++; break; }

                // Se o caractere for alfanumérico, muda para o estado IDENTIFIER.
                if(bisalnum(*char_iterator))
                {
                    state = OpTokenType::IDENTIFIER;
                    break;
                }

                // Se o caractere for `-`, muda para o estado FLAG.
                if(*char_iterator == '-')
                {
                    state = OpTokenType::FLAG;
                    break;
                }

                // Se o caractere for `"`, muda para o estado STRING.
                if(*char_iterator == '\"')
                {
                    char_iterator++;
                    state = OpTokenType::STRING;
                    break;
                }


                log_err(line, char_iterator, "Caractere ", *char_iterator, " não identificado.");
                char_iterator++;

            break;


            case OpTokenType::IDENTIFIER:

                // Acumula os caracteres alfanuméricos no buffer.
                while(char_iterator != line.end() && bisalnum(*char_iterator))
                {
                    buffer += *char_iterator;
                    char_iterator++;
                }

                // Adiciona o token do tipo IDENTIFIER ao vetor de tokens.
                tokens.emplace_back(buffer, state, col(line, char_iterator));
                to_initial(buffer, state); // Retorna ao estado inicial.
                
            break;
            

            case OpTokenType::FLAG:

                // Acumula caractere alfanumérico no buffer.
                if(bisalnum(*char_iterator))
                {
                    buffer += *char_iterator;
                    char_iterator++;
                }

                // Se o caractere for um '-', valida se a flag é do tipo `-f` ou `--flag`.
                else if(*char_iterator == '-')
                {    
                    // Caso o buffer já tenha mais de dois caracteres, o terceiro `-` demarca ou o início de uma nova flag ou um erro. 
                    if(buffer.size() > 2)
                    {
                        if(buffer == "--")
                        {
                            log_err(line, char_iterator, "Houve uma tentativa falha de adicionar uma flag sem identificador (-f ou --flag).");
                            // std::cerr << "<# Flag inválida. (" << col(char_iterator) << ")\n";
                            char_iterator++;
                        }

                        else
                        {
                            tokens.emplace_back(buffer, state, col(line, char_iterator));
                            to_initial(buffer, state);
                        }

                    }

                    // Iterador apontando para a posição inicial do iterador principal. 
                    auto start = char_iterator;

                    // Atravessa os caracteres `-`, atualizando a posição do iterador principal.
                    while(char_iterator != line.end() && *char_iterator == '-') char_iterator++;

                    // Calcula o número de ocorrências do caractere `-` baseado na diferença de posição dos iteradores.
                    const int occurrences = std::distance(start, char_iterator);

                    if(occurrences == 1) buffer += "-";         // Declara flag do tipo `-f`
                    else if(occurrences == 2) buffer += "--";   // Declara flag do tipo `--flag`
                    
                    // Caso hajam mais de dois caracteres de `-` no trecho.
                    else
                    {
                        log_err(line, char_iterator, "Houve uma tentativa falha de adicionar uma flag sem identificador (-f ou --flag).");
                        to_initial(buffer, state);
                        char_iterator = start + 1;
                    }

                }

                // Adiciona o token do tipo FLAG ao vetor de tokens.
                else
                {
                    tokens.emplace_back(buffer, state, col(line, char_iterator));
                    to_initial(buffer, state); // Retorna ao estado original.
                }

            break;


            case OpTokenType::STRING:

                auto start = char_iterator; // Iterador para a posição inicial do vetor.

                // Acumula elementos de string no buffer.
                while(char_iterator != line.end() && *char_iterator != '\"')
                {
                    buffer += *char_iterator;
                    char_iterator++;
                }

                // Booleano para checar se o iterador chegou ao fim sem fechar a string.
                bool is_end = (char_iterator == line.end());

                if(is_end)
                {
                    log_err(line, char_iterator, "Há um elemento `\"` inválido no comando que será ignorado.");
                    
                    char_iterator = start+1; // Salta uma posição para analisar o próximo elemento.
                    to_initial(buffer, state);
                    break; // Termina iteração.
                }

                // Dispara uma mensagem de erro caso o buffer (sem espaços) esteja vazio.
                else if(trim(buffer).empty())
                {
                    log_err(line, char_iterator, "A string vazia será ignorada.");
                }

                else
                {
                    tokens.emplace_back(buffer, state, col(line, char_iterator));
                }

                char_iterator++;
                to_initial(buffer, state);

            break;
        }
    }


    // Ignorar por enquanto.
    if(!buffer.empty())
    {
        // Caso ainda haja algo no buffer após o fim do loop.
        tokens.emplace_back(buffer, OpTokenType::UNKNOWN, std::distance(line.begin(), char_iterator-1));
    }

    std::cout << "<! A função do tokenizador foi concluída com sucesso.\n";
    return tokens;
}