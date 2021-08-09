#include "../headers/EMap.h"

EMap::EMap() {
    
}

EMap::EMap(const EMap& old_eMap) {
    *this = old_eMap;
}

EMap::EMap(const std::map<unsigned int, long>& old_map) {
    *this = old_map;
}

EMap::~EMap() {

}

void EMap::printMap() {
    EMap::iterator it = this->begin();
    std::cout<<"----EMap----"<<std::endl;
    for(; it != this->end(); ++it) {
        std::cout<<"- "<<it->first<<"  "<<it->second<<" -"<<std::endl;
    }
}

void EMap::setFileName(char* fileName) {
    this->fileName = fileName;
}

void EMap::load() {
    std::fstream load(fileName, std::ios::in | std::ios::binary);
    if(!load.is_open()) {
        load.close();
        std::fstream create(fileName, std::ios::out | std::ios::binary); create.close();
        return;
    }
    else {
        // load in data into map
        loadAllRecords(load);
        load.close();
    }
}

bool EMap::add_employee(WagedEmployee& emp) {

    int type = emp.getPayTypeInt();
    if(type != 1)
        return false;

    // add new data to temporary employee list
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    emp.setEmployeeId(this->currMaxId+1);
    if(this->find(emp.getEmployeeId()) != this->end()) {
        std::cout<<"EMap Error: duplicate non-UINT_MAX key. Exiting add_employee..."<<std::endl;
        return false;
    }

    EMap::iterator it = this->find(UINT_MAX);
    if(it != this->end()) {
        // remove UINT_MAX placeholder record and create new data for the map
        outFile.seekp(it->second);
        this->erase(it);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    }
    else {
        // insert new pair into map
        outFile.seekp(0, std::ios::end);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    } 

    bool active = true;
    outFile.write((char*)&type, sizeof(type));
    outFile.write((char*)&emp, sizeof(emp));
    outFile.write((char*)&active, sizeof(active));
    outFile.close(); // save data
    this->currMaxId++;
    return true; // assume return true == OK status
}

bool EMap::add_employee(SalariedEmployee& emp) {

    int type = emp.getPayTypeInt();
    if(type != 2)
        return false;

    // add new data to temporary employee list
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    emp.setEmployeeId(this->currMaxId+1);
    if(this->find(emp.getEmployeeId()) != this->end()) {
        std::cout<<"EMap Error: duplicate non-UINT_MAX key. Exiting add_employee..."<<std::endl;
        return false;
    }

    EMap::iterator it = this->find(UINT_MAX);
    if(it != this->end()) {
        // remove UINT_MAX placeholder record and create new data for the map
        outFile.seekp(it->second);
        this->erase(it);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    }
    else {
        // insert new pair into map
        outFile.seekp(0, std::ios::end);
        this->insert(std::pair<unsigned int, long>(emp.getEmployeeId(), outFile.tellp()));
    } 

    bool active = true;
    outFile.write((char*)&type, sizeof(type));
    outFile.write((char*)&emp, sizeof(emp));
    outFile.write((char*)&active, sizeof(active));
    outFile.close(); // save data
    this->currMaxId++;
    return true; // assume return true == OK status
}

bool EMap::delete_employee(unsigned int employee_id) {
    // set id to -1 and change bool handler to "false"
    std::fstream outFile(fileName, std::ios::in | std::ios::out | std::ios::binary);
    EMap::iterator it = this->find(employee_id);

    if(it == this->end()) {
        outFile.close();
        return false; // record not found
    }
    else {
        // set record id to UINT_MAX
        long offset = it->second;
        this->erase(it); // remove item from map
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset)); // reinsert with UINT_MAX key

        // go to offset and set bool value to false, indicating record is free
        long delOffset = gotoActiveVar(outFile, offset);
        bool active = false;
        outFile.seekp(active);
        outFile.write((char*)&active, sizeof(active));
        outFile.close();
        return true;
    }
}

bool EMap::read_employees_print() {
    for(EMap::iterator it=this->begin(); it!=this->end(); ++it) {
        if(it->first == UINT_MAX)
            continue;
        read_employee_print(it->first);
    }
}

bool EMap::read_employee_print(unsigned int employee_id) {
    EMap::iterator it = this->find(employee_id);
    if(it == this->end())
        return false;
    else {
        long offset = it->second;
        std::fstream inFile(this->fileName, std::ios::in | std::ios::binary);
        inFile.seekg(offset);
        
        int type;
        inFile.read((char*)&type, sizeof(type));
        if(type == 1) {
            // waged
            WagedEmployee we;
            inFile.read((char*)&we, sizeof(we));
            std::cout<<we<<std::endl;
        }
        else if(type == 2) {
            // salaried
            SalariedEmployee se;
            inFile.read((char*)&se, sizeof(se));
            std::cout<<se<<std::endl;
        }
        inFile.close();
    }
    return true;
}

bool EMap::read_employees_print_table() {
    std::fstream inFile(fileName, std::ios::in | std::ios::binary);
    std::cout<<"|"<<padStringCenter("ID", 8)<<"|"<<padStringCenter("Employee Name", 25)<<"|"<<padStringCenter("Position", 25)<<"|"<<padStringCenter("Clearance Level", 19)<<"|"<<std::endl;
    std::cout<<std::string(82, '-')<<std::endl;
    for(EMap::iterator it = this->begin(); it != this->end(); ++it) {
        if(it->first != UINT_MAX) {
            int type; inFile.seekg(it->second);
            inFile.read((char*)&type, sizeof(type));
            Employee* e;
            if(type == 0) continue;
            if(type == 1) {
                WagedEmployee we;
                inFile.read((char*)&we, sizeof(we));
                e = &we;
            }
            if(type == 2) {
                SalariedEmployee se;
                inFile.read((char*)&se, sizeof(se));
                e = &se;
            }
            std::cout<<"|"<<padStringLeft(std::to_string(e->getEmployeeId()), 8)<<"|"<<padStringCenter(e->getEmployeeName(), 25)<<"|"<<padStringCenter(e->getEmployeePosition(), 25)<<"|"<<padStringCenter(e->getClearanceLevel(), 19)<<"|"<<std::endl;
        }
    }
}

void EMap::loadAllRecords(std::fstream& inFile) { // assume offset 0
    int type;
    while(inFile.read((char*)&type, sizeof(int))) {
        if(type == 0) loadEmployeeRecord(inFile);
        if(type == 1) loadWageRecord(inFile);
        if(type == 2) loadSalaryRecord(inFile);
    }
}

void EMap::loadEmployeeRecord(std::fstream& inFile) {

}

void EMap::loadWageRecord(std::fstream& inFile) {
    WagedEmployee we; bool active; long offset = inFile.tellp(); offset-=sizeof(int);
    inFile.read((char*)&we, sizeof(we));
    inFile.read((char*)&active, sizeof(bool));
    if(active) {
        this->insert(std::pair<unsigned int, long>(we.getEmployeeId(), offset));
        if(we.getEmployeeId()>this->currMaxId)
            this->currMaxId = we.getEmployeeId();
    }
    else
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset));
}

void EMap::loadSalaryRecord(std::fstream& inFile) {
    SalariedEmployee se; bool active; long offset = inFile.tellp(); offset-=sizeof(int);
    inFile.read((char*)&se, sizeof(se));
    inFile.read((char*)&active, sizeof(bool));
    if(active) {
        this->insert(std::pair<unsigned int, long>(se.getEmployeeId(), offset));
        if(se.getEmployeeId()>this->currMaxId)
            this->currMaxId = se.getEmployeeId();
    }
    else
        this->insert(std::pair<unsigned int, long>(UINT_MAX, offset));
}

long EMap::gotoActiveVar(std::fstream& outFile, long offset) {
    outFile.seekg(offset);
    int type;
    outFile.read((char*)&type, sizeof(type));
    
    if(type == 0) {
        Employee e;
        outFile.read((char*)&e, sizeof(e));
    }
    else if(type == 1) {
        WagedEmployee we;
        outFile.read((char*)&we, sizeof(we));
    }  
    else if(type == 2) {
        SalariedEmployee se;
        outFile.read((char*)&se, sizeof(se));
    }

    // return where fstream now at "active" variable offset
    return outFile.tellg();
}

std::string EMap::padStringLeft(std::string text, int width) {
    if(text.length() < width) {
        std::stringstream ss;
        ss << std::setw(width) << text;
        return ss.str();
    }
    else
        return text;
}

std::string EMap::padStringCenter(std::string text, int width) {
    std::string repeat = std::string((width-text.length()) / 2, ' ');
    std::stringstream ss;
    ss << std::setw(width) << repeat + text + repeat;
    return ss.str(); 
}