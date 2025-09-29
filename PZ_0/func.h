#pragma once

#include "structs.h"

void get_point(Point& name);
void print_point(const Point& name);
void get_circle(Circle& name);
void print_circle(const Circle& name);
float length_circle(const Circle& name);
float s_circle(const Circle& name);
void get_square(Square& name);
void print_square(const Square& name);
float length_square(const Square& name);
float s_square(const Square& name);
bool in_circle(const Circle& name, Point point);
bool in_square(const Square& name, Point point);
bool on_circle(const Circle& name, Point point);
bool on_square(const Square& name, Point point);
bool intersection_of_circles(const Circle& name_1, const Circle& name_2);
bool intersection_of_square(const Square& name_1, const Square& name_2);