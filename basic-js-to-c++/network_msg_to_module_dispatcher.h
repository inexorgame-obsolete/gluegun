#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "DistributedStateAddress_generated.h"


/// Acts as a router for network messages to the module which owns the objects for that message.
/// Therefore a module needs to register the addresses of these objects.
class NetworkMsgToModuleDispatcher {

    std::unordered_map<const distributed_state::Address,
                       std::function<void(const distributed_state::Address &,
                                          const uint8_t *const buffer,
                                          size_t len)>> address_to_module_map;
public:

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


