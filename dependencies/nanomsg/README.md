[![Build status](https://ci.appveyor.com/api/projects/status/gdv7opmg0r2esn80/branch/master?svg=true)](https://ci.appveyor.com/project/Wi3ard/conan-nanomsg/branch/master)
[![Build Status](https://travis-ci.org/Wi3ard/conan-nanomsg.svg?branch=master)](https://travis-ci.org/Wi3ard/conan-nanomsg)

# conan-nanomsg
[Conan.io](https://conan.io) package for [nanomsg](https://github.com/nanomsg/nanomsg) library

The packages generated with this **conanfile** can be found in [conan.io](https://www.conan.io/source/nanomsg/1.0.0/Wi3ard/stable).

## Build packages

Download conan client from [Conan.io](https://conan.io) and run:

    $ python build.py

## Upload packages to server

    $ conan upload nanomsg/1.0.0@Wi3ard/stable --all

## Reuse the packages

### Basic setup

    $ conan install nanomsg/1.0.0@Wi3ard/stable
    
### Project setup

If you handle multiple dependencies in your project is better to add a *conanfile.txt*
    
    [requires]
    nanomsg/1.0.0@Wi3ard/stable

    [options]
    nanomsg:shared=True # False
    
    [generators]
    txt
    cmake

Complete the installation of requirements for your project running:

    conan install . 

Project setup installs the library (and all his dependencies) and generates the files *conanbuildinfo.txt* and *conanbuildinfo.cmake* with all the paths and variables that you need to link with your dependencies.
