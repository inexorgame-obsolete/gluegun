const flatbuffers = require("./flatbuffers-master/js/flatbuffers").flatbuffers;
const PlayerModule = require("./PlayerBuffer_generated").PlayerModule;

const nano = require('nanomsg');
const spawn = require('child_process').spawn;

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

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function get_player1s_initial_state_patch() {
    let player1 = new Player();
    player1.name = "player1's Initial State";
    player1.kills = 2;
    return player1.create_patch_from_dirty();
}

function test_serialisation() {
    const buf = get_player1s_initial_state_patch();

    let player2 = new Player();

    player2.update_from_patch(buf);

    console.log(`player2 name: ${player2.name}, kills = ${player2.kills}`);
}

function CoreLauncher() {
    console.log("FLEX: Launching core!");

    const args = [addr];
    const executable_path = "cmake-build-debug/basic_js_to_c__";

    this.core_process = spawn(executable_path, args);

    // set all kinds of callbacks
    this.core_process.stdout.on('data', (data) => {
        console.log(`FLEX: stdout from core! ${data}`);
    });
    this.core_process.stderr.on('data', (data) => {
        console.log(`FLEX: stderr from core! : ${data}`);
    });
    this.core_process.on('error', (err) => {
        console.log("FLEX: error!");
    });
    this.core_process.on('exit', (code, signal) => {
        console.log(`FLEX: core process exited with code ${code} from signal ${signal}`);
    });
    this.core_process.on('close', (code) => {
        console.log(`FLEX: core process closed with code ${code}`);
    });

}

// -------------

// create a nanomsg endpoint ("server", but not serving anything.
// Just the one who created the endpoint first).

let connection = nano.socket('pair');
connection.rcvtimeo(500);
const addr = 'tcp://127.0.0.1:65000'; // 'ipc:///synchrotest.ipc';
connection.bind(addr);
console.log(`FLEX: Created an endpoint on ${addr}`);

// on a new connection / i.e. new data with size 1, value 1:
//    -> send player1's initial state
//    -> set connection_established = true;
// on new data:
//      patch player1
//      console.log the new state
//      disconnect
//      exit
// 2. wait up to 20s for receiving a player name change

let core_connected = false;

connection.on('data', function (buf) {
    if (!core_connected) {
        console.log(`FLEX: New Connection. Received data: "${buf}"\n
                    Sending the initial player state`);
        const initial_state_patch = get_player1s_initial_state_patch();
        connection.send(initial_state_patch);
        core_connected = true;
    } else {
        console.log(`FLEX: New Patch Received.`);

        let player2 = new Player();
        player2.update_from_patch(buf);
        console.log(`player new name: ${player2.name}, kills = ${player2.kills}`);

        connection.close();
    }
});

// spawn c++ side
let core = new CoreLauncher();
