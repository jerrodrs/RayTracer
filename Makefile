
all: RayTrace 

RayTrace: main.o Vect.o Ray.o Camera.o Light.o Color.o
	g++ -o RayTrace main.o Vect.o Ray.o Camera.o Light.o Color.o

Color.o: Color.cpp
	g++ -c Color.cpp 

Light.o: Light.cpp
	g++ -c Light.cpp 

Vect.o: Vect.cpp
	g++ -c Vect.cpp 

Ray.o: Ray.cpp
	g++ -c Ray.cpp 

Camera.o: Camera.cpp
	g++ -c Camera.cpp 
	      
main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -f RayTrace *.o  
