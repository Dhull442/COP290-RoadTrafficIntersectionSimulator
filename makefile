CC=g++ -std=c++11
run:
	$(CC) Vehicle.cpp
clean:
	rm -rf a.out
me:
	g++ -std=c++11 main.cpp -o main
