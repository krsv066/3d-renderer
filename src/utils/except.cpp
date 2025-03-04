#include "except.h"
#include <exception>
#include <iostream>

namespace renderer::except {
void React() {
    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception" << std::endl;
    }
}
}  // namespace renderer::except
