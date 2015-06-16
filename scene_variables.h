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
Vect cam_shere_pos(0,0,0);
Sphere scene_sphere (origin, 0.5, pretty_green);
Sphere scene_sphere2 (t, 3, pretty_blue);
Sphere cam_sphere (t, 0.25, pretty_blue);
Plane scene_plane (Y, -1, maroon);