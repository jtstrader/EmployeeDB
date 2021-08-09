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