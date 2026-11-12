#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
using namespace std;

// ================= DATABASE (SQLite) =================

class Database {
private:
    sqlite3* db;

public:
    Database(string filename) {
        db = NULL;

        if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
            cout << "Cannot open database: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            db = NULL;
            return;
        }

        createTables();
    }

    sqlite3* getHandle() {
        return db;
    }

    bool isOpen() {
        return db != NULL;
    }

    bool execute(string sql) {
        if (db == NULL)
            return false;

        char* error = NULL;

        if (sqlite3_exec(db, sql.c_str(), NULL, NULL, &error) != SQLITE_OK) {
            cout << "Database error: " << error << endl;
            sqlite3_free(error);
            return false;
        }
        return true;
    }

    void createTables() {
        execute("CREATE TABLE IF NOT EXISTS patients ("
                "id INTEGER PRIMARY KEY, name TEXT, age INTEGER, "
                "disease TEXT, type INTEGER, days INTEGER)");

        execute("CREATE TABLE IF NOT EXISTS doctors ("
                "id INTEGER PRIMARY KEY, name TEXT, specialization TEXT, "
                "fee REAL, type INTEGER, experience INTEGER)");

        execute("CREATE TABLE IF NOT EXISTS appointments ("
                "appt_id INTEGER PRIMARY KEY, patient_id INTEGER, "
                "doctor_id INTEGER, date TEXT, active INTEGER)");
    }

    ~Database() {
        if (db != NULL)
            sqlite3_close(db);
    }
};

// reads a TEXT column safely (NULL becomes empty string)
string columnText(sqlite3_stmt* st, int col) {
    const unsigned char* t = sqlite3_column_text(st, col);

    if (t == NULL)
        return "";
    return string((const char*)t);
}

// ---------- Patients ----------

void savePatients(Database &database, Patient* patients[], int count) {
    if (!database.isOpen())
        return;

    database.execute("BEGIN");
    database.execute("DELETE FROM patients");

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "INSERT INTO patients VALUES (?, ?, ?, ?, ?, ?)", -1, &st, NULL);

    for (int i = 0; i < count; i++) {
        string name = patients[i]->getName();
        string disease = patients[i]->getDisease();

        sqlite3_bind_int(st, 1, patients[i]->getId());
        sqlite3_bind_text(st, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(st, 3, patients[i]->getAge());
        sqlite3_bind_text(st, 4, disease.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(st, 5, patients[i]->getTypeCode());
        sqlite3_bind_int(st, 6, patients[i]->getDays());

        sqlite3_step(st);
        sqlite3_reset(st);
    }

    sqlite3_finalize(st);
    database.execute("COMMIT");
}

void loadPatients(Database &database, Patient* patients[], int &count, const int MAX) {
    if (!database.isOpen())
        return;

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "SELECT id, name, age, disease, type, days FROM patients ORDER BY id",
        -1, &st, NULL);

    while (count < MAX && sqlite3_step(st) == SQLITE_ROW) {
        Patient* p;

        if (sqlite3_column_int(st, 4) == 2)
            p = new InPatient();
        else
            p = new Patient();

        p->setData(sqlite3_column_int(st, 0),
                   columnText(st, 1),
                   sqlite3_column_int(st, 2),
                   columnText(st, 3),
                   sqlite3_column_int(st, 5));

        patients[count] = p;
        count++;
    }

    sqlite3_finalize(st);
}

// ---------- Doctors ----------

void saveDoctors(Database &database, Doctor* doctors[], int count) {
    if (!database.isOpen())
        return;

    database.execute("BEGIN");
    database.execute("DELETE FROM doctors");

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "INSERT INTO doctors VALUES (?, ?, ?, ?, ?, ?)", -1, &st, NULL);

    for (int i = 0; i < count; i++) {
        string name = doctors[i]->getName();
        string spec = doctors[i]->getSpecialization();

        sqlite3_bind_int(st, 1, doctors[i]->getId());
        sqlite3_bind_text(st, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 3, spec.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(st, 4, doctors[i]->getFee());
        sqlite3_bind_int(st, 5, doctors[i]->getTypeCode());
        sqlite3_bind_int(st, 6, doctors[i]->getExperience());

        sqlite3_step(st);
        sqlite3_reset(st);
    }

    sqlite3_finalize(st);
    database.execute("COMMIT");
}

void loadDoctors(Database &database, Doctor* doctors[], int &count, const int MAX) {
    if (!database.isOpen())
        return;

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "SELECT id, name, specialization, fee, type, experience FROM doctors ORDER BY id",
        -1, &st, NULL);

    while (count < MAX && sqlite3_step(st) == SQLITE_ROW) {
        Doctor* d;

        if (sqlite3_column_int(st, 4) == 2)
            d = new SeniorDoctor();
        else
            d = new Doctor();

        d->setData(sqlite3_column_int(st, 0),
                   columnText(st, 1),
                   columnText(st, 2),
                   (float)sqlite3_column_double(st, 3),
                   sqlite3_column_int(st, 5));

        doctors[count] = d;
        count++;
    }

    sqlite3_finalize(st);
}

// ---------- Appointments ----------

void saveAppointments(Database &database, Appointment* appts[], int count) {
    if (!database.isOpen())
        return;

    database.execute("BEGIN");
    database.execute("DELETE FROM appointments");

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "INSERT INTO appointments VALUES (?, ?, ?, ?, ?)", -1, &st, NULL);

    for (int i = 0; i < count; i++) {
        string date = appts[i]->getDate();

        sqlite3_bind_int(st, 1, appts[i]->getApptId());
        sqlite3_bind_int(st, 2, appts[i]->getPatientId());
        sqlite3_bind_int(st, 3, appts[i]->getDoctorId());
        sqlite3_bind_text(st, 4, date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(st, 5, appts[i]->isActive() ? 1 : 0);

        sqlite3_step(st);
        sqlite3_reset(st);
    }

    sqlite3_finalize(st);
    database.execute("COMMIT");
}

void loadAppointments(Database &database, Appointment* appts[], int &count) {
    if (!database.isOpen())
        return;

    sqlite3_stmt* st;
    sqlite3_prepare_v2(database.getHandle(),
        "SELECT appt_id, patient_id, doctor_id, date, active FROM appointments ORDER BY appt_id",
        -1, &st, NULL);

    int maxId = 0;

    while (count < MAX_APPT && sqlite3_step(st) == SQLITE_ROW) {
        Appointment* a = new Appointment();

        a->setData(sqlite3_column_int(st, 0),
                   sqlite3_column_int(st, 1),
                   sqlite3_column_int(st, 2),
                   columnText(st, 3),
                   sqlite3_column_int(st, 4) == 1);

        if (a->getApptId() > maxId)
            maxId = a->getApptId();

        appts[count] = a;
        count++;
    }

    // new appointments must get an ID bigger than all old ones
    Appointment::setNextId(maxId);

    sqlite3_finalize(st);
}

#endif
