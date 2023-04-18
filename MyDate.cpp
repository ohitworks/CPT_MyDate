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
        int day{};
        int year{};

        MyDate(int year, std::string month, int day);

        explicit MyDate(std::time_t time_zone);

        ~MyDate();

        MyDate operator+(int days);

        MyDate operator-(int days);

        MyDate &operator=(const MyDate &from);

        const MyDate operator++(int);
        MyDate &operator++();

        static void update_from_time_zone(MyDate *self, std::time_t time_zone);

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
        MyDate::update_from_time_zone(this, time_zone);

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
        MyDate::update_from_time_zone(this, time_zone);
    }

    /**
     * 静态方法, 用 time_zone 更新 self, 正常计算闰年
     * @param self
     * @param time_zone
     */
    void MyDate::update_from_time_zone(MyDate *self, std::time_t time_zone) {
        char char_month[3] = {0};

        self->time_struct = *localtime(&time_zone);
        strftime(char_month, 3, "%y", &self->time_struct);
        self->year = self->time_struct.tm_year + 1900;
        self->month = std::string(char_month);
        self->day = self->time_struct.tm_mday;
    }

    const MyDate MyDate::operator++(int) {
        auto self_zone = std::mktime(&this->time_struct);
        const auto ret = MyDate(self_zone);

        self_zone += 24 * 60 * 60;  // time_zone 增加一天时间
        MyDate::update_from_time_zone(this, self_zone);
        if (not(this->february_has_28_days())) {
            // 输入了一个润年
            if (
                    (this->time_struct.tm_mon >= 2) or  // 三至十二月份
                    (this->time_struct.tm_mon == 1 and this->day > 28)
            ) {
                self_zone += 24 * 60 * 60;  // time_zone 增加一天时间
                MyDate::update_from_time_zone(this, self_zone);
            }
        }
        return ret;
    }

    MyDate &MyDate::operator++() {
        auto self_zone = std::mktime(&this->time_struct);
        self_zone += 24 * 60 * 60;  // time_zone 增加一天时间
        MyDate::update_from_time_zone(this, self_zone);
        if (not(this->february_has_28_days())) {
            // 输入了一个润年
            if (
                    (this->time_struct.tm_mon >= 2) or  // 三至十二月份
                    (this->time_struct.tm_mon == 1 and this->day > 28)
                    ) {
                self_zone += 24 * 60 * 60;  // time_zone 增加一天时间
                MyDate::update_from_time_zone(this, self_zone);
            }
        }
        return *this;
    }

}
