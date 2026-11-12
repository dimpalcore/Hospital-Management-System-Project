#ifndef DOCTOR_H
#define DOCTOR_H

#include <iostream>
#include <string>
using namespace std;

// ================= MODULE 2: DOCTORS =================

class Doctor {
protected:
    int id;
    string name;
    string specialization;
    float fee;
    static int count;

public:
    Doctor() {
        id = 0;
        name = "";
        specialization = "";
        fee = 0;
        count++;
    }

    void input() {
        cout << "Enter Doctor ID: ";
        cin >> id;

        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter Specialization: ";
        cin >> specialization;

        cout << "Enter Consultation Fee: ";
        cin >> fee;

        if (fee < 0) {
            cout << "Invalid fee! Setting to 0.\n";
            fee = 0;
        }
    }

    void setFee(float fee) {
        if (fee >= 0)
            this->fee = fee;
        else
            cout << "Invalid fee!\n";
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    float getFee() {
        return fee;
    }

    static void showCount() {
        cout << "Total doctor objects: " << count << endl;
    }

    friend void showDoctor(Doctor d);

    virtual void display() {
        cout << "Doctor ID: " << id << endl;
        cout << "Name: Dr. " << name << endl;
        cout << "Specialization: " << specialization << endl;
        cout << "Consultation Fee: " << fee << endl;
    }

    string getSpecialization() {
        return specialization;
    }

    // Used by the database module: 1 = Doctor, 2 = Senior Doctor
    virtual int getTypeCode() {
        return 1;
    }

    virtual int getExperience() {
        return 0;
    }

    virtual void setData(int id, string name, string specialization, float fee, int experience) {
        this->id = id;
        this->name = name;
        this->specialization = specialization;
        this->fee = fee;
    }

    virtual ~Doctor() {
        count--;
    }
};

int Doctor::count = 0;

void showDoctor(Doctor d) {
    cout << "Name: Dr. " << d.name << endl;
    cout << "Specialization: " << d.specialization << endl;
}

// Senior doctors: same record plus years of experience
class SeniorDoctor : public Doctor {
private:
    int experience;

public:
    SeniorDoctor() {
        experience = 0;
    }

    void inputExperience() {
        cout << "Enter years of experience: ";
        cin >> experience;

        if (experience < 0) {
            cout << "Invalid! Setting to 0.\n";
            experience = 0;
        }
    }

    void display() override {
        Doctor::display();
        cout << "Experience: " << experience << " years\n";
    }

    int getTypeCode() override {
        return 2;
    }

    int getExperience() override {
        return experience;
    }

    void setData(int id, string name, string specialization, float fee, int experience) override {
        Doctor::setData(id, name, specialization, fee, experience);
        this->experience = experience;
    }
};

// Returns index of doctor with given id, or -1
int findDoctor(Doctor* doctors[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (doctors[i]->getId() == id)
            return i;
    }
    return -1;
}

void doctorMenu(Doctor* doctors[], int &count, const int MAX) {
    int choice;

    do {
        cout << "\n===== DOCTOR MANAGEMENT =====\n";
        cout << "1. Add Doctor\n";
        cout << "2. Display Doctors\n";
        cout << "3. Search Doctor\n";
        cout << "4. Update Fee\n";
        cout << "5. Delete Doctor\n";
        cout << "6. Show Total Objects\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (count == MAX) {
                    cout << "Doctor list is full!\n";
                    break;
                }

                char senior;
                cout << "Is this a senior doctor? (y/n): ";
                cin >> senior;

                Doctor* d;
                if (senior == 'y' || senior == 'Y')
                    d = new SeniorDoctor();
                else
                    d = new Doctor();

                d->input();

                // extra input only for senior doctors
                SeniorDoctor* sd = dynamic_cast<SeniorDoctor*>(d);
                if (sd != NULL)
                    sd->inputExperience();

                if (findDoctor(doctors, count, d->getId()) != -1) {
                    cout << "Doctor ID already exists!\n";
                    delete d;
                } else {
                    doctors[count] = d;
                    count++;
                    cout << "Doctor added successfully!\n";
                }
                break;
            }

            case 2:
                if (count == 0) {
                    cout << "No doctor records found!\n";
                }

                for (int i = 0; i < count; i++) {
                    cout << "\nDoctor " << i + 1 << endl;
                    doctors[i]->display();
                }
                break;

            case 3: {
                int id;
                cout << "Enter Doctor ID to search: ";
                cin >> id;

                int found = findDoctor(doctors, count, id);

                if (found == -1)
                    cout << "Doctor not found!\n";
                else
                    doctors[found]->display();
                break;
            }

            case 4: {
                int id;
                float fee;
                cout << "Enter Doctor ID: ";
                cin >> id;

                int found = findDoctor(doctors, count, id);

                if (found == -1) {
                    cout << "Doctor not found!\n";
                } else {
                    cout << "Enter new fee: ";
                    cin >> fee;
                    doctors[found]->setFee(fee);
                }
                break;
            }

            case 5: {
                int id;
                cout << "Enter Doctor ID to delete: ";
                cin >> id;

                int found = findDoctor(doctors, count, id);

                if (found == -1) {
                    cout << "Doctor not found!\n";
                } else {
                    delete doctors[found];

                    for (int i = found; i < count - 1; i++) {
                        doctors[i] = doctors[i + 1];
                    }

                    count--;
                    cout << "Doctor record deleted successfully!\n";
                }
                break;
            }

            case 6:
                Doctor::showCount();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

#endif
