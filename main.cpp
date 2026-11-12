#include <iostream>
#include <string>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "billing.h"
#include "database.h"
using namespace std;

int main() {
    const int MAX = 100;

    Patient* patients[MAX];
    Doctor* doctors[MAX];
    Appointment* appts[MAX_APPT];

    int pCount = 0;
    int dCount = 0;
    int aCount = 0;
    int choice;

    // open (or create) the SQLite database file
    Database database("hospital.db");

    if (!database.isOpen()) {
        cout << "Could not open database. Exiting...\n";
        return 1;
    }

    // load saved records from the database
    loadPatients(database, patients, pCount, MAX);
    loadDoctors(database, doctors, dCount, MAX);
    loadAppointments(database, appts, aCount);

    do {
        cout << "\n===== HOSPITAL MANAGEMENT SYSTEM =====\n";
        cout << "1. Patient Management\n";
        cout << "2. Doctor Management\n";
        cout << "3. Appointments (Book / Cancel)\n";
        cout << "4. Generate Patient Bill\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                patientMenu(patients, pCount, MAX);
                savePatients(database, patients, pCount);
                break;

            case 2:
                doctorMenu(doctors, dCount, MAX);
                saveDoctors(database, doctors, dCount);
                break;

            case 3:
                appointmentMenu(patients, pCount, doctors, dCount, appts, aCount);
                saveAppointments(database, appts, aCount);
                break;

            case 4:
                billingMenu(patients, pCount, doctors, dCount, appts, aCount);
                break;

            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    // free all dynamically allocated objects
    for (int i = 0; i < pCount; i++) {
        delete patients[i];
    }

    for (int i = 0; i < dCount; i++) {
        delete doctors[i];
    }

    for (int i = 0; i < aCount; i++) {
        delete appts[i];
    }

    return 0;
}
