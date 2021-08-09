CC=--std=c++17

all: client clean

client: client.cpp SalariedEmployee.o WagedEmployee.o Employee.o EMap.o
	c++ $(CC) -o EmployeeDB.exe client.cpp SalariedEmployee.o WagedEmployee.o Employee.o EMap.o

Employee.o: ./headers/Employee.h ./implementations/Employee.cpp
	c++ $(CC) -c ./implementations/Employee.cpp

WagedEmployee.o: ./headers/WagedEmployee.h ./implementations/WagedEmployee.cpp
	c++ $(CC) -c ./implementations/WagedEmployee.cpp

SalariedEmployee.o: ./headers/SalariedEmployee.h ./implementations/SalariedEmployee.cpp
	c++ $(CC) -c ./implementations/SalariedEmployee.cpp

EMap.o: ./headers/EMap.h ./implementations/EMap.cpp
	c++ $(CC) -c ./implementations/EMap.cpp

clean:
	rm *.o