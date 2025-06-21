#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#define max 9999999
#define FILENAME "daftar_dokter.csv"
#define MAX_LINE 256
#define ABSOLUT 7

typedef enum { PAGI = 0, SIANG = 1, MALAM = 2 } ShiftType;

struct Date{
    int date;
    int month;
    int year;
};

struct Pelanggaran
{
    struct Doctor_data *choosen_doctor;
    bool Pelanggaran_max_shift;
    bool Pelanggaran_cuti;
    bool Pelanggaran_preferensi;
};


struct shift_slot{
    struct Date date;
    ShiftType shift;
    int assigned_amount;
    int assigned_doctor_ID[4];
    struct Pelanggaran List_pelanggaran[4];
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