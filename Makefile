@all:
	echo -e "nothing to do"

compile:
	./build_project.sh

cmake:
	clear && cd build && cmake ../source && cd ..

make:
	clear && make -C build && cp build/main runtime/server

ninja:
	clear && ninja -C build && cp build/main runtime/server

install:
	./install_dependencies.sh

run:
	make -C ./runtime run

clean:
	make -C ./runtime clean
	rm -rf deps && mkdir deps
	rm -rf build && mkdir build
	@ echo -e emacs files to be cleaned: $$(find -name "*~" | xargs)

rebase:
	rm -rf ./runtime/database.db

tanze:
	make -C testing tanze
