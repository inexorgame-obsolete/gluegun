#include <iostream>
#include <system_error>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

int main(int argc,  char** argv) {
  try {
    if (argc < 2) {

        std::cerr << "core executed without listening path" << std::endl;
        return 1;
    }

    // nnxx::socket s1 { nnxx::SP, nnxx::PAIR };
    nnxx::socket s2 { nnxx::SP, nnxx::PAIR };
    const char *addr = argv[1]; // "ipc:///tmp/foo.ipc";
    std::cout << "Core connection to Flex via " << addr << std::endl;

    // s1.bind(addr);
    s2.connect(addr);

    // s1.send("Hello World!");
    std::cout << "Waiting for Hello World! from flex" << std::endl;

    nnxx::message msg = s2.recv();
    std::cout << msg << std::endl;
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

