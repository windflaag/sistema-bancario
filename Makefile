@all:
	echo -e "nothing to do"

build:
	./build_project.sh

install:
	./install_dependencies.sh

run:
	make -C ./runtime run

clean:
	make -C ./runtime clean
	rm -rf deps && mkdir deps
	rm -rf build && mkdir build
