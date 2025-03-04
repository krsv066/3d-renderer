#include "application.h"
#include "except.h"

int main() {
    try {
        renderer::Application app;
        app.Run();
    } catch (...) {
        renderer::except::React();
    }

    return 0;
}
