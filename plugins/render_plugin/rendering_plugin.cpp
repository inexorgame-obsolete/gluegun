#include <iostream>
#include <system_error>
#include <thread>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>
#include "Player.h"

#include <thread>

PlayerData player;

int main(int argc, char **argv) {
    try {
        if (argc < 2) {

            std::cerr << "CORE(core executed without listening path)" << std::endl;
            return 1;
        }

        nnxx::socket connection{nnxx::SP, nnxx::PAIR};
        const char *addr = argv[1];
        std::cout << "CORE( connection to Flex via " << addr << ")" << std::endl;

        // connect and send a ping event

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

        uint8_t buffery[200];
        for (int i = 0; i < 200; i++) buffery[i] = 0;
        int msg = connection.recv(reinterpret_cast<void *>(buffery), 199);
        // const void * const player_buffer = msg.data();
        // auto player_buffer_size = msg.size();
        std::cout << "CORE(Received initial player patch)\nApplying.." << std::endl;
        using namespace std::chrono_literals;
        // std::this_thread::sleep_for(20s);
        player.update_from_patch(buffery, 0); // player_buffer_size);

        std::cout << "CORE( player name: " << player.get_name() <<
                            "player kills: " << player.get_kills() << ")" << std::endl;
        // send a new patch
        player.set_name("Player1");
        // player.set_kills(3);

        size_t player_buffer_size = 0;
        const uint8_t *new_player_buffer = player.create_patch_from_dirty(player_buffer_size);
        connection.send((const void *)new_player_buffer, player_buffer_size);
        return 0;
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

