#include <iostream>
#include <csignal>
#include <atomic>
#include <cstring>


#include "LAB4.h"

volatile int continue_program = 1;
char ch;
void signal_handle(int);

/// @brief Handles unusable input
void inline unknownInput(int signal) {
    std::cout << "Error?! Please, try again: " << std::endl;
    ch = '\0';
    signal_handle(signal);

}

void sigalrm_handle(int signal) {
    if (signal != SIGALRM) fprintf(stderr, "Caught wrong signal: %d\n", signal);
}

void safe_sleep(int sec) {
    struct sigaction sa{};
    sigset_t mask{};
    sa.sa_handler = &sigalrm_handle;
    sa.sa_flags = SA_RESETHAND;
    sigfillset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, nullptr);
    sigprocmask(0, nullptr, &mask);
    sigdelset(&mask, SIGALRM);
    alarm(sec);
    sigsuspend(&mask);

}

void signal_handle(int signal) {
    ch = '\0';
    Now now_date;
    lab4 local;
    struct tm tm{};
    std::string temp = now_date.getDate("%Y-%m-%d");
    auto now_storage = temp.data();

    strptime(now_storage, "%Y-%m-%d", &tm);
    time_t t3 = mktime(&tm);


    std::string user_date = "2022-07-04";

    auto user_date_cstring = user_date.data();
    strptime(user_date_cstring, "%Y-%m-%d", &tm);
    time_t t4 = mktime(&tm);
    const char *signal_name;

    std::string ds2 = "2022-07-04";
    Date date(user_date_cstring);
    Date date_one(now_storage);
    double days = diffDays(date_one, date);
    auto sec = difftime(t4, t3);
    auto hour = days * 24;
    auto minutes = hour * 60;
    auto seconds = minutes * 60;
    switch (signal) {
        case SIGINT:
            printf("Do you want to continue: (y)es or (n)o?");
            std::cin.clear();
            std::cin.ignore(INT_MAX);
            ch = (char)getchar();
            ch = (char) tolower(ch);
            if (ch == 'y' || ch == 'n') {
                std::cout << "\nCurrent date and time: "
                          << lab4::time_stamp()
                          << "\nDifference In: \n"
                          << std::fixed << "\tDays : "
                          << days << '\n'
                          << std::fixed << "\tHours: "
                          << hour << '\n'
                          << std::fixed << "\tMinutes: "
                          << minutes << '\n'
                          << std::fixed << "\tSeconds: "
                          << seconds << '\n'
                          << "\tdifftime() seconds: "
                          << sec << std::endl;
                local.iterateAlarmCounter(1);

            } else { unknownInput(signal); }
            if (ch == 'y') return; else if (ch == 'n')continue_program = 0;

            [[fallthrough]];
        case SIGHUP:
            signal_name = "SIGHUP";
            break;
        default:
            fprintf(stderr, "What signal is this: %d\n", signal);
            return;
    }
}


int main() {

    Now now_date;

    std::string user_date;

    struct tm tm{};

    std::string temp = now_date.getDate("%Y-%m-%d");
    auto now_storage = temp.data();

    strptime(now_storage, "%Y-%m-%d", &tm);
    time_t t3 = mktime(&tm);


    user_date = "2022-12-25";

    auto user_date_cstring = user_date.data();
    strptime(user_date_cstring, "%Y-%m-%d", &tm);
    time_t t4 = mktime(&tm);

    lab4 local_time;
    struct sigaction sa{};
    sa.sa_handler = &signal_handle;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGHUP, &sa, nullptr) == -1) perror("Error: cannot handle SIGHUP");
    if (sigaction(SIGINT, &sa, nullptr) == -1) perror("Error: cannot handle SIGINT");
    if (signal(SIGINT, signal_handle) == SIG_ERR) fprintf(stderr, R"(Incorrect signal: %d)", signal);
    if (signal(SIGALRM, signal_handle) == SIG_ERR) std::cerr << "\nUnable to catch signal alarm\n";
    std::string ds1 = lab4::time_stamp();
    while (continue_program) {
        auto x = local_time.getAlarmCounter();
        printf("\n%d by SIGALRM:\n", x);
        if (std::string ds2 = "2022-12-25"; Date::dateValidator(ds1) && Date::dateValidator(ds2)) {
            if (!Date::dateValidator(user_date)) std::cout << "Invalid date, man!" << std::endl;
            else {
                Date date(user_date_cstring);
                Date date_one(now_storage);
                double days = diffDays(date_one, date);
                auto sec = difftime(t4, t3);
                auto hour = days * 24;
                auto minutes = hour * 60;
                auto seconds = minutes * 60;
                std::cout << "Current date and time: " << lab4::time_stamp()
                          << "\nDifference In: \n"
                          << std::fixed << "\tDays : "
                          << days << '\n'
                          << std::fixed << "\tHours: "
                          << hour << '\n'
                          << std::fixed << "\tMinutes: "
                          << minutes << '\n'
                          << std::fixed << "\tSeconds: "
                          << seconds << '\n'
                          << "\tdifftime() seconds: "
                          << sec << std::endl;
            }

            local_time.iterateAlarmCounter(1);
        } else std::cout << "Dates are invalid.\n";
        safe_sleep(3);
    }

    return 0;
}