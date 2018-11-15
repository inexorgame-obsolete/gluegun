// automatically generated by the FlatBuffers compiler, do not modify

/**
 * @const
 * @namespace
 */
var distributed_state = distributed_state || {};

/**
 * @constructor
 */
distributed_state.Address = function() {
  /**
   * @type {flatbuffers.ByteBuffer}
   */
  this.bb = null;

  /**
   * @type {number}
   */
  this.bb_pos = 0;
};

/**
 * @param {number} i
 * @param {flatbuffers.ByteBuffer} bb
 * @returns {distributed_state.Address}
 */
distributed_state.Address.prototype.__init = function(i, bb) {
  this.bb_pos = i;
  this.bb = bb;
  return this;
};

/**
 * @param {flatbuffers.ByteBuffer} bb
 * @param {distributed_state.Address=} obj
 * @returns {distributed_state.Address}
 */
distributed_state.Address.getRootAsAddress = function(bb, obj) {
  return (obj || new distributed_state.Address).__init(bb.readInt32(bb.position()) + bb.position(), bb);
};

/**
 * @returns {number}
 */
distributed_state.Address.prototype.creatorId = function() {
  var offset = this.bb.__offset(this.bb_pos, 4);
  return offset ? this.bb.readInt16(this.bb_pos + offset) : 0;
};

/**
 * @param {number} value
 * @returns {boolean}
 */
distributed_state.Address.prototype.mutate_creator_id = function(value) {
  var offset = this.bb.__offset(this.bb_pos, 4);

  if (offset === 0) {
    return false;
  }

  this.bb.writeInt16(this.bb_pos + offset, value);
  return true;
};

/**
 * @returns {number}
 */
distributed_state.Address.prototype.objectId = function() {
  var offset = this.bb.__offset(this.bb_pos, 6);
  return offset ? this.bb.readInt16(this.bb_pos + offset) : 0;
};

/**
 * @param {number} value
 * @returns {boolean}
 */
distributed_state.Address.prototype.mutate_object_id = function(value) {
  var offset = this.bb.__offset(this.bb_pos, 6);

  if (offset === 0) {
    return false;
  }

  this.bb.writeInt16(this.bb_pos + offset, value);
  return true;
};

/**
 * @param {flatbuffers.Builder} builder
 */
distributed_state.Address.startAddress = function(builder) {
  builder.startObject(2);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} creatorId
 */
distributed_state.Address.addCreatorId = function(builder, creatorId) {
  builder.addFieldInt16(0, creatorId, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} objectId
 */
distributed_state.Address.addObjectId = function(builder, objectId) {
  builder.addFieldInt16(1, objectId, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @returns {flatbuffers.Offset}
 */
distributed_state.Address.endAddress = function(builder) {
  var offset = builder.endObject();
  return offset;
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} offset
 */
distributed_state.Address.finishAddressBuffer = function(builder, offset) {
  builder.finish(offset);
};

// Exports for ECMAScript6 Modules
export {distributed_state};
