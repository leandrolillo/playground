-------------
Dependencies:
-------------

already installed: openAl, openGl

#requires clang command line installed and running thus we have to install xcode and then xcode cli

xcode-select --install

brew install cmake
brew install libjpeg
brew install libpng
brew install sdl2
brew install libogg
brew install libvorbis

or singleline: brew install cmake libjpeg libpng sdl2 libogg libvorbis



-------------
Building:
-------------

'cmake-clean.sh' for cleaning cmake generated files

'cmake .' for generating make file - this does not build the executable, only puts toghether a makefile!
	'cmake -DCMAKE_BUILD_TYPE=Debug .' for debug profile
	'cmake -DCMAKE_BUILD_TYPE=Release .' for release profile
	
'make' for generating executable file in target directory



---------------------------
Debuging with lldb in osx - developers group
---------------------------
You might need to add your user to developers group in order to use lldb: 

	sudo dscl . append /Groups/_developer GroupMembership llillo

sample command line for rebuilding, running and tailing the log file:
--------------------------------------------------------------------

release: './cmake-clean.sh && cmake . && make && ./target/playground'

release & tail log: './cmake-clean.sh && cmake . && make && (./target/playground &  tail -n200 -f ./playground.log)'

debug: './cmake-clean.sh && cmake -DCMAKE_BUILD_TYPE=Debug . && make && lldb ./target/playground -S lldb.settings'