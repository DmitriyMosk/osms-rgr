CXXFLAGS = -std=c++20 -Wall -I$(shell find $(PWD) -type d -name include)

LIBDIR1 = ./cpp_code/helpers
LIBDIR2 = ./cpp_code/io
LIBDIR3 = ./cpp_code/signal

build: lib_helpers io_build lib_signal 
	g++ $(CXXFLAGS) ./cpp_code/main.cpp -o cpp_app.out \
	-L$(LIBDIR1) -lhelpers \
	-L$(LIBDIR3) -lsignal \
	-L$(LIBDIR2) -li

lib_helpers:
	$(MAKE) -C ./cpp_code/helpers CXXFLAGS="$(CXXFLAGS)"

io_build: lib_helpers
	$(MAKE) -C ./cpp_code/io CXXFLAGS="$(CXXFLAGS)"

lib_signal: io_build 
	$(MAKE) -C ./cpp_code/signal CXXFLAGS="$(CXXFLAGS)"

run:
	$(MAKE) -C . clean 
	$(MAKE) -C . build -j10
	./cpp_app.out
	rm ./cpp_app.out

sim_clean: 
	ggwp2

debug: 
	ggwp

vio: 
	/home/dmitry2401/dev/git_projects/sibsutis-ts/.venv/bin/python /home/dmitry2401/dev/git_projects/osms-rgr/visualisation/script.py

v2: 
	$(MAKE) -C ./cpp_code/build all

vio_v2: 
	cd ./cpp_code/build
	/home/dmitry2401/dev/git_projects/sibsutis-ts/.venv/bin/python /home/dmitry2401/dev/git_projects/osms-rgr/cpp_code/build/python/visualize.py /home/dmitry2401/dev/git_projects/osms-rgr/cpp_code/build/data/descriptions.txt

clean: 
	rm ./cpp_app.out || true
	$(MAKE) -C ./cpp_code/io clean
	$(MAKE) -C ./cpp_code/helpers clean
	$(MAKE) -C ./cpp_code/signal clean