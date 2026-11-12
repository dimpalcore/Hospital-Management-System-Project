#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include <string>
#include "patient.h"
#include "doctor.h"
using namespace std;

// ================= MODULE 3: APPOINTMENTS =================

const int MAX_APPT = 200;

class Appointment {
private:
    int apptId;
    int patientId;
    int doctorId;
    string date;
    bool active;
    static int nextId;

public:
    Appointment() {
        apptId = ++nextId;
        patientId = 0;
        doctorId = 0;
        date = "";
        active = true;
    }

    void setDetails(int patientId, int doctorId, string date) {
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->date = date;
    }

    int getApptId() {
        return apptId;
    }

    int getPatientId() {
        return patientId;
    }

    int getDoctorId() {
        return doctorId;
    }

    string getDate() {
        return date;
    }

    bool isActive() {
        return active;
    }

    void cancel() {
        active = false;
    }

    virtual void display() {
        cout << "Appointment ID: " << apptId << endl;
        cout << "Patient ID: " << patientId << endl;
        cout << "Doctor ID: " << doctorId << endl;
        cout << "Date: " << date << endl;
        cout << "Status: " << (active ? "Booked" : "Cancelled") << endl;
    }

    void setData(int apptId, int patientId, int doctorId, string date, bool active) {
        this->apptId = apptId;
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->date = date;
        this->active = active;
    }

    // after loading, next new appointment must get an ID bigger than all old ones
    static void setNextId(int n) {
        nextId = n;
    }

    virtual ~Appointment() {}
};

int Appointment::nextId = 0;

void appointmentMenu(Patient* patients[], int pCount,
                     Doctor* doctors[], int dCount,
                     Appointment* appts[], int &aCount) {
    int choice;

    do {
        cout << "\n===== APPOINTMENT MANAGEMENT =====\n";
        cout << "1. Book Appointment\n";
        cout << "2. Cancel Appointment\n";
        cout << "3. Display All Appointments\n";
        cout << "4. Search Appointment\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (aCount == MAX_APPT) {
                    cout << "Appointment list is full!\n";
                    break;
                }

                int pid, did;
                string date;

                cout << "Enter Patient ID: ";
                cin >> pid;

                if (findPatient(patients, pCount, pid) == -1) {
                    cout << "Patient not found! Register the patient first.\n";
                    break;
                }

                cout << "Enter Doctor ID: ";
                cin >> did;

                if (findDoctor(doctors, dCount, did) == -1) {
                    cout << "Doctor not found!\n";
                    break;
                }

                cout << "Enter Date (DD-MM-YYYY): ";
                cin >> date;

                bool duplicate = false;

                for (int i = 0; i < aCount; i++) {
                    if (appts[i]->isActive() &&
                        appts[i]->getPatientId() == pid &&
                        appts[i]->getDoctorId() == did &&
                        appts[i]->getDate() == date) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    cout << "Same appointment already booked!\n";
                } else {
                    Appointment* a = new Appointment();
                    a->setDetails(pid, did, date);
                    appts[aCount] = a;
                    aCount++;
                    cout << "Appointment booked! Your Appointment ID: "
                         << a->getApptId() << endl;
                }
                break;
            }

            case 2: {
                int id, found = -1;
                cout << "Enter Appointment ID to cancel: ";
                cin >> id;

                for (int i = 0; i < aCount; i++) {
                    if (appts[i]->getApptId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1)
                    cout << "Appointment not found!\n";
                else if (!appts[found]->isActive())
                    cout << "Appointment is already cancelled!\n";
                else {
                    appts[found]->cancel();
                    cout << "Appointment cancelled successfully!\n";
                }
                break;
            }

            case 3:
                if (aCount == 0) {
                    cout << "No appointments found!\n";
                }

                for (int i = 0; i < aCount; i++) {
                    cout << "\nAppointment " << i + 1 << endl;
                    appts[i]->display();
                }
                break;

            case 4: {
                int id, found = -1;
                cout << "Enter Appointment ID to search: ";
                cin >> id;

                for (int i = 0; i < aCount; i++) {
                    if (appts[i]->getApptId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1)
                    cout << "Appointment not found!\n";
                else
                    appts[found]->display();
                break;
            }

            case 0:
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

#endif
