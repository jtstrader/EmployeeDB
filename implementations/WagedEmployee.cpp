#include "../headers/WagedEmployee.h"

// constructors & destructor //
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

//// GETTERS ////
float WagedEmployee::getWage() {
    return this->wage;
}

// calculate weekly pay w/ overtime
float WagedEmployee::getWeeklyPay(int hoursWorked) {
    int overtime = 0;
    if(hoursWorked>40)
        overtime = hoursWorked-40;
    return overtime != 0 ? (40*wage)+(overtime*(1.5*wage)) : wage*hoursWorked;
}

// sum of 2 weeks of pay, need two separate variables to calculate overtime
float WagedEmployee::getBiWeeklyPay(int hoursWeek1, int hoursWeek2) {
    return getWeeklyPay(hoursWeek1)+getWeeklyPay(hoursWeek2);
}
//// END GETTERS ////

//// SETTERS ////
void WagedEmployee::setWage(float wage) {
    this->wage = wage;
}
//// END SETTERS ////

//// OPERATORS ////
std::ostream& operator << (std::ostream& stream, WagedEmployee& emp) {
    return (stream << 1 << std::endl << emp.getEmployeeId() << std::endl <<
            emp.getEmployeeName() << std::endl <<
            emp.getEmployeePosition() << std::endl <<
            emp.getEmployeeAge() << std::endl <<
            emp.getEmployeeClearanceLevel() << std::endl <<
            emp.getEmployeePayGrade() << std::endl <<
            emp.getWage() << std::endl << std::endl);
}

std::istream& operator >> (std::istream& stream, WagedEmployee& emp) {
    std::string burn;
    stream>>emp.empdat.employee_id; 
    getline(stream, burn);
    getline(stream, burn); strncpy(emp.empdat.employee_name, burn.c_str(), 49); emp.empdat.employee_name[49] = '\0';
    getline(stream, burn); strncpy(emp.empdat.employee_position, burn.c_str(), 50); emp.empdat.employee_position[49] = '\0';
    stream>>emp.empdat.employee_age; getline(stream, burn);
    getline(stream, burn);

    if(burn == "NONE")
        emp.empdat.employee_clearance = NONE;
    else if(burn == "CONFIDENTIAL")
        emp.empdat.employee_clearance = CONFIDENTIAL;
    else if(burn == "SECRET")
        emp.empdat.employee_clearance = SECRET;
    else if(burn == "TOP SECRET")
        emp.empdat.employee_clearance = TOP_SECRET;
    else
        emp.empdat.employee_clearance = NONE;
    getline(stream, burn); strncpy(emp.empdat.employee_pay_grade, burn.c_str(), 4); emp.empdat.employee_pay_grade[4] = '\0';
    stream>>emp.wage; getline(stream, burn); // clear buffer
    return stream;
}
//// END OPERATORS ////