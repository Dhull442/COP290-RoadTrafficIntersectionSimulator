CC=g++ -std=c++11
run:
	$(CC) Vehicle.cpp

me:
	g++ -std=c++11 main.cpp -o main

rv:

	g++ -std=c++11 Vehicle.cpp -c
	g++ -std=c++11 Road.cpp -c
	g++ -std=c++11 -o main Road.o Vehicle.o


clean:
	rm -rf *.o a.out
