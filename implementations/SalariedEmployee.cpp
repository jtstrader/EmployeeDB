#include "../headers/SalariedEmployee.h"

SalariedEmployee::SalariedEmployee() {
    this->emp_type = SALARIED;
}

SalariedEmployee::~SalariedEmployee() {

}

// getters //
float SalariedEmployee::getSalary() {
    return this->salary;
}

float SalariedEmployee::getBiWeeklyPay() {
    return this->salary/26;
}

// setters //
void SalariedEmployee::setSalary(float salary) {
    this->salary = salary;
}

std::ostream& operator << (std::ostream& stream, SalariedEmployee& emp) {
    return (stream << 2 << std::endl << emp.getEmployeeId() << std::endl <<
            emp.getEmployeeName() << std::endl <<
            emp.getEmployeePosition() << std::endl <<
            emp.getEmployeeAge() << std::endl <<
            emp.getClearanceLevel() << std::endl <<
            emp.getPayGrade() << std::endl <<
            emp.getSalary() << std::endl << std::endl);
}

std::istream& operator >> (std::istream& stream, SalariedEmployee& emp) {
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
    stream>>emp.salary; getline(stream, burn); // clear buffer
    return stream;
}