#ifndef COMMON_H
#define COMMON_H

#define max 9999999
#define FILENAME "daftar_dokter.csv"
#define MAX_LINE 256

typedef enum { PAGI = 0, SIANG = 1, MALAM = 2 } ShiftType;

struct Date{
    int date;
    int month;
    int year;
};

struct shift_slot{
    struct Date date;
    ShiftType shift;
    int assigned_amount;
    int assigned_doctor_ID[10];
};

struct Doctor_data{
    int ID;
    char name[50];
    int maxShiftsPerWeek;
    int prefersShift[3]; // 0: pagi, 1: siang, 2: malam
    int restDay;
    int assignedShiftsPerWeek[5];
    int totalAssignedShifts;
    struct Doctor_data *next;
    struct Doctor_data *prev;
};


#endif