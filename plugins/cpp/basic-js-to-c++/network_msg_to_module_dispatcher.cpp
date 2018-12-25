#include "network_msg_to_module_dispatcher.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include "DistributedStateAddress_generated.h"
/*
void NetworkMsgToModuleDispatcher::register_module_object_addresses(
        const std::vector<distributed_state::Address> &object_addresses,
      // const char * const module_readable_name,
        const std::function<void(const distributed_state::Address &,
                                 const uint8_t *const buffer,
                                 size_t len)> &module_dispatcher_functor)
{
    for (const auto &object_address : object_addresses)
        address_to_module_map.emplace(object_address, module_dispatcher_functor);
}

void NetworkMsgToModuleDispatcher::unregister_module_object_addresses(
        const std::vector<distributed_state::Address> &object_addresses)
{
    for (const auto &object_address : object_addresses)
        address_to_module_map.erase(object_address);
}

void NetworkMsgToModuleDispatcher::dispatch(const uint8_t *buffer, size_t len)
{
    const auto &object_address = *distributed_state::GetSizePrefixedAddress(buffer);

    // the size in bytes
    const uint32_t size_object_address = flatbuffers::GetPrefixedSize(buffer);

    // ignore the address part of the message to retrieve the second part
    const uint8_t *module_information_buffer = buffer + size_object_address;
    const size_t module_information_size = len - size_object_address;
    try {
        // forward the objectid + changeset to the responsible party to apply the change
        const auto &functor = address_to_module_map.at(object_address);
        functor(object_address, module_information_buffer, module_information_size);
    }
    catch(std::out_of_range &e) {
        // TODO: log error here: no module available anymore.
    }
}*/
