# About

This is a very naive implementation of the library of babel in the C programming language only using standard C libraries.
Inspired by the original found at https://libraryofbabel.info/
I made this during one of my first years in University.

# Compilation

	1. git clone https://github.com/DorianTe/Pseudo-Library-of-babel-C.git
	2. cmake .
	3. make
	
The executable is called "babel". Make sure you create a library the first time you launch it.  
Tip: don't create too many chambers... the library can take up a lot of space...

# Tweaking the library

In the library.h file you can set some library specific settings, which are quite self explanatory.
I might implement a configuration file reader so that you dont have to recompile the program each time you change the settings.

# Todo's 

- The generated text doesn't contain punctuation characters like in the original library of babel (spaces, dots and commas), so its one big text...
- Word search algorithm can be improved...
- Configuration file reader instead of defines
