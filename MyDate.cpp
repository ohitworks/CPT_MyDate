/**
  **************************** Project: MyDate.cpp ****************************
  * Created by TODO:AuthorHere on 2023/4/14.
  * @file
  * @author     TODO:AuthorHere
  * @date       2023-04-14
  * @brief      TODO:BriefHere
  **************************** Project: MyDate.cpp ****************************
 */
#pragma once

#include <utility>
#include <sstream>
#include <string>
#include <ctime>


namespace cpt_project_2 {

    class MyDate {
    public:
        std::string month;
        int day;
        int year;

        MyDate(int year, std::string month, int day);

        explicit MyDate(std::time_t time_zone);

        ~MyDate();

        MyDate operator+(int days);
        MyDate operator-(int days);

        std::tm time_struct{};
    private:

        [[nodiscard]] bool february_has_28_days() const;
    };

    // --- start Definite ---

    /**
     * 从年月日生成对象
     * @param year : example: 2023
     * @param month : example: "Jun", "Apr"
     * @param day : example: 30
     */
    MyDate::MyDate(int year, std::string month, int day) {

        std::stringstream ss;

        ss << year << "-" << month << "-" << day;

        strptime(ss.str().c_str(), "%Y-%b-%d", &(this->time_struct));

        this->year = year;
        this->month = std::move(month);
        this->day = day;
    }

    MyDate::~MyDate() = default;

    bool MyDate::february_has_28_days() const {
        return not((this->year % 4 == 0) and (this->year % 100 != 0));
    }

    /**
     * 增加指定天数, 注意二月始终按照 28 天算
     * @param days
     * @return
     */
    MyDate MyDate::operator+(int days) {
        auto self_zone = std::mktime(&this->time_struct);
        auto new_zone = self_zone + days * 24 * 60 * 60;

        return MyDate(new_zone);
    }

    MyDate MyDate::operator-(int days) {
        return *this + (-days);
    }

    /**
     * 通过时间戳生成 MyDate 对象
     * @param time_zone: 时间戳, 不考虑闰秒
     * @note 特别处理了 二月 的情况(题目要求 2 月为 28 天)
     */
    MyDate::MyDate(std::time_t time_zone) {
        char char_month[3] = {0};

        this->time_struct = *localtime(&time_zone);
        strftime(char_month, 3, "%y", &this->time_struct);
        this->year = this->time_struct.tm_year + 1900;
        this->month = std::string(char_month);
        this->day = this->time_struct.tm_mday;

        if (this->february_has_28_days()) {
            // 输入了一个平年
            return;
        }
        // 闰年
        if (this->time_struct.tm_mon == 0) {
            // 一月份
            return;
        } else if (this->time_struct.tm_mon == 1) {
            // 二月份
            if (this->day <= 28) {
                return;
            }
        } else {
            // 三到十二月
        }

        time_zone += 24 * 60 * 60;  // time_zone 增加一天时间
        this->time_struct = *localtime(&time_zone);
        strftime(char_month, 3, "%y", &this->time_struct);
        this->year = this->time_struct.tm_year + 1900;
        this->month = std::string(char_month);
        this->day = this->time_struct.tm_mday;
    }
}
