#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <string>
using namespace std;

// ================= MODULE 1: PATIENTS =================

class Patient {
protected:
    int id;
    string name;
    int age;
    string disease;
    static int count;

public:
    Patient() {
        id = 0;
        name = "";
        age = 0;
        disease = "";
        count++;
    }

    virtual void input() {
        cout << "Enter Patient ID: ";
        cin >> id;

        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter Age: ";
        cin >> age;

        if (age <= 0 || age > 120) {
            cout << "Invalid age! Enter 1 to 120.\n";
            age = 0;
        }

        cout << "Enter Disease: ";
        cin >> disease;
    }

    void setDisease(string disease) {
        this->disease = disease;
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    int getAge() {
        return age;
    }

    static void showCount() {
        cout << "Total patient objects: " << count << endl;
    }

    friend void showPatient(Patient p);

    virtual void display() {
        cout << "Patient ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Disease: " << disease << endl;
        cout << "Type: " << getType() << endl;
    }

    // Overridden by InPatient
    virtual float getRoomCharges() {
        return 0;
    }

    virtual string getType() {
        return "Out-Patient";
    }

    string getDisease() {
        return disease;
    }

    // Used by the database module: 1 = Out-Patient, 2 = In-Patient
    virtual int getTypeCode() {
        return 1;
    }

    virtual int getDays() {
        return 0;
    }

    virtual void setData(int id, string name, int age, string disease, int days) {
        this->id = id;
        this->name = name;
        this->age = age;
        this->disease = disease;
    }

    virtual ~Patient() {
        count--;
    }
};

int Patient::count = 0;

void showPatient(Patient p) {
    cout << "Name: " << p.name << endl;
    cout << "Disease: " << p.disease << endl;
}

// Admitted patient: stays in hospital, so room charges apply
class InPatient : public Patient {
private:
    int days;
    float ratePerDay;

public:
    InPatient() {
        days = 0;
        ratePerDay = 1500;
    }

    void input() override {
        Patient::input();

        cout << "Enter number of days admitted: ";
        cin >> days;

        if (days <= 0) {
            cout << "Invalid days! Setting to 1.\n";
            days = 1;
        }
    }

    float getRoomCharges() override {
        return days * ratePerDay;
    }

    int getTypeCode() override {
        return 2;
    }

    int getDays() override {
        return days;
    }

    void setData(int id, string name, int age, string disease, int days) override {
        Patient::setData(id, name, age, disease, days);
        this->days = days;
    }

    string getType() override {
        return "In-Patient (Admitted)";
    }

    void display() override {
        Patient::display();
        cout << "Days Admitted: " << days << endl;
        cout << "Room Charges: " << getRoomCharges() << endl;
    }
};

// Returns index of patient with given id, or -1
int findPatient(Patient* patients[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (patients[i]->getId() == id)
            return i;
    }
    return -1;
}

void patientMenu(Patient* patients[], int &count, const int MAX) {
    int choice;

    do {
        cout << "\n===== PATIENT MANAGEMENT =====\n";
        cout << "1. Register Patient\n";
        cout << "2. Display Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Update Disease\n";
        cout << "5. Discharge / Delete Patient\n";
        cout << "6. Show Total Objects\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (count == MAX) {
                    cout << "Patient list is full!\n";
                    break;
                }

                int type;
                cout << "1. Out-Patient  2. In-Patient (Admitted): ";
                cin >> type;

                Patient* p;
                if (type == 2)
                    p = new InPatient();
                else
                    p = new Patient();

                p->input();

                if (findPatient(patients, count, p->getId()) != -1) {
                    cout << "Patient ID already exists!\n";
                    delete p;
                } else {
                    patients[count] = p;
                    count++;
                    cout << "Patient registered successfully!\n";
                }
                break;
            }

            case 2:
                if (count == 0) {
                    cout << "No patient records found!\n";
                }

                for (int i = 0; i < count; i++) {
                    cout << "\nPatient " << i + 1 << endl;
                    patients[i]->display();
                }
                break;

            case 3: {
                int id;
                cout << "Enter Patient ID to search: ";
                cin >> id;

                int found = findPatient(patients, count, id);

                if (found == -1)
                    cout << "Patient not found!\n";
                else
                    patients[found]->display();
                break;
            }

            case 4: {
                int id;
                string disease;
                cout << "Enter Patient ID: ";
                cin >> id;

                int found = findPatient(patients, count, id);

                if (found == -1) {
                    cout << "Patient not found!\n";
                } else {
                    cout << "Enter new disease: ";
                    cin >> disease;
                    patients[found]->setDisease(disease);
                    cout << "Record updated!\n";
                }
                break;
            }

            case 5: {
                int id;
                cout << "Enter Patient ID to delete: ";
                cin >> id;

                int found = findPatient(patients, count, id);

                if (found == -1) {
                    cout << "Patient not found!\n";
                } else {
                    delete patients[found];

                    for (int i = found; i < count - 1; i++) {
                        patients[i] = patients[i + 1];
                    }

                    count--;
                    cout << "Patient record deleted successfully!\n";
                }
                break;
            }

            case 6:
                Patient::showCount();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

#endif
