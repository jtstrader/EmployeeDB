#include "../headers/SalariedEmployee.h"

// constructors & destructor //
SalariedEmployee::SalariedEmployee() {
    this->emp_type = SALARIED;
}

SalariedEmployee::~SalariedEmployee() {

}

//// GETTERS ////
float SalariedEmployee::getSalary() {
    return this->salary;
}

float SalariedEmployee::getBiWeeklyPay() {
    return this->salary/26;
}
//// END GETTERS ////

//// SETTERS ////
void SalariedEmployee::setSalary(float salary) {
    this->salary = salary;
}
//// END SETTERS ////

//// OPERATORS ////
std::ostream& operator << (std::ostream& stream, SalariedEmployee& emp) {
    return (stream << 2 << std::endl << emp.getEmployeeId() << std::endl <<
            emp.getEmployeeName() << std::endl <<
            emp.getEmployeePosition() << std::endl <<
            emp.getEmployeeAge() << std::endl <<
            emp.getEmployeeClearanceLevel() << std::endl <<
            emp.getEmployeePayGrade() << std::endl <<
            emp.getSalary() << std::endl << std::endl);
}

std::istream& operator >> (std::istream& stream, SalariedEmployee& emp) {
    std::string burn;
    stream>>emp.empdat.employee_id; 
    getline(stream, burn);
    getline(stream, burn); strncpy(emp.empdat.employee_name, burn.c_str(), 49); emp.empdat.employee_name[49] = '\0';
    getline(stream, burn); strncpy(emp.empdat.employee_position, burn.c_str(), 49); emp.empdat.employee_position[49] = '\0';
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
    stream>>emp.salary; getline(stream, burn); // clear buffer
    return stream;
}
//// END OPERATORS ////