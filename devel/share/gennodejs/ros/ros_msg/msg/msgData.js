// Auto-generated. Do not edit!

// (in-package ros_msg.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class msgData {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.Join_data = null;
      this.Join_state = null;
      this.data = null;
    }
    else {
      if (initObj.hasOwnProperty('Join_data')) {
        this.Join_data = initObj.Join_data
      }
      else {
        this.Join_data = new Array(6).fill(0);
      }
      if (initObj.hasOwnProperty('Join_state')) {
        this.Join_state = initObj.Join_state
      }
      else {
        this.Join_state = new Array(6).fill(0);
      }
      if (initObj.hasOwnProperty('data')) {
        this.data = initObj.data
      }
      else {
        this.data = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type msgData
    // Check that the constant length array field [Join_data] has the right length
    if (obj.Join_data.length !== 6) {
      throw new Error('Unable to serialize array field Join_data - length must be 6')
    }
    // Serialize message field [Join_data]
    bufferOffset = _arraySerializer.float64(obj.Join_data, buffer, bufferOffset, 6);
    // Check that the constant length array field [Join_state] has the right length
    if (obj.Join_state.length !== 6) {
      throw new Error('Unable to serialize array field Join_state - length must be 6')
    }
    // Serialize message field [Join_state]
    bufferOffset = _arraySerializer.float64(obj.Join_state, buffer, bufferOffset, 6);
    // Serialize message field [data]
    bufferOffset = _serializer.uint32(obj.data, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type msgData
    let len;
    let data = new msgData(null);
    // Deserialize message field [Join_data]
    data.Join_data = _arrayDeserializer.float64(buffer, bufferOffset, 6)
    // Deserialize message field [Join_state]
    data.Join_state = _arrayDeserializer.float64(buffer, bufferOffset, 6)
    // Deserialize message field [data]
    data.data = _deserializer.uint32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 100;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ros_msg/msgData';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '73f187edf478e820d25171010c16a164';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64[6] Join_data
    float64[6] Join_state
    uint32 data
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new msgData(null);
    if (msg.Join_data !== undefined) {
      resolved.Join_data = msg.Join_data;
    }
    else {
      resolved.Join_data = new Array(6).fill(0)
    }

    if (msg.Join_state !== undefined) {
      resolved.Join_state = msg.Join_state;
    }
    else {
      resolved.Join_state = new Array(6).fill(0)
    }

    if (msg.data !== undefined) {
      resolved.data = msg.data;
    }
    else {
      resolved.data = 0
    }

    return resolved;
    }
};

module.exports = msgData;
