#include "structs.h"
#include <math.h>
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */

void get_point(Point& name){
    std::cout << "Coordinates of the point: x y" << std::endl;
    std::cin >> name.x;
    std::cin >> name.y;
}

void print_point(const Point& name){
    std::cout << "Point: (" << name.x << ", " << name.y << ")" << std::endl;
}

void get_circle(Circle& name){
    std::cout << "Coordinates of the centre: x y" << std::endl;
    std::cin >> name.centre.x;
    std::cin >> name.centre.y;
    std::cout << "Radius length" << std::endl;
    std::cin >> name.r;
}

void print_circle(const Circle& name){
    std::cout << "Point: (" << name.centre.x << ", " << name.centre.y << ")" << std::endl;
    std::cout << "R = " << name.r << std::endl;
}

float length_circle(const Circle& name){
    return M_PI * 2 * name.r;
}

float s_circle(const Circle& name){
    return M_PI * name.r * name.r;
}

void get_square(Square& name){
    std::cout << "Coordinates of the left_point: x y" << std::endl;
    std::cin >> name.left_point.x;
    std::cin >> name.left_point.y;
    std::cout << "Side length" << std::endl;
    std::cin >> name.a;
}

void print_square(const Square& name){
    std::cout << "Point: (" << name.left_point.x << ", " << name.left_point.y << ")" << std::endl;
    std::cout << "size = " << name.a << std::endl;
}

float length_square(const Square& name){
    return 4 * name.a;
}

float s_square(const Square& name){
    return name.a * name.a;
}

bool in_circle(Circle& name, Point point){
    return (abs(name.centre.x - point.x) < name.r) && (abs(name.centre.y - point.y) < name.r);
}

bool in_square(Square& name, Point point){
    return (name.left_point.x < point.x) && ((name.left_point.x + name.a) > point.x) && (name.left_point.y > point.y) && ((name.left_point.y - name.a) < point.y);
}   

bool on_circle(Circle& name, Point point){
    return abs(name.centre.x - point.x) * abs(name.centre.x - point.x) + abs(name.centre.y - point.y) * abs(name.centre.y - point.y) == name.r * name.r;
}

bool on_square(Square& name, Point point){
    bool lines_x = ((name.left_point.y == point.y)||(name.left_point.y - name.a == point.y)) && (name.left_point.x <= point.x) && ((name.left_point.x + name.a) >= point.x);
    bool lines_y = ((name.left_point.x == point.x)||(name.left_point.x + name.a == point.x)) && (name.left_point.y >= point.y) && ((name.left_point.y - name.a) <= point.y);
    return lines_x || lines_y ;
}   