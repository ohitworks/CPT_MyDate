/**
  **************************** Project: 2143293.cpp ****************************
  * Created by oh-it-works on 2023/4/14.
  * @file
  * @author     oh-it-works
  * @date       2023-04-14
  * @brief      TODO:BriefHere
  **************************** Project: 2143293.cpp ****************************
 */
#include "MyDate.cpp"
#include "SubDate.cpp"

#include <iostream>


int main() {
    cpt_project_2::USE_FEBRUARY_ALWAYS_28_DAYS = true;
    auto day = cpt_project_2::MyDate(2020, "February", 28);
    std::cout << day.date << std::endl;
    auto day2 = day + 1;
    day2++;
    std::cout << day2.date << std::endl;

    auto day_sub = cpt_project_2::SubDate(2020, "February", 28, 1, 1, 12);
    std::cout << day_sub.date << std::endl;

    return 0;
}
