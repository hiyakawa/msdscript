CXX= c++
CFLAGS= --std=c++14 -O2
HEADER= $(wildcard *.h)
CCSOURCE= $(wildcard *.cpp)

msdscript: main.o expr.o parse.o val.o env.o
	$(CXX) $(CFLAGS) -o msdscript $^

test_msdscript:  tests.o exec.o
	$(CXX) $(CFLAGS) -o test_msdscript tests.o exec.o

main.o: main.cpp cmdline.h catch.h pointer.h
	$(CXX) $(CFLAGS) -c main.cpp

tests.o: tests.cpp exec.h
	$(CXX) $(CFLAGS) -c tests.cpp

expr.o: expr.cpp expr.h
	$(CXX) $(CFLAGS) -c expr.cpp

val.o: val.cpp val.h
	$(CXX) $(CFLAGS) -c val.cpp

parse.o: parse.cpp parse.h
	$(CXX) $(CFLAGS) -c parse.cpp

exec.o: exec.cpp exec.h
	$(CXX) $(CFLAGS) -c exec.cpp

env.o: env.cpp env.h
	$(CXX) $(CFLAGS) -c env.cpp

.PHONY: test
test: msdscript
	./msdscript --test

.PHONY: doc
doc:
	cd documentation && doxygen

clean:
	rm -f *.o *~