#pragma once

#include <cstddef>
#include <stdint.h>

class network_nanomsg {

    /// Pull data from the network interface.
    /// @param data: a char buffer to be filled with the received data.
    /// @param size: will be set with the length of the data afterwards in Bytes.
    void pull_blocking(const uint8_t *data, size_t &size);

    /// Push a bunch of data to the network interface.
    /// @param data the data to be sent.
    /// @param size the size of the data array.
    void push_blocking(const uint8_t *data, size_t size);
};


