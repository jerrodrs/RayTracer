
all: RayTrace 

RayTrace: main.o Vect.o Ray.o Camera.o Light.o Color.o Sphere.o Object.o Plane.o Source.o
	g++ -o RayTrace main.o Vect.o Ray.o Camera.o Light.o Color.o Sphere.o Object.o Plane.o Source.o

Source.o: Source.cpp
	g++ -c Source.cpp 

Plane.o: Plane.cpp
	g++ -c Plane.cpp 

Object.o: Object.cpp
	g++ -c Object.cpp 

Sphere.o: Sphere.cpp
	g++ -c Sphere.cpp 

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
