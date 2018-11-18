#pragma once

#include <string>
#include "PlayerBuffer_generated.h"


/// The state of a Player
struct PlayerData {
private:
    int ID;
    std::string name;
    bool name_field_is_dirty = true;
    int kills;
    bool kills_field_is_dirty = true;

    // used for serialization
    flatbuffers::FlatBufferBuilder builder;
public:
    std::string get_name() { return name; }
    void set_name(std::string new_name);
    int get_kills() { return kills; }
    void set_kills(int new_killcount);

    /// Patch a Player with a changeset in a Buffer-object.
    void update_from_patch(uint8_t *raw_buffer, size_t len);

    /// Create a Buffer which contains everything which changed before this function was called the last time.
    uint8_t * create_patch_from_dirty(size_t &len);
};

void testplayer();
