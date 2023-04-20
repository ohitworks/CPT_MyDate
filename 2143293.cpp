/**
  **************************** Project: 2143293.cpp ****************************
  * Created by TODO:AuthorHere on 2023/4/14.
  * @file
  * @author     TODO:AuthorHere
  * @date       2023-04-14
  * @brief      TODO:BriefHere
  **************************** Project: 2143293.cpp ****************************
 */

#include "MyDate.cpp"

#include <ctime>
#include <iostream>


int main() {
    auto day = cpt_project_2::MyDate(2020, "Feb", 27);
    std::cout << asctime(&day.time_struct) << std::endl;
    auto day2 = day + 2;
    day2++;
    std::cout << asctime(&day2.time_struct) << std::endl;

    return 0;
}
