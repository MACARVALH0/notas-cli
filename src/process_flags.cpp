#include "utils.hpp"

////////// DATA //////////

using flagArgVariant = std::variant<std::string, bool>;

struct Flag
{
    std::string name;
    flagArgVariant arg;

    Flag() = default;
    Flag(std::string n, std::string a) : name(n), arg(a) {}
    Flag(std::string n, bool a) : name(n), arg(a) {}
};


struct FlagRule
{
    std::unordered_set<std::string> group;
    std::string value;
    std::string config_name;
    std::optional<bool> needs_argument;
};



class FlagSet
{
    std::map<std::string, Flag> map{};
    
    void setSize(int size)
    {
        // throw std::out_of_range("Houve uma tentativa de atualizar o tipo de tamanho da entrada com um valor inválido.");
    }


    public:

        // Construtor padrão da classe.
        FlagSet() = default;

        void setFlag(const Flag& flag, const std::string& config_name)
        {
            if(map.find(config_name) != map.end()) throw std::runtime_error("<# Múltiplas flags de definção do modo de escrita/reescrita da entrada.\n");
            else map[config_name] = flag;
        }

};






////////// FUNCTIONS //////////

/**
 *  @brief Retorna um elemento (bool/string) para ocupar o espaço de "argumento" da flag.
 *  @return `bool` ou `std::string` correspondente ao atributo de "argumento" da flag.
 */
flagArgVariant getFlagArgument (const FlagRule& rule, auto& next_it, auto& end)
{
    if(rule.needs_argument == std::nullopt){ return (next_it != end && next_it->type == OpTokenType::STRING) ? next_it->content : ""; }

    if(!rule.needs_argument){ return std::string(""); }

    if(rule.needs_argument)
    {
        if(next_it != end && next_it->type == OpTokenType::STRING)
        { return next_it->content; }

        else true;
    }
};


FlagSet getFlags(const token_list& tokens)
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
                    // TODO Atualmente estou aqui!
                    // Iterador apontando para o próximo item.
                    auto next_it = token_it+1 != end ? token_it+1 : token_it;

                    flagArgVariant flag_argument = getFlagArgument(rule, next_it, end);

                    /*
                        TODO:
                        
                        - Entender como exatamente se utiliza `std::visit` e confirmar se vale à pena seguir com essa abordagem.
                        - Criar Flag com nome e argumento (Questão do momento, lidar com tipo bool ou string);
                        - Executar set.setFlags para adicionar Flag criada ao map de flags. 
                    */
                }

                else 
                {
                    ErrorMsg err;
                    err << "A flag `" << token_it->content << "` é inválida ou não existe neste contexto.\n";
                    throw std::runtime_error(err.get());
                }
            }
        }
    }










    // // Organizando e definindo flags.
    // while(token_it != end)
    // {
    //     if(token_it->type != OpTokenType::FLAG) token_it++; 

    //     // Hardcoded, só um placeholder pra organizar as ideias.
    //     else
    //     {
    //         if(token_it->content == "-s" || token_it->content == "--short")
    //         {

    //             if(++token_it == end || (token_it+1)->type != OpTokenType::STRING) throw std::runtime_error("Falta argumentos para a criação da entrada.\nFormato: <-s/--short> [Texto da entrada].");
            
    //         }

    //     }
    // }
}