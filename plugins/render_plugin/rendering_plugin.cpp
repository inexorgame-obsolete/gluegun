#include <iostream>
#include <system_error>
#include <thread>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

#include "flatbuffers/flatbuffers.h"
#include "plugins/render_plugin/gluegun_generated.h"

#include <thread>
#include <tuple>

using namespace inexorgame::plugin;

class RegistryConnection {

    nnxx::socket connection;
    constexpr static char *addr = "ipc:///tmp/registry_endpoint.ipc";

    std::tuple<uint8_t *, size_t> build_registration_message(const char *plugin_name) {

        flatbuffers::FlatBufferBuilder builder;
        auto name = builder.CreateString(plugin_name);
        auto plugin_id = CreatePluginId(builder, name);
        auto full_msg = CreatePluginMessage(builder, LifeCycleSignals_RegisterMe, plugin_id);
        builder.Finish(full_msg);
        return std::make_tuple(builder.GetBufferPointer(), builder.GetSize());
    }
public:
    RegistryConnection() : connection(nnxx::SP, nnxx::PAIR) {
        std::cout << "rendering plugin connecting to registry endpoint on " << addr << std::endl;
        connection.connect(addr);
    }

    /// Send a flatbuffers message to the registry so it knows this plugin exists.
    /// @param own_name the name of this plugin.
    void register_self(const char *own_name) {
        auto msg_and_len = build_registration_message(own_name);
        send_msg(std::get<0>(msg_and_len), std::get<1>(msg_and_len));
    }

    /// Send a message to the plugin registry
    void send_msg(uint8_t *buffer, size_t buffer_len) {
        if (connection.send((void *)buffer, buffer_len) != buffer_len) {
            throw std::runtime_error("was not able to send message to registry");
        }
    }
};

int main(int argc, char **argv) {
    try {
        RegistryConnection registry_connection{};
        registry_connection.register_self("render_plugin");
        return 0;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

