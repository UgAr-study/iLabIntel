#include "include/MainHeader.h"
#include "include/LexemHeader.h"
#include "include/ParserHeader.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Too few arguments: expected filepath" << std::endl;
        return 0;
    }

    std::vector<char> text = ReadFile(argv[1]);
    if (text.empty()) {
        std::cout << "Nothing to execute" << std::endl;
        return 0;
    }

    std::vector<Node*> lexems = Lexer(text);
    Parser(lexems.begin(), lexems.end(), nullptr);
    return 0;
}
