@all:
	echo -e "nothing to do"

compile:
	./build_project.sh

cmake:
	cd build && cmake ../source && cd ..

make:
	make -C build && cp build/main runtime/server

ninja:
	ninja -C build && cp build/main runtime/server

install:
	./install_dependencies.sh

run:
	make -C ./runtime run

clean:
	make -C ./runtime clean
	rm -rf deps && mkdir deps
	rm -rf build && mkdir build

tanze:
	./tanzomat.py
