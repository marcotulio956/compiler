#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SemanticAnalysis
{
    // tem que fazer pra unicidade tb

    
    public:
    static std::string verifyType(std::string tipo1, std::string tipo2)
    {
        if ((tipo1.compare(tipo2) != 0))
        {
            return tipo1;
        }
        else if ((tipo1.compare("tipo_erro-nao-declarado") != 0) || (tipo2.compare("tipo_erro-nao-declarado") != 0))
        {
            return "tipo_erro-nao-declarado";
        }
        else 
        {
            return "tipo_erro-tipo-invalido";
        }
    }
    static std::string verifyUnity(std::SymbolTable tabelaDeSimbolos, std:: Lexeme palavra)
    {
        if (tabelaDeSimbolos.get(palavra.getLexeme()) == null){
            return "nao-declarado";
        }
        else{
            return "declarado";
        }
    }
};