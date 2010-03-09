TARGETS = nqueen
CXXFLAGS = -Wall
all: $(TARGETS)
test: $(TARGETS)
	./nqueen 4
clean:
	rm $(TARGETS) *.o
nqueen: nqueen.o main.o
	$(CXX) -o $@ -lboost_program_options $^
