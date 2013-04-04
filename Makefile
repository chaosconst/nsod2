bin: src/*
	mkdir -p bin
	cd src && make
clean: 
	rm -rf bin
	cd src && make clean
