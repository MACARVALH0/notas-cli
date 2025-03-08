#include "utils.hpp"

////////// DATA //////////

using flagArgVariant = std::variant<std::string, bool>;
using flag_map = std::map<std::string, Flag>;

/**
 *  @brief Estrutura de dados de uma Flag.
 * Armazena um `name` e um `arg`,
 * que representam, respectivamente, a qualidade de alguma configuração e seu argumento.
 * 
 *  @example `Flag flag("SHORT", "Texto de escrita/reescrita curta.")`
 */
struct Flag
{
    std::string name;
    flagArgVariant arg;

    Flag() = default;
    Flag(std::string n, flagArgVariant a) : name(n), arg(a) {}
};


/** 
 *  @brief Uma abstração que representa as regras de detecção de flags.
 */
struct FlagRule
{
    std::unordered_set<std::string> group;
    std::string value;
    std::string config_name;
    std::optional<bool> needs_argument;
};


/**
 *  @brief Uma classe que armazena um mapa contendo o conjunto de flags recuperadas na linha de comando,
 * conectadas na relação chave-valor a uma string que indica sua função semântica.
 */
class FlagSet
{
    flag_map map{};
    
    void setSize(int size)
    {
        // throw std::out_of_range("Houve uma tentativa de atualizar o tipo de tamanho da entrada com um valor inválido.");
    }


    public:

        // Construtor padrão da classe.
        FlagSet() = default;

        flag_map retrieve(){ return map; }

        void setFlag(const Flag& flag, const std::string& config_name)
        {
            if(map.find(config_name) != map.end()) throw std::runtime_error("<# Múltiplas flags de definição do modo de escrita/reescrita da entrada.\n");
            else map[config_name] = flag;
        }

};


////////// FUNCTIONS //////////

/**
 *  @brief      Retorna um elemento (bool/string) para ocupar o espaço de "argumento" da flag.
 *  @param rule Elemento `FlagRule` cuja regra bateu com a busca.
 *  @param it   Iterador apontando para o elemento, da lista de tokens, que combina com a regra encontrada.
 *  @param end  Iterador que aponta para o final da lista de tokens.
 *  @return     `bool` ou `std::string` correspondente ao atributo de "argumento" da flag.
 */
flagArgVariant getFlagArgument (const FlagRule& rule, const auto& it, const auto& end)
{
    // Iterador apontando para o próximo item.
    auto next_it = it+1 != end ? it+1 : it;

    /*
        Se `rule.needs_argument == std::nullopt`, então o argumento é opcional.
        Logo, segue-se a análise de preenchimento opcional, que retorna:
        a. O token seguinte, caso o próximo item seja do tipo STRING, ou
        b. Uma string vazia, caso a condição anterior não seja atendida.
    */
    if(rule.needs_argument == std::nullopt){ return (next_it != end && next_it->type == OpTokenType::STRING) ? next_it->content : ""; }

    // Caso a regra não exija argumento, retorna uma string vazia.
    if(!rule.needs_argument){ return std::string(""); }

    // FIXME Seria mais adequado que a função retornasse um erro caso a flag necessitasse de um argumento e não o encontrasse.
    if(rule.needs_argument)
    {
        if(next_it != end && next_it->type == OpTokenType::STRING)
        { return next_it->content; } // Retorna std::string

        else true; // Retorna booleano
    }
};


flag_map getFlags(const token_list& tokens)
{
    // Define o objeto de set de flags.
    FlagSet set;

    // Definição da minha estrutura de regras.
    std::vector<FlagRule> rules =
    {
        { { "-s", "--short" },  "SHORT",    "SIZE",    true },
        { { "-l", "--long"  },  "LONG",     "SIZE" }
    };

    auto token_it = tokens.begin(); // Iterador que percorrerá o vetor.
    const auto end = tokens.end();  // Cache do iterador marcando o final do vetor.

    // Loop sobre todods os elementos da lista de tokens.
    while(token_it != end)
    {
        if(token_it->type != OpTokenType::FLAG) token_it++; 

        else
        {
            for(const FlagRule& rule : rules)
            {
                // Caso o conteúdo do token esteja incluso em algum grupo de `rules`.
                if(rule.group.find(token_it->content) != rule.group.end())
                {
                    // Armazena o atributo `arg` da flag. 
                    flagArgVariant flag_argument = getFlagArgument(rule, token_it, end);

                    // Declara a flag baseado na regra testada.
                    const Flag flag(rule.value, flag_argument);

                    // Adiciona a flag atual ao conjunto de FlagSet.
                    set.setFlag(flag, rule.config_name);
                    
                    // Retorna os valores armazenados no set de flags.
                    return set.retrieve();
                }

                else
                {
                    ErrorMsg err;
                    err << "A flag `" << token_it->content << "` é inválida ou não existe neste contexto.\n";
                    std::cerr << err.get();
                    // throw std::runtime_error(err.get()); // Off For debug
                }
            }
        }
    }
}