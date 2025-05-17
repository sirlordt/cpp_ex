#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++ Demo!" << std::endl;
    
    std::vector<std::string> args(argv, argv + argc);
    
    if (args.size() > 1) {
        std::cout << "Arguments:" << std::endl;
        for (size_t i = 1; i < args.size(); ++i) {
            std::cout << "  " << i << ": " << args[i] << std::endl;
        }
    }
    
    return 0;
}
