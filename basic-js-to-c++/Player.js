const flatbuffers = require("./flatbuffers-master/js/flatbuffers").flatbuffers;
const PlayerModule = require("./PlayerBuffer_generated").PlayerModule;

class Player {
    constructor() {
        this._name_value = "";
        this._kills_value = 0;
        this._kills_value_is_dirty = true;
        this._name_value_is_dirty = true;
    }
    get name() { return this._name_value; }

    set name(new_name) {
        this._name_value_is_dirty = true;
        this._name_value = new_name;
    }
    get kills() { return this._kills_value; }

    set kills(new_kill_count) {
        this._kills_value_is_dirty = true;
        this._kills_value = new_kill_count;
    }

    /// Encode all dirty fields into a PlayerBuffer and return it.
    create_patch_from_dirty() {
        const builder = new flatbuffers.Builder(0);

        // Create some weapons for our Monster ('Sword' and 'Axe').
        let name_str = null;
        if (this._name_value_is_dirty) {
            name_str = builder.createString(this._name_value);
        }
        PlayerModule.PlayerBuffer.startPlayerBuffer(builder);
        if (this._name_value_is_dirty)
            PlayerModule.PlayerBuffer.addName(builder, name_str);
        if (this._kills_value_is_dirty)
            PlayerModule.PlayerBuffer.addKills(builder, this._kills_value);
        let patch = PlayerModule.PlayerBuffer.endPlayerBuffer(builder);
        builder.finish(patch);

        this._kills_value_is_dirty = false;
        this._name_value_is_dirty = false;

        return builder.dataBuffer();
    }

    update_from_patch(buffer) {
        const patch = PlayerModule.PlayerBuffer.getRootAsPlayerBuffer(buffer);
        flatbuffers.hasOwnProperty();
        const default_value_kills = 0;
        if (patch.kills() !== default_value_kills) {
            this._kills_value = patch.kills();
            this._kills_value_is_dirty = false; // TODO: this overrides recent changes.
        }
        if (patch.name() !== null) {
            this._name_value = patch.name();
            this._name_value_is_dirty = false;
        }
    }
}
// Attention: JS is async. any notes to take for the syncing procedure?

function test_serialisation() {
    let player1 = new Player();
    player1.name = "player1Name";
    player1.kills = 2;

    const buf = player1.create_patch_from_dirty();

    let player2 = new Player();

    player2.update_from_patch(buf);

    console.log(`player2 name: ${player2.name}, kills = ${player2.kills}`);
}

test_serialisation();
