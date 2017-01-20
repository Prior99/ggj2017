.PHONY: web run default clean clang webrun

default:
	mkdir -p build
	cd build; cmake ..; make -j

clang:
	mkdir -p build
	cd build; CXX=clang++ cmake ..; make -j

run: default
	build/ggj2017

web:
	mkdir -p build-web
	cd build-web; /usr/lib/emscripten/emcmake cmake ..; make

ci-web:
	mkdir -p build-web
	cd build-web; /usr/local/bin/emcmake cmake ..; make

webrun:
	emrun build-web/index.html

clean:
	rm -rf build
	rm -rf build-web
