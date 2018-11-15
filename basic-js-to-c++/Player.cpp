#include "Player.h"


/// Set the name of the player and send the update to JS.
void PlayerData::set_name(std::string new_name) {
    name_field_is_dirty = true;
    name = new_name;
}

/// Set the amount of kills of the player and send the update to JS.
void PlayerData::set_kills(int new_killcount) {

    kills_field_is_dirty = true;
    kills = new_killcount;
}

void PlayerData::update_from_patch(MyGame::PlayerBuffer &buffer) {
    if (buffer.CheckField(buffer.VT_KILLS))
        kills = buffer.kills();
    if (buffer.CheckField(buffer.VT_NAME))
        name = buffer.name()->str();
}

MyGame::PlayerBuffer PlayerData::create_patch_from_dirty() {
    MyGame::PlayerBuffer d;
    flatbuffers::FlatBufferBuilder builder;
    MyGame::PlayerBufferBuilder player_buffer_builder(builder);
    if (kills_field_is_dirty)
        player_buffer_builder.add_kills(kills);
    if (name_field_is_dirty)
        player_buffer_builder.add_name(builder.CreateString(name));
    auto playerbuffer = player_buffer_builder.Finish();
    builder.Finish(playerbuffer);

    // Reset the dirty flags
    kills_field_is_dirty = false;
    name_field_is_dirty = false;

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    return d;
}

void testplayer() {

    PlayerData own_player;

    own_player
}
