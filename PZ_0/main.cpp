#include <iostream>
#include "structs.h"
#include "func.h"


int main(){
    // Point name;
    //get_point(name);
    // print_point(name);
    // Circle circl;
    // get_circle(circl);
    // print_circle(circl);
    // std::cout << length_circle(circl) << std::endl;
    // std::cout << s_circle(circl) << std::endl;
    Square square;
    get_square(square);
    std::cout << length_square(square) << std::endl;
    std::cout << s_square(square) << std::endl;
    // std::cout << "in circle" << in_circle(circl, name)  << std::endl;
    // std::cout << "in square" << in_square(square, name)  << std::endl;
    // std::cout << "on circle" << on_circle(circl, name)  << std::endl;
    // std::cout << "on square" << on_square(square, name)  << std::endl;
    // Circle circl_2;
    // get_circle(circl_2);
    // print_circle(circl);
    // print_circle(circl_2);
    // std::cout << "intersection of circles" << intersection_of_circles(circl, circl_2)  << std::endl;
    Square square_2;
    get_square(square_2);
    print_square(square);
    print_square(square_2);
    std::cout << "intersection of square " << intersection_of_square(square, square_2)  << std::endl;
    return 0;
}
