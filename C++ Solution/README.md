C++ solution for the Pizza problem.

The algortihm used calculates the possible rectangles configurations with the maximum number of cells of a slice. For each configuration, it
divides the pizza in horizontal or vertical strips and within those strips it searches for slices with that rectangle shape. It counts the 
the number of wasted cells, then repeats the process for another rectangle configuration. After it does this for all rectangle configurations,
it picks the configuration for which there were the least number of wasted cells, and runs th algorithm again but this times it writes
slices that are found to a file.

On a judge system made by one of my lecturers, this solution got 877400 points.
