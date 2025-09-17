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
bool in_circle(Circle& name, Point point);
bool in_square(Square& name, Point point);
bool on_circle(Circle& name, Point point);
bool on_square(Square& name, Point point);