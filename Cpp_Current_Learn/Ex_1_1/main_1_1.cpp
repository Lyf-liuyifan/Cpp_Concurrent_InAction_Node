#include <thread>
#include <iostream>

void hello() {
    std::cout << "Hello Concurrent World\n";
}

int main(void) {
    std::thread t(hello);
    t.join();
    return 0;
}