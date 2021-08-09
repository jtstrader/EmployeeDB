#include "../headers/WagedEmployee.h"

WagedEmployee::WagedEmployee() {
    this->emp_type = WAGED;
}

WagedEmployee::WagedEmployee(const WagedEmployee& we) {
    *this = we;
} 

WagedEmployee::WagedEmployee(const EmployeeData& empdat) : Employee(empdat){

}

WagedEmployee::~WagedEmployee() {

}

// getters //
float WagedEmployee::getWage() {
    return this->wage;
}

float WagedEmployee::getWeeklyPay(int hoursWorked) {
    int overtime = 0;
    if(hoursWorked>40)
        overtime = hoursWorked-40;
    return overtime != 0 ? (40*wage)+(overtime*(1.5*wage)) : wage*hoursWorked;
}

float WagedEmployee::getBiWeeklyPay(int hoursWeek1, int hoursWeek2) {
    return getWeeklyPay(hoursWeek1)+getWeeklyPay(hoursWeek2);
}

// setters //
void WagedEmployee::setWage(float wage) {
    this->wage = wage;
}