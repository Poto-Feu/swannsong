# Changelog
Major changes will be documented here. For minor changes such as typos or more details, see the commit history.

## Pre-Alpha 1 - 2020-04-03

### Added

* Changelog ;-)
* Intvar (struct with variable name and int value)
* Inventory system (using intvar)
* Simple unit testing

## Pre-Alpha 2 - 2020-05-04

### Added

* Support for conditions in the interpreter
* Implement token system in the interpreter
* Implement curses library

### Changed

* Refactor some parts in C++ such as intvar
* Use OOP in room module
* Store roomfile in a vector in order to reduce drive I/O

## v0.1 - 2020-05-26

### Added

* Create a GO function which can be used to move the player into another Room
* Add a Room choice prompt
* Create a textual cutscenes system

### Change

* Polish the textual UI
* Refactor most modules into C++
