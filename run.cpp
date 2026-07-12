#include <cstdlib>

int main() {
    std::system("clang++ main.cpp -o CodeNC -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo");
    std::system("./CodeNC");
}