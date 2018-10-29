# About

This is a rough implementation of the library of babel in the C programming language only using standard C libraries.
Inspired by the original found at https://libraryofbabel.info/

# Compilation

	1. Go to cloned folder
	2. $ cmake .
	3. $ make
	
The executable is called "babel". Make sure you create a library the first time you launch it.
Tip: don't create too many chambers... the library becomes very big filesize wise...

# Tweaking the library

In the library.h file you can set some library specific settings, which are quite self explanatory.
I might implement a configuration file reader so that you dont have to recompile the program each time you change the settings.

# Todo's 

- At the moment the text generated doesn't contain punctuation characters like in the original library of babel (spaces, dots and commas), so its one big text...
- word search algorithm can be improved...
- configuration file reader instead of defines
