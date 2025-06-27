.PHONY: build

build:
	cmake -S . -B build
	cmake --build build
	cp ./build/3700kvstore ./3700kvstore

symlink:
	ln -s build/compile_commands.json . 
