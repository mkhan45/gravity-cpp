#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <tuple>
#include <unistd.h>

using namespace std;

const float G = 10.0;

float distance(float x1, float y1, float x2, float y2){
    float dx_sqr = pow(x2 - x1, 2);
    float dy_sqr = pow(y2 - y1, 2);
    return sqrt(dx_sqr + dy_sqr);
}

class Body{
    public:
        float radius;
        float mass;
        float x;
        float y;
        float v_x;
        float v_y;

    public:
        Body (float, float, float, float);
        float accel_mag(float);
        tuple<float, float> accel_vector(float x, float y);
};

Body::Body (float r, float m, float x1, float y1){
    radius = r;
    mass = m;
    x = x1;
    y = y1;
    v_x = 0.0;
    v_y = 0.0;
}

float Body::accel_mag(float radius){
    return (G * mass) / pow(radius, 2);
}

tuple<float, float> Body::accel_vector(float other_x, float other_y){
    float radius = distance(x, y, other_x, other_y);
    float mag = accel_mag(radius);
    float x_comp = (x - other_x)/radius * mag;
    float y_comp = (y - other_y)/radius * mag;
    return make_tuple(x_comp, y_comp);
}

int main(){
    vector<Body> bodies;

    bodies.push_back(Body (5.0, 5.0, 10.0, 10.0));
    bodies.push_back(Body (5.0, 25.0, 100.0, 100.0));
    bodies.push_back(Body (15.0, 125.0, 600.0, 600.0));

    while(true){
        for(int i = 0; i < bodies.size(); i++){
            float accel_x = 0.0;
            float accel_y = 0.0;

            Body current = bodies[i];

            cout << "x: " << current.x << " y: " << current.y << "\n";

            for(int j = 0; j < bodies.size(); j++){
                Body other = bodies[j];
                if(distance(current.x, current.y, other.x, other.y) >= current.radius + other.radius){
                    tuple<float, float> accel = other.accel_vector(current.x, current.y);
                    accel_x += get<0>(accel);
                    accel_y += get<1>(accel);
                }
            }

            current.v_x += accel_x;
            current.v_y += accel_y;

            current.x += current.v_x + (accel_x/2);
            current.y += current.v_y + (accel_y/2);

            bodies[i] = current;

            /* cout << "After x: " << current.x << " After y: " << current.y << "\n"; */
        }

        usleep(16666);
    }
}
