CC=g++ -std=c++11
run:
	$(CC) Vehicle.cpp

me:
	g++ -std=c++11 main.cpp -o main

rv:
	g++ -std=c++11 RenderEngine.cpp -c -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama 
	g++ -std=c++11 Vehicle.cpp -c
	g++ -std=c++11 Road.cpp -c
	g++ -std=c++11 -o main main.cpp -DIMPL Road.o Vehicle.o RenderEngine.o -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama 

rend:
		g++ -o RenderEngine RenderEngine.cpp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursorclear

clean:
	rm -rf *.o a.out
