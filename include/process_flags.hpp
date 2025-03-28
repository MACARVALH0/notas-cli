#ifndef PROCESS_FLAGS_HPP
#define PROCESS_FLAGS_HPP

#include "global_structures.hpp"

enum class Configuration
{
    SIZE
};

enum class FlagValue
{
    SHORT,
    LONG
};

using flag_arg = std::variant<std::string, bool>; // Um variant dos tipos `string` e `bool`.

/**
 *  @brief Mapa do setup de flags.
 * 
 *  @param `Configuration`: Nome do tipo de configuração (como Configuration::SIZE).
 *  @param `Flag`: Um elemento Flag com um valor do tipo FlagValue e um argumento do tipo string.
 */


using flag_config = std::unordered_map<Configuration, FlagValue>; // Mapa de configuração de flags ativas.


/**
 *  @brief Estrutura de dados de uma Flag.
 * Armazena um `value` e um `arg`,
 * que representam, respectivamente, a qualidade de alguma configuração e seu argumento.
 * 
 *  @example `Flag flag("SHORT", "Texto de escrita/reescrita curta.")`
 */
struct Flag
{
    FlagValue value;
    flag_arg arg;

    Flag() = default;
    Flag(FlagValue n, flag_arg a) : value(n), arg(a) {}
};

using flag_setup_map = std::map<Configuration, Flag>;


/** 
 *  @brief Uma abstração que representa as regras de detecção de flags.
 */
struct FlagRule
{
    std::unordered_set<std::string> group;
    FlagValue value;
    Configuration config_name;
    std::optional<bool> needs_argument;
};


/**
 *  @brief Uma classe que armazena um mapa contendo o conjunto de flags recuperadas na linha de comando,
 * conectadas na relação chave-valor a uma string que indica sua função semântica.
 */
class FlagSet
{
    flag_setup_map map{};
    
    void setSize(int size)
    {
        // throw std::out_of_range("Houve uma tentativa de atualizar o tipo de tamanho da entrada com um valor inválido.");
    }


    public:

        // Construtor padrão da classe.
        FlagSet() = default;

        flag_setup_map retrieve(){ return map; }

        void setFlag(const Flag& flag, Configuration config_name)
        {
            if(map.find(config_name) != map.end()) throw std::runtime_error("<# Múltiplas flags de definição do modo de escrita/reescrita da entrada.\n");
            else map[config_name] = flag;
        }

};

/*
    TODO
    Transformar a abstração `std::vector<ContextConfiguration>` de operation_functions
    em um std::map<Configuration, CtxConfig>
*/
struct CtxConfig
{
    bool obligatory;
    bool exists;
    Flag flag;

    CtxConfig(bool obligatory) : obligatory(obligatory) {}
};


struct ContextConfiguration
{
    Configuration configuration;
    bool obligatory;
    bool exists;
    Flag flag;

    ContextConfiguration(Configuration c, bool obligatory)
    : configuration(c), obligatory(obligatory) {}
};



/**
 *  @brief          Retorna um mapa com flags.
 *  @param tokens   Lista de tokens da linha de comando.  
 *  @return         Um mapa std::map<std::string, Flag> indicando as definições de flags encontradas.
 */
flag_setup_map getFlagSetup(std::vector<Token>& tokens);


std::string toString_Configuration(Configuration config);


#endif