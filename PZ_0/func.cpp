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

bool in_circle(const Circle& name, Point point){
    return (abs(name.centre.x - point.x) < name.r) && (abs(name.centre.y - point.y) < name.r);
}

bool in_square(const Square& name, Point point){
    return (name.left_point.x < point.x) && ((name.left_point.x + name.a) > point.x) && (name.left_point.y > point.y) && ((name.left_point.y - name.a) < point.y);
}   

bool on_circle(const Circle& name, Point point){
    return abs(name.centre.x - point.x) * abs(name.centre.x - point.x) + abs(name.centre.y - point.y) * abs(name.centre.y - point.y) == name.r * name.r;
}

bool on_square(const Square& name, Point point){
    bool lines_x = ((name.left_point.y == point.y)||(name.left_point.y - name.a == point.y)) && (name.left_point.x <= point.x) && ((name.left_point.x + name.a) >= point.x);
    bool lines_y = ((name.left_point.x == point.x)||(name.left_point.x + name.a == point.x)) && (name.left_point.y >= point.y) && ((name.left_point.y - name.a) <= point.y);
    return lines_x || lines_y ;
}   

bool intersection_of_circles(const Circle& name_1, const Circle& name_2){
    float x = name_1.centre.x - name_2.centre.x;
    float y = name_1.centre.y - name_2.centre.y;
    return sqrt(x * x + y * y) < (name_1.r + name_2.r);
}

bool intersection_of_square(const Square& name_1, const Square& name_2){
    Point mus_1[4], mus_2[4];
    mus_poin(name_1, mus_1);
    mus_poin(name_2, mus_2);

    bool flag_1 = false;
    for (int i = 0; i < 4; i++){
        if (in_square(name_1, mus_2[i]) || in_square(name_2, mus_1[i])){
            flag_1 = true;
            break;
        }
    }
    return flag_1;
}

bool intersection(const Circle& name_1, const Square& name_2){
    Point mus_2[4];
    mus_poin(name_2, mus_2);
    for (int i = 0; i < 4; i++){
        if (in_circle(name_1, mus_2[i])){
            return true;
        }
    }

    if (in_square(name_2, name_1.centre)) {
        return true;
    }

    float closestX = name_1.centre.x;
    float closestY = name_1.centre.y;
    
    if (name_1.centre.x < name_2.left_point.x) closestX = name_2.left_point.x;
    else if (name_1.centre.x > name_2.left_point.x + name_2.a) closestX = name_2.left_point.x + name_2.a;
    
    if (name_1.centre.y > name_2.left_point.y) closestY = name_2.left_point.y;
    else if (name_1.centre.y < name_2.left_point.y - name_2.a) closestY = name_2.left_point.y - name_2.a;
    
    float dx = name_1.centre.x - closestX;
    float dy = name_1.centre.y - closestY;
    
    return (dx * dx + dy * dy) <= (name_1.r * name_1.r);
}

bool circl_in_circl(const Circle& name_1, const Circle& name_2){
    float x = name_1.centre.x - name_2.centre.x;
    float y = name_1.centre.y - name_2.centre.y;
    return x * x + y * y < (name_1.r - name_2.r)*(name_1.r - name_2.r);
}

bool square_in_square(const Square& name_1, const Square& name_2){
    Point mus_1[4], mus_2[4];
    mus_poin(name_1, mus_1);
    mus_poin(name_2, mus_2);

    bool flag_1 = false;
    for (int i = 0; i < 4; i++){
        if (in_square(name_1, mus_2[i])){
            flag_1 = true;
        }else{
            flag_1 = false;
            break;
        }
    }

    bool flag_2 = false;
    for (int i = 0; i < 4; i++){
        if (in_square(name_2, mus_1[i])){
            flag_2 = true;
        }else{
            flag_2 = false;
            break;
        }
    }
    return flag_1 || flag_2;
}

bool square_in_circle(const Square& name_1, const Circle& name_2){
    Point mus[4];
    mus_poin(name_1, mus);
    for (int i = 1; i <= 4; i++){
        
    }
    return in_circle(name_2, mus[0]) && (name_2.r * 2 >= name_1.a * sqrt(2)); 
}

bool circle_in_square(const Square& name_1, const Circle& name_2){
    return in_square(name_1, name_2.centre) && (name_2.r * 2 <= name_1.a); 
}

void mus_poin(const Square& name_1, Point mus_1[4]){
    // Квадрат
    mus_1[0] = name_1.left_point; // левый верхний
    mus_1[1] = name_1.left_point; // правый верхний
    mus_1[1].x += name_1.a;
    mus_1[2] = name_1.left_point; // левый нижний
    mus_1[2].y -= name_1.a;
    mus_1[3] = name_1.left_point; // правый нижний
    mus_1[3].x += name_1.a;
    mus_1[3].y -= name_1.a;
}