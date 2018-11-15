#include "PlayerModule.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include "DistributedStateAddress_generated.h"
#include "PlayerModuleObjects_generated.h"


/// Acts as a router for network messages to the module which owns the objects for that message.
/// Therefore a module needs to register the addresses of these objects.
class PlayerModule {

    std::unordered_map<const distributed_state::Address,
            std::function<void(const distributed_state::Address &,
            const uint8_t *const buffer,
            size_t len)>> address_to_cpp_object_map;

    /// Divide the address space in chunks of one byte (256 objects per chunk)
    /// Each module can fill up its address space, until it runs out of space, in which case it asks the network message
    /// dispatcher to receive the next address space.

    /// Together the address space size is limited to 4 bytes (uint32_t).
    /// This means since the chunk size is set to 1 byte (uint8_t), that address_space_size = chunk_size * module_amount:
    /// -> 4 modules can be
    /// Worst case: 32 bits modules and one object in each ->
    uint32_t object_address_space;
    uint8_t counter;
public:

    explicit PlayerModule(uint32_t first_address_space) : object_address_space(first_address_space) {}

    // also move to network addressing service, see network module dispatcher.
    uint32_t get_unique_address_for_object() {

    }

    // TODO: no registration indirection necessary: pass function (containing memory addresses) directly
    //       (maybe even possible across plugin boundaries)
    /// Register all of the given object addresses, to forward incoming messages to their corresponding module dispatcher.
    /// @param object_addresses: A vector of addresses corresponding to the module.
    /// @param module_readable_name: The name of the module to be printed on error.
    /// @param module_dispatcher_functor: The function to forward the message data to.
    void register_module_object_addresses(const std::vector<distributed_state::Address> &object_addresses,
            const std::function<void(const distributed_state::Address &,
    const uint8_t *const buffer,
            size_t len)> &module_dispatcher_functor){

    }

    /// Stop forwarding those object ids: print an error instead.
    void unregister_module_object_addresses(const std::vector<distributed_state::Address> &object_addresses){

    }

    /// Receive all messages for this module and pass the individual changesets to the individual objects.
    /// All objects need to register themselves first.
    /// @param address: the object address associated with this change.
    /// @param buffer: a buffer containing the information to be passed to the module, which is one of 3 different
    ///                flatbuffer root_objects.
    /// @param len: the length of the complete buffer.
    void dispatch_associated_messages(const distributed_state::Address &address,
                                   const uint8_t *const buffer,
                                   size_t len) {
        // find out local object using the (senderid, objectid) tuple (???)

    }
};
