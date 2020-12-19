# Max-Cut Algorithm Tester

Repo: https://github.com/E4tHam/MaxCut-UnitIntervalGraphs/edit/main/algorithm-tester/

Download: https://github.com/E4tHam/MaxCut-UnitIntervalGraphs/releases/download/v1.0/algorithm-tester.zip

## Purpose

This app helps display Unit Interval Graphs and applies various Max-Cut algorithms.

The intention is to prodce an app that allows for rapid testing of algorithm ideas.

## Getting Started

Run `make` to compile, and a "maxcut.exe" file will be created. Run this with `./maxcut <PARAMS>`.

## Command Parameters

You can find all flags with `./maxcut --help`. This will display:

> ./maxcut --help
>
>         Graph Source
>             -r, --random
>                     Randomize the Unit Interval Graph
> 
>             -fi, --file-in <Input file path>
>                     Graph is loaded from file
>
>         File Cut
>             -fc, --file-cut
>                         Cut according to file input
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
>             -fo, --file-out <Output file path>
>                     Change the path of the output file


## Code

| file                    | Functionality                                   |
|-------------------------|-------------------------------------------------|
| `main.cpp`              | Parses input                                    |
| `graph.h`               | Class declarations for Graph and its subclasses |
| `graph.cpp`             | Implementation of all Graph methods             |
| `maxcut_algorithms.cpp` | Implementation of all Max-Cut algorithms        |


## Libraries

Libraries used:

 * [clipp](https://github.com/muellan/clipp)

