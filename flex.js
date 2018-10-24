#!/usr/bin/env node

const nano = require('nanomsg');
const process = require('process');
const spawn = require('child_process').spawn;

var pub = nano.socket('pub');
var sub = nano.socket('sub');

// create the publisher

console.log("Creating Flex Publisher!");
const addr = 'tcp://127.0.0.1:65000'; // 'ipc:///tmp/foo.ipc';
pub.bind(addr);


// connect from in here
/*
sub.connect(addr);

sub.on('data', function (buf) {
  console.log(String(buf));
  pub.close();
  sub.close();
});
*/

// execute core
console.log("Starting up core!");

const args = [addr];
const executable_path = "build/core";

let options = {
    //cwd: executable_folder,
    env: process.env
};
const core_process = spawn(executable_path, args, options);
core_process.stdout.on('data', (data) => {
  console.log(`stdout: ${data}`);
});
core_process.stderr.on('data', (data) => {
  console.log(`stderr: ${data}`);
});
core_process.on('error', (err) => {
    console.log("error!");
});
core_process.on('exit', (code, signal) => {
    console.log("exit!");
});


console.log("Sending Hello from flex");

// send hello world to the subscribers
setTimeout(function () {
  pub.send("Hello from nanomsg!");
}, 100);
console.log("Sent from flex");

