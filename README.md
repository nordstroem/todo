# todo

A lightweight to-do list manager which runs in the terminal

## Build requirements
* C++20 compliant compiler
* Linux (tested with Ubuntu 20.04.1, but other distributions should work fine)

## Setup
* `git clone https://github.com/nordstroem/todo.git --recursive`
* `cd todo`
* `cmake --S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
* `cmake --build build`

An alias for the executable `../todo/build/bin/todo` can, for example, be set to `td` for convenience. 
## Usage
* Set the environment variable `TODO_DATABASE_PATH` to be the path of the database file. 

* Adding a task

    ```
    td -a "A task to do"                       // Adds a task to-do today 
    td -a "A task to do" -d 2020-10-07 -p 100  // Adds a task to do at a specific date with a specific priority
    ```
    Both date `-d` and priority `-p` are optional, and are defaulted to `today` and `0` respectively.
*  Show tasks

    ```
    td -s               // Lists all tasks to-do today
    td -s -d 2021-01-02 // At a specific date
    ```
    
*  Check a task (toggles done/not done)

    ```
    td -c 532             // Checks the task with hash 532
    ```

*  Remove a task

    ```
    td -r 32             // Removes the task with hash 32
    ```

