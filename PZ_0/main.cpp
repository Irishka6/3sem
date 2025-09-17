#include <iostream>
#include "structs.h"
#include "func.h"


int main(){
    Point name;
    get_point(name);
    print_point(name);
    Circle circl;
    get_circle(circl);
    print_circle(circl);
    std::cout << length_circle(circl) << std::endl;
    std::cout << s_circle(circl) << std::endl;
    Square square;
    get_square(square);
    print_square(square);
    std::cout << length_square(square) << std::endl;
    std::cout << s_square(square) << std::endl;
    std::cout << "in circle" << in_circle(circl, name)  << std::endl;
    std::cout << "in circle" << in_square(square, name)  << std::endl;
    std::cout << "on circle" << on_circle(circl, name)  << std::endl;
    std::cout << "on circle" << on_square(square, name)  << std::endl;
    return 0;
}
