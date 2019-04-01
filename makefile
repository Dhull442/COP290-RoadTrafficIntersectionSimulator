all: rend v road comp removeoutput
v:
ifeq ($(dim),D3)
	g++ -std=c++11 Vehicle.cpp -c -DD3
else
	g++ -std=c++11 Vehicle.cpp -c
endif

rend:
ifeq ($(dim),D3)
		g++ -std=c++11 Render.cpp -DD3 -c -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
else
		g++ -std=c++11 RenderEngine.cpp -c -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
endif
road:
ifeq ($(dim),D3)
	g++ -std=c++11 Road.cpp -c -DD3
else
	g++ -std=c++11 Road.cpp -c
endif
comp:
ifeq ($(dim),D3)
	g++ -std=c++11 -o main main.cpp -DD3  Road.o Vehicle.o Render.o -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
else
	g++ -std=c++11 -o main main.cpp Road.o Vehicle.o RenderEngine.o -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
endif

removeoutput:
	rm -rf output.txt
clean:
	rm -rf *.o main

test:
	g++ -std=c++11 test.cpp -o test -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
