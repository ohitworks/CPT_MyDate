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
#ifdef FEBRUARY_NOT_ALWAYS_28_DAYS
    bool USE_FEBRUARY_ALWAYS_28_DAYS = false;
#else
    bool USE_FEBRUARY_ALWAYS_28_DAYS = true;
#endif

    class MyDate {
    public:
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
     * 从年月日生成对象
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
     * 将析构函数定义为 default
     */
    MyDate::~MyDate() = default;

    /**
     * 返回 this->year 是否是平年
     * @return
     */
    bool MyDate::february_has_28_days() const {
        return not((this->year % 4 == 0) and (this->year % 100 != 0));
    }

    /**
     * 赋值操作, 实话说没搞明白, 反正做了一个复制值
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
     * 增加指定天数
     * @param days: 变化的天数
     * @return
     */
    MyDate MyDate::operator+(int days) {
        auto self_zone = std::mktime(&this->time_struct);
        auto new_zone = self_zone + days * 24 * 60 * 60;
        auto result = MyDate(new_zone);

        if (not USE_FEBRUARY_ALWAYS_28_DAYS) {
            // 关闭了强制 2月为 28天的选项
            return result;
        }

        // 考虑 2月问题
        if (this->year == result.year) {  // 未跨年
            if ((not this->february_29_passed_this_year()) and result.february_29_passed_this_year()) {
                // 二月始终按照 28 天算
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
     * 减去指定天数, 其实就是 *this + (-days)
     * @param days: 变化的天数
     * @return
     */
    MyDate MyDate::operator-(int days) {
        return *this + (-days);
    }

    /**
     * 通过时间戳生成 MyDate 对象
     * @param time_zone: 时间戳, 不考虑闰秒
     */
    MyDate::MyDate(std::time_t time_zone) {
        MyDate::update_from_time_zone(this, time_zone);
    }

    /**
     * 静态方法, 用 time_zone 更新 self, 正常计算闰年
     * @param self
     * @param time_zone
     */
    void MyDate::update_from_time_zone(MyDate *self, std::time_t time_zone) {
        std::stringstream ss_month, ss_date;

        self->time_struct = *localtime(&time_zone);
        ss_month << std::put_time(&(self->time_struct), "%y");
        ss_date << std::put_time(&(self->time_struct), "%Y-%b-%d");

        self->year = self->time_struct.tm_year + 1900;
        self->month = ss_month.str();
        self->day = self->time_struct.tm_mday;
        self->date = ss_date.str();
    }

    /**
     * 后置 ++ 操作符, 调用了前置 ++. 实话说不知道这个 Warning 怎么回事.
     * @return
     */
    const MyDate MyDate::operator++(int) {
        auto self_zone = std::mktime(&this->time_struct);
        const auto copy = MyDate(self_zone);  // Copy self.

        ++(*this);

        return copy;
    }

    /**
     * 前置 ++ 操作符
     * @return
     */
    MyDate &MyDate::operator++() {
        auto self_zone = std::mktime(&this->time_struct);
        self_zone += 24 * 60 * 60;  // time_zone 增加一天时间
        if (USE_FEBRUARY_ALWAYS_28_DAYS and
            (not this->february_has_28_days()) and this->time_struct.tm_mon == 1 and this->day == 28) {
            // 闰年 2月28日 增加 1 天是 2月29日, 应当被 "修正" 为 3月1日
            self_zone += 24 * 60 * 60;
        }
        // 使用 MyDate::update_from_time_zone 更新自身
        MyDate::update_from_time_zone(this, self_zone);
        return *this;
    }

    /**
     * 返回 my_date 日期是否经过了 my_date->year 的 2月29日, 若 my_date->year 是平年, 则返回 False
     * @return
     */
    bool MyDate::february_29_passed_this_year() const {
        return this->february_has_28_days() and  // 平年一定没有 2月29日
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
            return - sub / (60 * 60 * 24);
        }
    }
}
