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
public:
    std::string get_name() { return name; }
    void set_name(std::string new_name);
    int get_kills() { return kills; }
    void set_kills(int new_killcount);

    /// Patch a Player with a changeset in a Buffer-object.
    void update_from_patch(MyGame::PlayerBuffer &buffer);

    /// Create a Buffer which contains everything which changed before this function was called the last time.
    MyGame::PlayerBuffer create_patch_from_dirty();
};

void testplayer();
