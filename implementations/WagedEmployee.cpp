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

std::ostream& operator << (std::ostream& stream, WagedEmployee& emp) {
    return (stream << 1 << std::endl << emp.getEmployeeId() << std::endl <<
            emp.getEmployeeName() << std::endl <<
            emp.getEmployeePosition() << std::endl <<
            emp.getEmployeeAge() << std::endl <<
            emp.getClearanceLevel() << std::endl <<
            emp.getPayGrade() << std::endl <<
            emp.getWage() << std::endl << std::endl);
}

std::istream& operator >> (std::istream& stream, WagedEmployee& emp) {
    std::string burn;
    stream>>emp.empdat.employee_id; 
    getline(stream, burn);
    getline(stream, burn); strncpy(emp.empdat.employee_name, burn.c_str(), 50);
    getline(stream, burn); strncpy(emp.empdat.employee_position, burn.c_str(), 50);
    stream>>emp.empdat.employee_age; getline(stream, burn);
    getline(stream, burn);

    if(burn == "NONE")
        emp.empdat.employee_clearance = NONE;
    else if(burn == "CONFIDENTIAL")
        emp.empdat.employee_clearance = CONFIDENTIAL;
    else if(burn == "SECRET")
        emp.empdat.employee_clearance = SECRET;
    else if(burn == "TOP_SECRET")
        emp.empdat.employee_clearance = TOP_SECRET;
    else
        emp.empdat.employee_clearance = NONE;
    getline(stream, burn); strncpy(emp.empdat.employee_pay_grade, burn.c_str(), 4);
    stream>>emp.wage; getline(stream, burn); // clear buffer
    return stream;
}