#include <cstdio>
#include <string>
#include "../lexical/SymbolTable.h"
class Generate {
    public: 
        Generate(SymbolTable* st);
        void cancelFile();
        void concludeWrite();
        void writeStringInFile(std::string command);
    private:
        std::string filename;
        FileOutputStream  fileStream;
        OutputStreamWriter streamWriter;
};

#endif