# Path Case Solver

App: https://maxcut.sifferman.dev/path/

Repo: https://github.com/E4tHam/MaxCut-UnitIntervalGraphs/tree/main/docs/path

## Purpose

This app impliments an algorithm to solve the path case.

## App Explanation

"S" represents the size of each twin class.

Recall the Max-Cut problem separates a graph into two disjoint subsets, "A" and "B".
For the odd twin classes, the slider shows how many verticies are in subset "A".
For the even twin classes, the slider shows how many verticies are in subset "B".

A twin class "i" with a picture [ / ] means that n_i is locally maximal at S_i.
A twin class "i" with a picture [ \ ] means that n_i is locally maximal at 0.
A twin class "i" with a picture [ /\ ] means that n_i is locally maximal between 0 and S_i.

The [ Maximize ] button, locally maximizes that twin class's "n" value.
