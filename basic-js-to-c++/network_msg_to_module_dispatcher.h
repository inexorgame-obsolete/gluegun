#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "DistributedStateAddress_generated.h"

/*
/// Acts as a router for network messages to the module which owns the objects for that message.
/// Therefore a module needs to register the addresses of these objects.
class NetworkMsgToModuleDispatcher {

    std::unordered_map<const distributed_state::Address,
                       std::function<void(const distributed_state::Address &,
                                          const uint8_t *const buffer,
                                          size_t len)>> address_to_module_map;
    uint16_t object_id_counter = 1; // replaceable with modulemap.size() probably

public:

    /// Receive a new distributed state address, to register an object created on this endpoint.
    distributed_state::Address get_new_distributed_state_address(bool created_locally = true) {
        constexpr uint16_t own_network_id = 0; // 0 is core, 1 is flex for now.
        constexpr uint16_t remote_network_id = 1; // 0 is core, 1 is flex for now.

        flatbuffers::FlatBufferBuilder builder;
        auto addressbuffer = distributed_state::CreateAddress(builder,
                                                              own_network_id ? created_locally : remote_network_id,
                                                              object_id_counter++);
        builder.Finish(addressbuffer);
        uint8_t *buf = builder.GetBufferPointer();
        return *distributed_state::GetAddress(buf);
    }

    /// Register all of the given object addresses, to forward incoming messages to their corresponding module dispatcher.
    /// @param object_addresses: A vector of addresses corresponding to the module.
    /// @param module_readable_name: The name of the module to be printed on error.
    /// @param module_dispatcher_functor: The function to forward the message data to.
    void register_module_object_addresses(const std::vector<distributed_state::Address> &object_addresses,
                                          const std::function<void(const distributed_state::Address &,
                                                                   const uint8_t *const buffer,
                                                                   size_t len)> &module_dispatcher_functor);

    /// Stop forwarding those object ids: print an error instead.
    void unregister_module_object_addresses(const std::vector<distributed_state::Address> &object_addresses);

    /// Read the first part of the message containing the object address to be changed, then execute the corresponding
    /// functor, associated with that object.
    /// @param buffer: a buffer containing two parts:
    ///                firstly the address flatbuffers object prefixed with its size,
    ///                secondly the informations to be passed to the module (probably a flatbuffers object as well).
    /// @param len: the length of the complete buffer.
    void dispatch(const uint8_t *buffer, size_t len);
};

*/
