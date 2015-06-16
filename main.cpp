#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Object.h"
#include "Source.h"
#include "raytracevars.h"

using namespace std;

struct RGBType {
	double r;
	double g;
	double b;
};

Uint32 * pixels = new Uint32[width * height];

bool runThread = true;
bool threadsFinishedRendering[numOfThreads];
double threadsSpeed[numOfThreads];

double camtrackerx = 1.0;
double camtrackery = 1.5;
double camtrackerz = -4;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture * texture;

SDL_Thread *displayThread;
SDL_Thread *threads[numOfThreads];

//RAY TRACE ENGINE FUNCTIONS
int winningObjectIndex(vector<double> object_intersections){
	// return the index of the winning intersection
	int index_of_minimum_value;

	//prevent unnecessary calculations
	if (object_intersections.size() == 0){
		// if there are no intersections
		return -1;
	}else if (object_intersections.size() == 1){
		//only one object is intersected
		if (object_intersections.at(0) > 0){
			//if that intersection is greater than zero then its our index of minimum
			return 0;
		}else{
			//otherwise the only intersection value is negative, missed everything
			return -1;
		}
	}else{
		// otherwise there is more than one intersection
		// first find the maximum value

		double max = 0;
		for (int i = 0; i < object_intersections.size(); i++){
			if (max < object_intersections.at(i)){
				max = object_intersections.at(i);
			}
		}

		// then starting from the maximum value find the minimum position value
		if (max > 0){
			// we only want positive intersections
			for (int index = 0; index < object_intersections.size(); index++){
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max){
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}else{
			// all the intersections were negative
			return -1;
		}
	}

}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources,  double accuracy, double ambientlight, int bounces){
	
	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	if (winning_object_color.getColorSpecial() == 2){
		//tile floor

		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		if ((square % 2) == 0) {
			//black
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}else{
			//white
			winning_object_color.setColorRed(1);
			winning_object_color.setColorGreen(1);
			winning_object_color.setColorBlue(1);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
		//reflection from objects that have spec value
		double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
		Vect scalar1 = winning_object_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray (intersection_position, reflection_direction);

		//determine what the ray intersects first
		vector<double> reflection_intersections;

		for(int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++){
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}

		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1){
			//reflection ray missed everything else
			//recursive number to keep track of light bounces
			int numberOfBounces = bounces;
			if (numberOfBounces < 2){
				numberOfBounces++;
				//determine the position and direction at the point of intersection with the ray
				// the ray only affects the color if it reflects off something
				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction,scene_objects,index_of_winning_object_with_reflection,light_sources, accuracy, ambientlight,numberOfBounces);

				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
			}
		}

	}

	for (int light_index = 0; light_index < light_sources.size(); light_index++){
		Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

		float cosine_angle = winning_object_normal.dotProduct(light_direction);

		if (cosine_angle > 0){
			// test for shadows
			bool shadowed = false;

			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative());
			float distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;


			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++){
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}

			for (int c = 0; c < secondary_intersections.size(); c++){
				if(secondary_intersections.at(c) > accuracy){
					if (secondary_intersections.at(c) <= distance_to_light_magnitude){
						shadowed = true;
					}
					break;
				}
			}
			if (shadowed == false){
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
					//special 0 to 1
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0){
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}
			}
			

		}
	}

	return final_color.clip();
}

unsigned long createRGBA(int a, int r, int g, int b)
{   
    return ((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8)
           + (b & 0xff);
}



//SDL INIT AND CLOSE
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
	}

	return success;
}


void close()
{
	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;

	for (int i = 0; i < numOfThreads; i++){
		delete threads[i];
	}
	delete displayThread;
	delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

	//Quit SDL subsystems
	SDL_Quit();
}


//SCENE VARIABLES
Vect origin (0,0,0);
Vect X (1,0,0);
Vect Y (0,1,0);
Vect Z (0,0,1);

Color white_light (1.0, 1.0, 1.0, 0);
Color pretty_green (0.5, 1.0, 0.5, 0.3);
Color pretty_blue (0.1, 0.5, 1.0, 1);
Color maroon (0.5, 0.25, 0.25, 2);
Color gray (0.5, 0.5, 0.5, 0);
Color black (0.0, 0.0 ,0.0 ,0.5);

Vect look_at (0,0,0); //direction of camera
Vect light_pos (-7, 10, -10);

Vect t (0,3,3);


Sphere scene_sphere (origin, 0.5, pretty_green);
Sphere scene_sphere2 (t, 3, pretty_blue);

Plane scene_plane (Y, -1, maroon);

int renderFunction(void *data){
		while(runThread){
			int secNum = (int)data;
			if(threadsFinishedRendering[secNum-1] == false){
				clock_t start;
				double duration;
				start = clock();

				Vect campos (camtrackerx-1.2, camtrackery, camtrackerz);

				//Vect cam_sphere_pos (camtrackerx,camtrackery,camtrackerz);
				//Sphere cam_sphere (cam_sphere_pos, 1, pretty_blue);
				Vect diff_btw (campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ()); //difference between camera's coor - look at

				Vect camdir = diff_btw.negative().normalize();
				Vect camright = Y.crossProduct(camdir).normalize();
				Vect camdown = camright.crossProduct(camdir);

				Camera scene_cam (campos, camdir, camright, camdown);

				Light scene_light (light_pos,white_light);
				vector<Source*> light_sources;
				light_sources.push_back(dynamic_cast<Source*>(&scene_light));

				vector<Object*> scene_objects;
				scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
				scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
				//scene_objects.push_back(dynamic_cast<Object*>(&cam_sphere));
				scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));

				double xamnt, yamnt;
				int aa_index;

				for (int x = 0; x < width; x++){
					for (int y  = (int)((height/numOfThreads)*(secNum-1)); y < (int)((height/numOfThreads)*(secNum)); y++){

					
						//blank pixel
						double tempRed[aadepth*aadepth];
						double tempGreen[aadepth*aadepth];
						double tempBlue[aadepth*aadepth];

						for (int aax = 0; aax < aadepth; aax++){
							for(int aay = 0; aay < aadepth; aay++){

								aa_index = aay*aadepth + aax;

								srand(time(0));

								//create the ray from the camera to this pixel
								if(aadepth == 1){
									//start with no anti aliasing
									if (width > height) {
										//the image is wider than tall
										xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
										yamnt = ((height -y) + 0.5)/height;
									}else if (height > width){
										//the image is taller than wide
										xamnt = (x+0.5)/ width;
										yamnt = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
									}else{
										//the image is square
										xamnt = (x+0.5)/width;
										yamnt = ((height - y) + 0.5)/height;
									}
								}else{
									//anti alias
									if (width > height) {
										//the image is wider than tall
										xamnt = ((x+(double)aax/((double)aadepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
										yamnt = ((height -y) + (double)aax/((double)aadepth - 1))/height;
									}else if (height > width){
										//the image is taller than wide
										xamnt = (x+(double)aax/((double)aadepth - 1))/ width;
										yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectratio - (((height - width)/(double)width)/2);
									}else{
										//the image is square
										xamnt = (x+(double)aax/((double)aadepth - 1))/width;
										yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
									}
								}



								Vect cam_ray_origin = scene_cam.getCameraPosition();
								Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

								Ray cam_ray (cam_ray_origin, cam_ray_direction);

								vector<double> intersections;
								//0.02 seconds up to this point^


								for (int index = 0; index < scene_objects.size(); index++){
									//loops through each object in scene and finds intersection
									intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
								}
								//0.08 seconds to this point^

								int index_of_winning_object = winningObjectIndex(intersections);
								//0.104 seconds to this point^

								//painting the scene
								if (index_of_winning_object == -1){
									tempRed[aa_index] = 0;
									tempGreen[aa_index] = 0;
									tempBlue[aa_index] = 0;
									//pixels[y * width + x] = 0;
								}else{
									//index is a hit on an object in the scene
									if (intersections.at(index_of_winning_object) > accuracy){
										//determine the position and direction vectors at the point of intersection

										Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
										Vect intersecting_ray_direction = cam_ray_direction;
				
										Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight,0);
										tempRed[aa_index] = intersection_color.getColorRed()*255;
										tempGreen[aa_index] = intersection_color.getColorGreen()*255;
										tempBlue[aa_index] = intersection_color.getColorBlue()*255;

										//pixels[y * width + x] = createRGBA(255,(int)(intersection_color.getColorRed()*255), (int)(intersection_color.getColorGreen()*255), (int)(intersection_color.getColorBlue()*255));

									}
								}
								if(y == (int)((height/numOfThreads)*(secNum-1))){pixels[y * width + x] = 0x00ff0000;}
								//0.308 seconds to this point^, about 0.080 is for rendering pixels
							}
						}

						double totalRed = 0;
						double totalGreen = 0;
						double totalBlue = 0;

						for(int iRed = 0; iRed < aadepth*aadepth; iRed++){
							totalRed = totalRed + tempRed[iRed];
						}
						for(int iGreen = 0; iGreen < aadepth*aadepth; iGreen++){
							totalGreen = totalGreen + tempGreen[iGreen];
						}
						for(int iBlue = 0; iBlue < aadepth*aadepth; iBlue++){
							totalBlue = totalBlue + tempBlue[iBlue];
						}

						double avgRed = totalRed/(aadepth*aadepth);
						double avgGreen = totalGreen/(aadepth*aadepth);
						double avgBlue = totalBlue/(aadepth*aadepth);

						pixels[y * width + x] = createRGBA(255,avgRed,avgGreen,avgBlue);
					}
				}
				//delete tempBlue, tempGreen, tempRed;
				threadsFinishedRendering[secNum-1] = true;
				duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
				threadsSpeed[secNum-1] = duration;
				//cout << "Thread Number: " << secNum << " with time: " << duration << " seconds" << endl; 
			}
		}
		return 0;
}

int displayFunction(void *data){
	while (true){	
		int count = 0;
		for(int i = 0; i < numOfThreads; i++){
			if(threadsFinishedRendering[i] == true){
				count++;
			}
		}
		if (count == numOfThreads){
			SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
		    SDL_UpdateTexture(texture, NULL, pixels, width * 4);
		    SDL_RenderClear(renderer);
		    SDL_RenderCopyEx(renderer, texture, NULL, NULL, NULL, NULL, flip);
			SDL_RenderPresent(renderer);
			double fps = 0;
			for(int i = 0; i < numOfThreads; i++){
				threadsFinishedRendering[i] = false;
				if(threadsSpeed[i] > fps){fps = threadsSpeed[i];}

			}
			cout << 1/fps << "fps" << endl;

		}


	}
	return 0;
}


int main(int argc, char *argv[]){
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{	
		//Main loop flag
		bool quit = false;
		bool singleRender = false;
		bool singleRender2 = false;

		int widthBoxes = width/32;
		int heightBoxes = height/32;

		for(int i = 0; i < numOfThreads; i++){
			int secNum = i + 1;
			threads[i] = SDL_CreateThread(renderFunction, "Render", (void*)secNum);
		}
		displayThread = SDL_CreateThread(displayFunction, "Display", (void*)NULL);



		//Event handler
		SDL_Event e;
		
		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}else if (e.type == SDL_KEYDOWN){
					switch ( e.key.keysym.sym )
					{
						case SDLK_RIGHT:
							camtrackerx += 0.1;
							break;
						case SDLK_LEFT:
							camtrackerx -= 0.1;
							break;
						case SDLK_DOWN:
							camtrackery += 0.1;
							break;
						case SDLK_UP:
							camtrackery -= 0.1;
							break;
						default :
							break;
					}
				}
			}	

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}