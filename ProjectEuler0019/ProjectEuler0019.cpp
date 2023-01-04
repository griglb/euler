// You are given the following information, but you may prefer to do some research for yourself.
//    - 1 Jan 1900 was a Monday.
//    - Thirty days has September,
//      April, June and November.
//      All the rest have thirty-one,
//      Saving February alone,
//      Which has twenty-eight, rain or shine.
//      And on leap years, twenty-nine.
//    - A leap year occurs on any year evenly divisible by 4, but not on a century unless it is divisible by 400.
// How many Sundays fell on the first of the month during the twentieth century (1 Jan 1901 to 31 Dec 2000)?


#include <iostream>
#include <string>


constexpr int NormalMonths[]{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
constexpr int LeapMonths[]{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


bool is_leap_year(int year) {
    if (year % 4 > 0)
        return false;
    if ((year % 100 == 0) && (year % 400 != 0))
        return false;
    return true;
}


int length_of_year(int year) {
    return is_leap_year(year) ? 366 : 365;
}


int length_of_century() {
    int32_t sum{ 0 };
    for (int y = 1901; y < 2001; ++y)
        sum += length_of_year(y);

    return sum;
}


std::string first_day_of_year(int year) {
    const std::string DaysOfWeek[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

    int dow{ 0 };
    for (int y = 1900; y < year; ++y) {
        dow += length_of_year(y) % 7;
    }
    return DaysOfWeek[dow % 7];
}


std::string first_day_of_month(int year, int month) {
    const std::string DaysOfWeek[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

    int dow{ 0 };
    for (int y = 1900; y < year; ++y) {
        dow += length_of_year(y) % 7;
    }

    if (is_leap_year(year)) {
        for (int m = 0; m < month; ++m) {
            dow += LeapMonths[m];
        }
    }
    else {
        for (int m = 0; m < month; ++m) {
            dow += NormalMonths[m];
        }
    }
    return DaysOfWeek[dow % 7];
}


int sundays_per_year(int year) {
    int dow{ 0 };
    for (int y = 1900; y < year; ++y) {
        dow += length_of_year(y) % 7;
    }
    dow %= 7;

    dow += length_of_year(year);
    return dow / 7;
}


int main()
{
    std::cout << "Hello World!\n";

    int num_sundays{ 0 };
    for (int y = 1901; y < 2001; ++y) {
        std::cout << y << " " << length_of_year(y) << " " << first_day_of_year(y) << " " << sundays_per_year(y);
        for (int m = 0; m < 12; ++m) {
            std::cout << " " << first_day_of_month(y, m);
            num_sundays += first_day_of_month(y, m) == "Sun";
        }
        std::cout << std::endl;
    }

    std::cout << length_of_century() << " " << length_of_century() / 7.0 << std::endl;
    std::cout << num_sundays << std::endl;
}
