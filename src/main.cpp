#include "application.h"
#include "except.h"

int main(int argc, char* argv[]) {
    try {
        renderer::Application app(argc, argv);
        app.Run();
    } catch (...) {
        renderer::React();
    }

    return 0;
}
