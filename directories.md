# The bin directory

This is where the built executable is stored. This directory needs to exist
    in order for ```make``` to work, so this file ensures the directory structure
    is preserved through git.

# The debug directory

This directory is where the output of debug programs such as
    Valgrind are kept. Running ```make valgrind``` will produce
    a log file in this directory

# The include directory

This directory is for all the ```.h``` files. All the files
    in this directory will be included in the ```make``` command
    to produce an executable, regardless of whether they are actually used.

# The obj directory

This directory is for all the ```.o``` files generated from
    their respective ```.c``` files. These files are made by ```make```
    during the compilation process.

# The src directory

This is for all the ```.c``` files. Any other files here will not be included
    in the compilation/build process.