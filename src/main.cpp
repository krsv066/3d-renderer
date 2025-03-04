#include "application.h"
#include "except.h"

int main() {
    try {
        Application app;
        app.Run();
    } catch (...) {
        except::React();
    }

    return 0;
}
