#include "Player.h"
#include <iostream>

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

void PlayerData::update_from_patch(uint8_t *raw_buffer, size_t len) {
    const auto buffer = PlayerModule::GetPlayerBuffer(raw_buffer);
    //if (PlayerModule::VerifyPlayerBufferBuffer(buffer))// bool Play:: // TODO: MYgame -> PlayerModule?
    std::cout << buffer->kills() << std::endl;
    if (flatbuffers::IsFieldPresent(buffer, buffer->VT_KILLS)) {
        kills = buffer->kills();
        kills_field_is_dirty = false;
    }
    if (flatbuffers::IsFieldPresent(buffer, buffer->VT_NAME)) {
        name = buffer->name()->str();
        name_field_is_dirty = false;
    }
}

uint8_t * PlayerData::create_patch_from_dirty(size_t &len) {

    // construct entries first..
    const auto name_field_or_null = name_field_is_dirty
                                    ?  builder.CreateString(name)
                                    : flatbuffers::Offset<flatbuffers::String>();

    // construct the table
    PlayerModule::PlayerBufferBuilder player_buffer_builder(builder);
    if (kills_field_is_dirty)
        player_buffer_builder.add_kills(kills);
    if (name_field_is_dirty)
        player_buffer_builder.add_name(name_field_or_null);
    auto playerbuffer = player_buffer_builder.Finish();
    builder.Finish(playerbuffer);

    // Reset the dirty flags
    kills_field_is_dirty = false;
    name_field_is_dirty = false;

    len = builder.GetSize();
    return builder.GetBufferPointer();
    // TODO: when does the builder get resetted?
}

void testplayer() {

    // Clone player1 into an empty player2
    PlayerData player1, player2;
    player1.set_name("Player1");
    player1.set_kills(3);

    size_t player_buffer_size = 0;
    uint8_t *player_buffer = player1.create_patch_from_dirty(player_buffer_size);
    player2.update_from_patch(player_buffer, player_buffer_size);

    assert(player1.get_name() == player2.get_name());
    assert(player1.get_kills() == player2.get_kills());

    // update player1 and send just the change
    PlayerData player3;

    player1.set_kills(3);
    player_buffer = player1.create_patch_from_dirty(player_buffer_size);
    player3.update_from_patch(player_buffer, player_buffer_size);
    assert(player3.get_name() == "");
    assert(player1.get_kills() == player3.get_kills());
}
