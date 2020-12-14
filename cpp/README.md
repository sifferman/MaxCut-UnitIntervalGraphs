# Max-Cut Algorithm Tester

## Purpose

This app helps display Unit Interval Graphs and applies various Max-Cut algorithms.

The intention is to prodce an app that allows for rapid testing of algorithm ideas.

## Getting Started

Simply run `make` to compile, and a "maxcut.exe" file will be created. Run this with `./maxcut.exe <PARAMS>`.

## Command Parameters

You can find all flags with `./maxcut.exe --help`. This will display:

> ./maxcut.exe --help
>
>         Graph Source
>             -r, --random
>                     Randomize the Unit Interval Graph
> 
>             <file_in_path>
>                     Graph is loaded from file
> 
>         Graph Structure
>             -b, --basic
>                     No restrictions on structure
> 
>             -p, --path
>                     Path structure
> 
>             -s, --staircase
>                     Staircase structure
> 
>         Max-Cut Algorithm
>             -bf, --brute-force
>                     Run brute-force algorithm
> 
>             -eo, --every-other
>                     Run every-other approx algorithm
> 
>             -si, --sifferman
>                     Run Sifferman alg (only on path)
> 
>         File Out Path
>             <Output file path>
>                     Change the path of the output file


## Code

| file                    | Functionality                                   |
|-------------------------|-------------------------------------------------|
| `main.cpp`              | Parses input                                    |
| `graph.h`               | Class declarations for Graph and its subclasses |
| `graph.cpp`             | Implementation of all Graph methods             |
| `maxcut_algorithms.cpp` | Implementation of all Max-Cut algorithms        |


## Libraries

This app uses [clipp](https://github.com/muellan/clipp).