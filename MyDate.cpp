/**
  **************************** Project: MyDate.cpp ****************************
  * Created by oh-it-works on 2023/4/14.
  * @file
  * @author     oh-it-works
  * @date       2023-04-14
  * @brief      TODO:BriefHere
  **************************** Project: MyDate.cpp ****************************
 */
#pragma once

#include <utility>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>


namespace cpt_project_2 {
    class MyDate {
    public:

#ifdef FEBRUARY_NOT_ALWAYS_28_DAYS
        bool USE_FEBRUARY_ALWAYS_28_DAYS = false;
#else
        bool USE_FEBRUARY_ALWAYS_28_DAYS = true;
#endif
        std::string date;
        std::string month;
        int day{};
        int year{};

        MyDate(int year, std::string month, int day);

        explicit MyDate(std::time_t time_zone);

        ~MyDate();

        MyDate operator+(int days);

        MyDate operator-(int days);

        int operator-(const MyDate &other_date);

        bool operator>(MyDate other_date);

        bool operator<(MyDate other_date);

        bool operator==(const MyDate &other_date);

        MyDate &operator=(const MyDate &from);

        const MyDate operator++(int);

        MyDate &operator++();

        static void update_from_time_zone(MyDate *self, std::time_t time_zone);

        std::tm time_struct{};

        [[nodiscard]] bool february_has_28_days() const;

        [[nodiscard]] bool february_29_passed_this_year() const;
    };

    // --- start Definite ---

    /**
     * create object from year, month, day
     * @param year : example: 2023
     * @param month : example: "Jun", "Apr"
     * @param day : example: 30
     */
    MyDate::MyDate(int year, std::string month, int day) {

        std::stringstream ss;
        std::stringstream ss_date;

        ss << year << "-" << month << "-" << day;
        ss.imbue(std::locale("en_US.utf-8"));
        ss >> std::get_time(&(this->time_struct), "%Y-%b-%d");  // Note: Clion on Linux 说这里有个 Error (下一行)
        // In template: reference to non-static member function must be called

        ss_date << std::put_time(&(this->time_struct), "%Y-%b-%d");

        this->year = year;
        this->month = std::move(month);
        this->day = day;
        this->date = ss_date.str();
    }

    /**
     * ~ to default
     */
    MyDate::~MyDate() = default;

    /**
     * return this->year is normal year or not
     * @return
     */
    bool MyDate::february_has_28_days() const {
        return not((this->year % 4 == 0) and (this->year % 100 != 0));
    }

    /**
     * Assignment operation, to be honest, I don’t understand it, anyway, I made a copy value.
     * @param from
     * @return
     */
    MyDate &MyDate::operator=(const MyDate &from) {
        if (this == &from) {
            // 给自己赋值
            return *this;
        }

        std::tm time_from = from.time_struct;
        auto time_zone = std::mktime(&time_from);
        MyDate::update_from_time_zone(this, time_zone);
        return *this;
    }

    /**
     * Add the specified number of days
     * @param days: date changed
     * @return
     */
    MyDate MyDate::operator+(int days) {
        auto self_zone = std::mktime(&this->time_struct);
        auto new_zone = self_zone + days * 24 * 60 * 60;
        auto result = MyDate(new_zone);

        if (not this->USE_FEBRUARY_ALWAYS_28_DAYS) {
            // Disabled the option to force February to be 28 days
            return result;
        }

        // Consider the February issue
        if (this->year == result.year) {  // in this year
            if ((not this->february_29_passed_this_year()) and result.february_29_passed_this_year()) {
                // February always counts as 28 days
                MyDate::update_from_time_zone(&result, new_zone + 24 * 60 * 60);
            }
        } else {  // 跨年了
            auto days_to_add = 0;
            days_to_add += not this->february_29_passed_this_year();
            for (int i = this->year + 1; i < result.year; i++) {
                days_to_add += (this->year % 4 == 0) and (this->year % 100 != 0);
            }
            MyDate::update_from_time_zone(&result, new_zone + 24 * 60 * 60 * days_to_add);
            if (result.february_29_passed_this_year()) {
                MyDate::update_from_time_zone(&result, new_zone + 24 * 60 * 60);
            }
        }

        return result;
    }

    /**
     * Subtract the specified number of days, in fact is *this + (-days)
     * @param days: number of day changed
     * @return
     */
    MyDate MyDate::operator-(int days) {
        return *this + (-days);
    }

    /**
     * Generate MyDate object from timestamp
     * @param time_zone: Timestamp, not taking leap seconds into account
     */
    MyDate::MyDate(std::time_t time_zone) {
        MyDate::update_from_time_zone(this, time_zone);
    }

    /**
     * Static method, update self with time_zone, calculate leap year normally
     * @param self : the object pinter to be updated
     * @param time_zone
     */
    void MyDate::update_from_time_zone(MyDate *self, std::time_t time_zone) {
        std::stringstream ss_month, ss_date;

        localtime_s(&self->time_struct, &time_zone);
        ss_month << std::put_time(&(self->time_struct), "%y");
        ss_date << std::put_time(&(self->time_struct), "%Y-%b-%d");

        self->year = self->time_struct.tm_year + 1900;
        self->month = ss_month.str();
        self->day = self->time_struct.tm_mday;
        self->date = ss_date.str();
    }

    /**
     * The post-position ++ operator calls the pre-position ++.
     * To be honest, I don’t know what is going on with this Warning.
     * @return
     */
    const MyDate MyDate::operator++(int) {
        auto self_zone = std::mktime(&this->time_struct);
        const auto copy = MyDate(self_zone);  // Copy self.

        ++(*this);

        return copy;
    }

    /**
     * pre-position ++
     * @return
     */
    MyDate &MyDate::operator++() {
        auto self_zone = std::mktime(&this->time_struct);
        self_zone += 24 * 60 * 60;  // time_zone increases the time of day
        if (this->USE_FEBRUARY_ALWAYS_28_DAYS and
            (not this->february_has_28_days()) and this->time_struct.tm_mon == 1 and this->day == 28) {
            // February 28th in leap year adds 1 day to February 29th, which should be "corrected" to March 1st.
            self_zone += 24 * 60 * 60;
        }
        // update self with MyDate::update_from_time_zone
        MyDate::update_from_time_zone(this, self_zone);
        return *this;
    }

    /**
     * Returns whether my_date has passed February 29 of my_date->year,
     * or returns False if my_date->year is an ordinary year
     * @return
     */
    bool MyDate::february_29_passed_this_year() const {
        return this->february_has_28_days() and  // There must be no ordinary years February 29
               this->time_struct.tm_mon > 1 or this->time_struct.tm_mon == 1 and this->day > 28;
    }

    bool MyDate::operator>(MyDate other_date) {
        auto self_zone = std::mktime(&this->time_struct);
        auto other_zone = std::mktime(&other_date.time_struct);
        return self_zone > other_zone;
    }

    bool MyDate::operator<(MyDate other_date) {
        auto self_zone = std::mktime(&this->time_struct);
        auto other_zone = std::mktime(&other_date.time_struct);
        return self_zone < other_zone;
    }

    bool MyDate::operator==(const MyDate &other_date) {
        std::tm time_from = other_date.time_struct;
        auto self_zone = std::mktime(&this->time_struct);
        auto other_zone = std::mktime(&time_from);
        return self_zone == other_zone;
    }

    int MyDate::operator-(const MyDate &other_date) {
        std::tm time_from = other_date.time_struct;
        auto self_zone = std::mktime(&this->time_struct);
        auto other_zone = std::mktime(&time_from);

        if (self_zone > other_zone) {
            int sub = (int) (self_zone - other_zone);
            return sub / (60 * 60 * 24);
        } else {
            int sub = (int) (other_zone - self_zone);
            return -sub / (60 * 60 * 24);
        }
    }
}
