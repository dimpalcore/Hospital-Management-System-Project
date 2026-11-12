#ifndef BILLING_H
#define BILLING_H

#include <iostream>
#include <string>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
using namespace std;

// ================= MODULE 4: BILLING =================

class Bill {
private:
    int billNo;
    string patientName;
    float consultation;
    float room;
    float medicine;
    float discount;
    float total;
    static int nextBillNo;

public:
    Bill() {
        billNo = ++nextBillNo;
        patientName = "";
        consultation = 0;
        room = 0;
        medicine = 0;
        discount = 0;
        total = 0;
    }

    void setDetails(string patientName, float consultation, float room, float medicine, int age) {
        this->patientName = patientName;
        this->consultation = consultation;
        this->room = room;
        this->medicine = medicine;

        float subtotal = consultation + room + medicine;

        // 10% senior citizen discount (age 60+)
        if (age >= 60)
            discount = subtotal * 0.10;
        else
            discount = 0;

        total = subtotal - discount;
    }

    float getTotal() {
        return total;
    }

    virtual void display() {
        cout << "\n========== HOSPITAL BILL ==========\n";
        cout << "Bill No: " << billNo << endl;
        cout << "Patient: " << patientName << endl;
        cout << "-----------------------------------\n";
        cout << "Consultation Fees : " << consultation << endl;
        cout << "Room Charges      : " << room << endl;
        cout << "Medicine Charges  : " << medicine << endl;
        cout << "Discount          : -" << discount << endl;
        cout << "-----------------------------------\n";
        cout << "TOTAL AMOUNT      : " << total << endl;
        cout << "===================================\n";
    }

    virtual ~Bill() {}
};

int Bill::nextBillNo = 0;

void billingMenu(Patient* patients[], int pCount,
                 Doctor* doctors[], int dCount,
                 Appointment* appts[], int aCount) {
    int choice;

    do {
        cout << "\n===== BILLING =====\n";
        cout << "1. Generate Patient Bill\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                cout << "Enter Patient ID: ";
                cin >> id;

                int p = findPatient(patients, pCount, id);

                if (p == -1) {
                    cout << "Patient not found!\n";
                    break;
                }

                // add fees of all active (booked) appointments of this patient
                float consultation = 0;
                int visits = 0;

                for (int i = 0; i < aCount; i++) {
                    if (appts[i]->isActive() && appts[i]->getPatientId() == id) {
                        int d = findDoctor(doctors, dCount, appts[i]->getDoctorId());
                        if (d != -1) {
                            consultation += doctors[d]->getFee();
                            visits++;
                        }
                    }
                }

                if (visits == 0)
                    cout << "Note: No active appointments found for this patient.\n";

                float medicine;
                cout << "Enter medicine charges: ";
                cin >> medicine;

                if (medicine < 0) {
                    cout << "Invalid amount! Setting to 0.\n";
                    medicine = 0;
                }

                // polymorphism: In-Patient returns room charges, Out-Patient returns 0
                float room = patients[p]->getRoomCharges();

                Bill b;
                b.setDetails(patients[p]->getName(), consultation, room, medicine, patients[p]->getAge());
                b.display();
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
