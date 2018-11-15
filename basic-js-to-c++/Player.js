
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
}
/// Attention: JS is async. any notes to take for the syncing procedure?
