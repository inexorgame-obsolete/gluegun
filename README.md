# modular-synchro
A test to sync a set of variables between flex (node.js) and core (C++) and a website (JS).

# How to Run Core

Use
`conan export dependencies/nanomsg inexorgame/testing`
and
`conan export dependencies/nanomsgxx inexorgame/testing`
to create the nanomsg c++ conan package.

Then build all dependencies (i.e. nanomsgxx) with

`conan install . -if=build --build missing`
.

Afterwards build it by changing into the build directory and executing cmake:

`cd build && cmake .. && cmake --build .`

Then execute `core` with

`./app`


*You should see `Hello World!` in the console here*

# How to run Flex

Install the [yarn package manager](https://yarnpkg.com/en/docs/install).

Afterwards do

`yarn add .`
to install the dependency node-nanomsg

and use

`yarn run flex` to execute flex.

