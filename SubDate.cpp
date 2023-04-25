/**
  **************************** Project: SubDate.cpp ****************************
  * @file
  * Created by john on 23-4-21.
  * @author     john
  * @date       Apr 21, 2023
  * @brief      WriteHere
  * @note       WriteHere
  **************************** Project: SubDate.cpp ****************************
 */

#include <utility>

#include "MyDate.cpp"


namespace cpt_project_2 {
    class SubDate : public MyDate {
    public:
        [[maybe_unused]] int hour{}, minute{}, second{};

        SubDate(int year, std::string month, int day, int hour, int minute, int second);

        static void update_from_time_zone(SubDate *self, std::time_t time_zone);
    };

    // --- Start definite

    SubDate::SubDate(int year, std::string month, int day, int hour, int minute, int second) :
            MyDate(year, std::move(month), day) {
        auto self_zone = std::mktime(&this->time_struct);
        self_zone += hour * 60 * 60 + minute * 60 + second;
        update_from_time_zone(this, self_zone);
    }

    void SubDate::update_from_time_zone(SubDate *self, std::time_t time_zone) {
        std::stringstream ss;

        MyDate::update_from_time_zone(self, time_zone);

        ss << std::put_time(&(self->time_struct), "%Y-%b-%d %H:%M:%S");

        self->hour = self->time_struct.tm_hour;
        self->minute = self->time_struct.tm_min;
        self->second = self->time_struct.tm_sec;

        self->date = ss.str();
    }
}
