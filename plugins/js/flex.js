#!/usr/bin/env node

const nano = require('nanomsg');
const process = require('process');
const spawn = require('child_process').spawn;

var pub = nano.socket('pub');
var sub = nano.socket('pair');
sub.rcvtimeo(500);

// create the publisher

const addr = 'tcp://127.0.0.1:65000'; // 'ipc:///synchrotest.ipc';

// console.log("FLEX: Creating Publisher!");
//pub.bind(addr);

// execute core
console.log("FLEX: Launching core!");

const args = [addr];
const executable_path = "build/core";

const core_process = spawn(executable_path, args);
core_process.stdout.on('data', (data) => {
    console.log(`FLEX: stdout from core! ${data}`);
});
core_process.stderr.on('data', (data) => {
    console.log(`FLEX: stderr from core! : ${data}`);
});
core_process.on('error', (err) => {
    console.log("FLEX: error!");
});
core_process.on('exit', (code, signal) => {
  console.log(`FLEX: core process exited with code ${code} from signal ${signal}`);
});
core_process.on('close', (code) => {
  console.log(`FLEX: core process closed with code ${code}`);
});

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function take_action() {
    console.log('FLEX: Delaying 0.5s for handshake...');

    await sleep(500);

    /*
    console.log("FLEX: Sending Hello from flex");
    // send hello world to the subscribers
    setTimeout(function () {
        pub.send("This is the hello world message!\0");
    }, 100);
    console.log("FLEX: Sent from flex");
    */

    // connect from in here
    console.log(`FLEX: Connecting to ${addr}`);

    sub.connect(addr);

    sub.on('data', function (buf) {
      console.log(`FLEX: Received data: "${String(buf)}"`);
      sub.close();
    });
}

take_action();


