build:
	cmake -S . -B build
	cmake --build build

symlink:
	ln -s build/compile_commands.json . 
