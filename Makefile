FILE = main.cpp
OUTPUT = a

all:
	g++ -O3 $(FILE) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
