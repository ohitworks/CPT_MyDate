/**
  **************************** Project: test.cpp ****************************
  * @file
  * Created by john on 23-4-21.
  * @author     john
  * @date       Apr 21, 2023
  * @brief      WriteHere
  * @note       From webpage https://en.cppreference.com/w/cpp/io/manip/get_time
  **************************** Project: test.cpp ****************************
 */


#include <iostream>
#include <sstream>
#include <iomanip>

int main()
{
    std::tm t = {};
    std::istringstream ss("2011-Feb-18 23:12:34");
    ss.imbue(std::locale("en_US.utf-8"));
    ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");

    if (ss.fail())
        std::cout << "Parse failed\n";
    else
        std::cout << std::put_time(&t, "%c") << '\n';
}
