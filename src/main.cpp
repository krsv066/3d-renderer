#include "application.h"
#include "except.h"

int main() {
    try {
        renderer::Application app;
        app.Run();
    } catch (...) {
        renderer::React();
    }

    return 0;
}
