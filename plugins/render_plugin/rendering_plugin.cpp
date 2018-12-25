#include <iostream>
#include <system_error>
#include <thread>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

#include <thread>


int main(int argc, char **argv) {
    try {
        const char *addr = "ipc:///tmp/registry_endpoint.ipc";
        std::cout << "rendering plugin connecting to registry endpoint on " << addr << std::endl;

        // connect and send a ping event
        nnxx::socket connection{nnxx::SP, nnxx::PAIR};
        connection.connect(addr);
        if (connection.send("Hello World!") != 12) {
            std::cerr << "CORE(Was not able to send hello world)" << std::endl;
            return 1;
        } else {
            std::cout << "CORE(Sent hello world)" << std::endl;
        }

        return 0;
        // wait for player patch
        std::cout << "CORE(Waiting for initial player state from flex)" << std::endl;
        return 0;
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

