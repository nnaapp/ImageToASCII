converter: main.o image.o
	g++ main.o image.o -o converter

main.o: main.cpp
	g++ -c main.cpp

image.o: image.cpp
	g++ -c image.cpp

clean:
	rm *.o