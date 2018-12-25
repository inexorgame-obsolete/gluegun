#!/usr/bin/env node

const bunyan = require('bunyan');
const flatbuffers = require('flatbuffers');
const nano = require('nanomsg');

const LogMessage = require('./logging_plugin_interface_generated.js').inexorgame.logging.Message;

let log = bunyan.createLogger({name: '@inexorgame/logger'});

let gluegunSocket, loggerSocket;
try {
    gluegunSocket = nano.socket('bus');
    loggerSocket = nano.socket('bus');
    gluegunSocket.connect('icp://tmp/registry_endpoint.ipc');
    loggerSocket.bind('icp://tmp/logger_endpoint.ipc');
} catch (err) {
    log.error('Something went wrong while opening sockets!');
}

log.info('Starting logger plugin!');

let exitCondition = false;
function main() {

    if (!exitCondition) {
        setTimeout(main, 250);
    }
}

main();
