# modular-engine
A modular approach for a new game engine using a dependency injection framework google fruit.

# How to Run

Use
`conan export googlefruit inexorgame/stable`
to create a google fruit package.

Then use
`conan export lmdb inexorgame/testing`
to create a LMDB package.

Then use
`conan export lmdbxx inexorgame/testing`
to create a LMDB++ (the C++ wrapper around LMDB) package.

Afterwards use the new experimental conan feature [workspaces](https://docs.conan.io/en/latest/developing_packages/workspaces.html):a solution to work on different conan packages in the same *workspace*.

Therefore resolve all dependencies (in the workspace)

`conan install . -if=build --build missing`

Afterwards build it by changing into the build directory and executing cmake.


`cd build && cmake .. && cmake --build .`

Then execute `app` with

`cd app`

`./app`


*You should see `Hello World` in the console here*