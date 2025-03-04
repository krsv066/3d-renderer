#include "application.h"
#include <iostream>

int main() {
    try {
        Application app;
        app.Run();
    } catch (...) {
        std::cerr << "An unexpected error occurred" << std::endl;
        return -1;
    }

    return 0;
}
