The project consists of the following files:
    1. library.cpp
    2. library.hpp
    3. book.hpp
    4. utils.hpp

The project can be run by either compiling library.cpp using g++ and running the executable with the path to the directory of books as command line argument
Alternatively, a Makefile has been provided through which the project can be runned directly by calling "make"

To do so:
    1. Copy the project files along with the Makefile into a suitable folder
    2. Open a terminal at that folder
    3. Run the following command
        $ make ARGS=/path/to/directory/