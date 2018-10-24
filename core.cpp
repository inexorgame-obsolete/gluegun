#include <iostream>
#include <system_error>
#include <thread>
#include <nnxx/message.h>
//#include <nnxx/pair.h>
#include <nnxx/pubsub.h>
#include <nnxx/socket.h>

using namespace std::chrono_literals;

int main(int argc,  char** argv) {
  try {
    if (argc < 2) {

        std::cerr << "CORE(core executed without listening path)" << std::endl;
        return 1;
    }

    // nnxx::socket s1 { nnxx::SP, nnxx::PAIR };
    // nnxx::socket s2 { nnxx::SP, nnxx::PAIR };
    nnxx::socket s1 { nnxx::SP, nnxx::PUB };
    nnxx::socket s2 { nnxx::SP, nnxx::SUB };
    const char *addr = argv[1]; // "ipc:///tmp/foo.ipc";
    std::cout << "CORE( connection to Flex via " << addr << ")" << std::endl;

     s1.bind(addr);
    //s2.connect(addr);
    std::this_thread::sleep_for(5s);
    std::cout << "CORE(waited 5s)" << std::endl;

    if (s1.send("Hello World!") != 12) {
      std::cerr << "CORE(Was not able to send hello world)" << std::endl;
      return 1;
    }
    else {
      std::cout << "CORE(Sent hello world)" << std::endl;
    }
/*
    std::cout << "CORE(Waiting for Hello World! from flex)" << std::endl;

    nnxx::message msg = s2.recv();
    std::cout << "CORE(Received message: " << msg << ")" << std::endl;*/
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

