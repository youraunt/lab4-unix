//
// Created by bk on 4/30/22.
//

#ifndef UNIX_LAB_04_UNSUBMITTED_LAB4_H
#define UNIX_LAB_04_UNSUBMITTED_LAB4_H

#include <iomanip>
#include <iostream>



class Date {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

public:
    friend std::ostream &operator<<(std::ostream &os, const Date &date);

    [[nodiscard]]  int getYear() const {
        return year;
    }

    [[nodiscard]] int getMonth() const {
        return month;
    }

    [[nodiscard]] int getDay() const {
        return day;
    }

    [[nodiscard]] int getHour() const {
        return hour;
    }

    void setHour(int i) {
        Date::hour = i;
    }

    [[nodiscard]] int getMinute() const {
        return minute;
    }

    void setMinute(int i) {
        Date::minute = i;
    }

    [[nodiscard]] int getSecond() const {
        return second;
    }

    void setSecond(int i) {
        Date::second = i;
    }

    explicit Date(std::string str) {
        if (dateValidator(str)) {
            year = (int) strtol(&str[0], nullptr, 10);
            month = (int) strtol(&str[5], nullptr, 10);
            day = (int) strtol(&str[8], nullptr, 10);
            hour = (int) strtol(&str[11], nullptr, 10);
            minute = (int) strtol(&str[14], nullptr, 10);
            second = (int) strtol(&str[17], nullptr, 10);
        } else {
            throw std::invalid_argument("Invalid date");
        }
    }


    // YYYY-MM-DD
    static bool dateValidator(std::string date_string) {
        if (date_string[4] != '-' || date_string[7] != '-') return false;
        if (!isdigit(date_string[0]) || !isdigit(date_string[1]) || !isdigit(date_string[2]) || !isdigit(date_string[3])) return false;
        if (!isdigit(date_string[5]) || !isdigit(date_string[6])) return false;
        if (!isdigit(date_string[8]) || !isdigit(date_string[9])) return false;

        if (date_string.length() == 19 && (!isdigit(date_string[11]) || !isdigit(date_string[12]))) return false;

        auto year = (int) strtol(&date_string[0], nullptr, 10);
        auto month = (int) strtol(&date_string[5], nullptr, 10);
        auto day = (int) strtol(&date_string[8], nullptr, 10);

        if (year <= 0 || month <= 0 || day <= 0) return false;
        if (month > 12) return false;

        switch (month) {
            case 2:
                if (day > 29) return false;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                if (day > 31) return false;
                break;
            default:
                if (day > 30) return false;
                break;
        }
        return true;
    }



};

std::ostream &operator<<(std::ostream &os, const Date &date) {
    os << " year: " << date.year
       << " month: " << date.month
       << " day: " << date.day
       << " hour: " << date.hour
       << " minute: " << date.minute
       << " second: " << date.second;
    return os;
}

#define WHAT_IS(x) std::cerr << #x << " is " << (x) << std::endl;


///
/// \param d1
/// \param d2
/// \return
int diffDays(Date d1, Date d2) {
    auto d1m = (d1.getMonth() + 9) % 12;
    auto d1y = d1.getYear() - d1m / 10;
    auto d2m = (d2.getMonth() + 9) % 12;
    auto d2y = d2.getYear() - d2m / 10;
    auto days1 = 365 * d1y + d1y / 4 - d1y / 100 + d1y / 400 + (d1m * 306 + 5) / 10 + (d1.getDay() - 1);
    auto days2 = 365 * d2y + d2y / 4 - d2y / 100 + d2y / 400 + (d2m * 306 + 5) / 10 + (d2.getDay() - 1);
    return days2 - days1;
}


class lab4 {
private:
    std::tm tm{};
    int alarm_counter;
public:
    void setAlarmCounter(int i) {
        alarm_counter = i;
    }
    [[nodiscard]]  int getAlarmCounter() const  {
        return lab4::alarm_counter;
    }
    void iterateAlarmCounter(int i) {
        alarm_counter += i;
    }
    [[nodiscard]] static inline std::tm localtime_os(std::time_t timer) {
        std::tm local_t{};
#if defined(__unix__)
        localtime_r(&timer, &local_t);
#elif defined(_MSC_VER)
        localtime_s(&tm, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    tm = *std::localtime(&timer);
#endif
        return local_t;

    }

    // default = "YYYY-MM-DD HH:MM:SS"
    inline static std::string time_stamp(const std::string &fmt = "%F %T")  {
        auto tp = localtime_os(std::time(nullptr));
        char buf[64];
        return {buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &tp)};
    }

    lab4() {
        time_t t = time(0);
        localtime_r(&t, &tm);
        setAlarmCounter(0);
    }


};
class Now {
private:
    struct tm local_t{};
public:
    Now() {
        time_t t = time(0);
        localtime_r(&t, &local_t);
    }

    std::string getDate(const char *fmt) const {
        char out[1024];
        size_t result = strftime(out, sizeof out, fmt, &local_t);
        return std::string{out,
                           out + result};///Braced initializer avoids repeating the return type from the declaration
    }

    [[nodiscard]] std::string getDateString() const { return getDate("%F %A, %r, %Z "); }
};
#endif //UNIX_LAB_04_UNSUBMITTED_LAB4_H
