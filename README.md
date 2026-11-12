# Hospital Management System

## Project Overview

The Hospital Management System is a menu-driven C++ project developed using Object-Oriented Programming (OOP) and SQLite.

It manages patients, doctors, appointments and bills, and stores all records permanently in a SQLite database.

## Project Modules

### Module 1: Register and Manage Patients

- Register out-patients and admitted (in-) patients.
- Display and search patient records.
- Update patient disease.
- Discharge / delete patient records.
- Store ID, Name, Age, Disease and Days Admitted.

### Module 2: Manage Doctor Records

- Add normal and senior doctors.
- Display and search doctor records.
- Update consultation fee.
- Delete doctor records.
- Store ID, Name, Specialization, Fee and Experience.

### Module 3: Book and Cancel Appointments

- Book an appointment for an existing patient and doctor.
- Cancel an appointment (record is kept as Cancelled).
- Display all appointments and search by Appointment ID.
- Duplicate appointments are not allowed.

### Module 4: Generate Patient Bills

- Add consultation fees of all active appointments.
- Add room charges for admitted patients (Rs. 1500 per day).
- Add medicine charges.
- 10% discount for senior citizens (age 60 or above).

## Technologies Used

- Language: C++
- Database: SQLite3
- Compiler: GCC / MinGW
- IDE: VS Code / Code::Blocks

## OOP Concepts

- Classes and Objects
- Encapsulation
- Inheritance (InPatient extends Patient, SeniorDoctor extends Doctor)
- Polymorphism (virtual functions and method overriding)
- Static Members (object counters and auto-increment IDs)
- Friend Functions
- Constructors and Virtual Destructors
- Dynamic Memory Allocation (new / delete)

## Project Structure

| File           | Purpose                                  |
| -------------- | ---------------------------------------- |
| main.cpp       | Main menu, connects all modules          |
| patient.h      | Module 1: Patient and InPatient classes  |
| doctor.h       | Module 2: Doctor and SeniorDoctor classes |
| appointment.h  | Module 3: Appointment class              |
| billing.h      | Module 4: Bill class                     |
| database.h     | SQLite connection, save and load         |

## Database Tables

| Table        | Columns                                                            |
| ------------ | ------------------------------------------------------------------ |
| patients     | id, type, name, age, disease, days                                 |
| doctors      | id, type, name, specialization, fee, experience                    |
| appointments | appt_id, patient_id, doctor_id, date, active                       |

The database file `hospital.db` is created automatically when the program runs for the first time. Records are loaded when the program starts and saved when you go back from a module menu.

## Time Complexity

| Operation                | Complexity |
| ------------------------ | ---------- |
| Register Patient / Add Doctor | O(n)  |
| Search Patient / Doctor  | O(n)       |
| Update Patient / Doctor  | O(n)       |
| Delete Patient / Doctor  | O(n)       |
| Display Records          | O(n)       |
| Book Appointment         | O(n)       |
| Cancel Appointment       | O(n)       |
| Generate Bill            | O(n)       |

Note: Registering and adding records is O(n) because the duplicate ID check searches the array.

## How to Run

Install SQLite first. On Linux: `sudo apt install libsqlite3-dev`. On Windows, download `sqlite3.c` and `sqlite3.h` from sqlite.org and keep them in the project folder.

Compile on Linux:

```
g++ main.cpp -o hospital -lsqlite3
```

Compile on Windows (MinGW):

```
gcc -c sqlite3.c -o sqlite3.o
g++ main.cpp sqlite3.o -o hospital
```

Run on Windows:

```
hospital.exe
```

Run on Linux:

```
./hospital
```

Note: Names, diseases and dates must be entered as a single word (for example Ravi_Sharma and 12-11-2026).

## Future Enhancements

- Use getline for full names with spaces.
- Direct SQL INSERT, UPDATE and DELETE instead of rewriting tables.
- Date validation for appointments.
- Search patients by disease.
- Login system for admin and receptionist.

## Conclusion

This project demonstrates the practical use of OOP concepts and SQLite database integration in C++. It provides basic hospital record management for patients, doctors, appointments and billing.

**Project Name:** Hospital Management System

**Language:** C++

**Project Type:** OOP and SQLite Database
