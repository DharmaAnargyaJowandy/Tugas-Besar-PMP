#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "Schedule.h"

bool get_integer_input(const char* prompt, int* result) {
    int scanf_result;
    int c;

    printf("%s", prompt);
    scanf_result = scanf("%d", result); 

    while ((c = getchar()) != '\n' && c != EOF); 

    if (scanf_result == 1) {
        return true; 
    } else {
        printf("Input tidak valid! Harap masukkan angka saja.\n");
        return false; 
    }
}

void generate_schedule(struct shift_slot *slot, struct Doctor_data *front ){
    int index = 0;
    for(int i = 1; i < 31; i++){
        for(int j = 0; j < 3; j++){
            slot[index].shift = j;
            slot[index].assigned_amount = 0;
            slot[index].date.date = i;
            memset(slot[index].List_pelanggaran,0,sizeof(slot[index].List_pelanggaran));
            for (int k = 0; k < 4; k++) {
                slot[index].assigned_doctor_ID[k] = 0; 
            }
            index++;
        }
    }

    struct Doctor_data *temp = front;
    while (temp)
    {
        temp ->totalAssignedShifts = 0;
        for(int i = 0; i< 5; i++){
            temp ->assignedShiftsPerWeek[i] = 0;
        }

        temp = temp -> next;
    }
    
}

void assign_doctor(struct shift_slot *slots, struct Doctor_data *front ){


    if (front == NULL){
        printf("Daftar dokter kosong, tidak dapat melakukan penjadwalan \n");
        return;
    }

    // pointer untuk melacak titik awal pencarian dokter 
    struct Doctor_data *search_start = front;

    for(int i = 0; i < 90; i++){
        struct shift_slot *slot = &slots[i];
        int week = (slot -> date.date) / 7;


        for( int amount = 0; amount < 4; amount++){
            int minshift = max;
            struct Doctor_data *choosen_doctor_ptr = NULL;
            struct Doctor_data *temp = search_start;

            int count = 0; //agar tidak out of range
            struct Doctor_data *counter_pointer = front;

            //melakukan iterasi unutk melihat jumlah dokter dari titik awal 
            while (counter_pointer)
            {
                count++;
                counter_pointer = counter_pointer -> next;
            }
            
            //iterasi sebanyak jumlah dokter dari titik pencarian
            for(int j = 0; j < count; j++)
            {
                if((temp ->prefersShift[slot -> shift] == 1) 
                    && ((temp->assignedShiftsPerWeek[week]) < (temp->maxShiftsPerWeek))
                    && check_assigned_status(temp ->ID,slots, i) 
                    && (slot -> date.date != temp ->restDay)
                    &&(!prevent_duplicate(temp->ID, slot))){
                    if(temp -> assignedShiftsPerWeek[week] < minshift){
                        choosen_doctor_ptr = temp;
                        minshift = temp -> assignedShiftsPerWeek[week];
                    }
                }

                temp = temp -> next;

                //agar kembali ke head apabila telah mencapai akhir 
                if(temp == NULL){
                    temp = front;
                }
            }

            if(choosen_doctor_ptr != NULL){
                slot->assigned_doctor_ID[slot->assigned_amount] = choosen_doctor_ptr->ID;
                choosen_doctor_ptr->assignedShiftsPerWeek[week]++;
                choosen_doctor_ptr->totalAssignedShifts++;
                slot->assigned_amount ++;

                //update titik awal pencarian menjadi dokter berikutnya setelah dokter yang ditugaskan 
                search_start = choosen_doctor_ptr->next;
                if(search_start == NULL){
                    search_start = front;
                }
            }
            else{
                break;
            }
        }
    }
}

int prevent_duplicate(int doctor_ID, struct shift_slot *Slots){
    for(int i = 0; i < Slots ->assigned_amount; i ++){
        if(doctor_ID == Slots ->assigned_doctor_ID[i]){
            return 1;
        }
    }
    return 0;
}

int check_assigned_status(int doctor_ID, struct shift_slot *Slots, int current_index){
    struct shift_slot *temp1 = &Slots[current_index];
    int current_day = temp1 ->date.date;
    ShiftType current_shift = temp1 -> shift;

    for(int i = 0; i < current_index; i++){

        struct shift_slot *temp2 = &Slots[i];
        int prev_day = temp2 -> date.date;
        ShiftType prev_shift = temp2 -> shift;

        if (prev_day == current_day || (prev_day == current_day - 1 && prev_shift == MALAM &&  current_shift == PAGI))
        {
            for(int j = 0; j < temp2 ->assigned_amount; j++){
                if (temp2 ->assigned_doctor_ID[j] == doctor_ID)
                {
                    return 0;
                }
                
            }
        }
        
    }
    return 1;
}

void print_schedule_for_day(struct shift_slot *Slots, struct Doctor_data *front, int day){
    printf("\nJadwal untuk Tanggal: %d\n", day);
    for(int i = 0 ; i < 90 ; i++){
        if (Slots[i].date.date == day) {
            const char *shiftNames[] = {"Pagi ", "Siang", "Malam"};
            printf("Shift : %s | Dokter : ", shiftNames[Slots[i].shift]);

            if(Slots[i].assigned_amount == 0){
                printf("belum dijadwalkan\n");
            } else {
                for(int j = 0; j < Slots[i].assigned_amount; j++){
                    struct Doctor_data *temp = front;
                    while (temp) {
                        if(temp->ID == Slots[i].assigned_doctor_ID[j]){
                            printf("%s", temp->name);
                            if (j < Slots[i].assigned_amount - 1){
                                printf(", ");
                            }
                            break;
                        }
                        temp = temp->next;
                    }
                }
                printf("\n");
            }
        }
    }
}

void print_schedule_for_week(struct shift_slot *Slots, struct Doctor_data *front, int week){
    int start_date, end_date;
    if (week >= 1 && week <= 4) {
        start_date = (week - 1) * 7 + 1;
        end_date = week * 7;
    } else if (week == 5) {
        start_date = 29;
        end_date = 30;
    } else {
        printf("Minggu tidak valid.\n");
        return;
    }

    printf("\nJadwal untuk Minggu ke-%d (Tanggal %d s/d %d)\n", week, start_date, end_date);

    for(int d = start_date; d <= end_date; d++){
        printf("--- Tanggal: %d ---\n", d);
        for(int i = 0; i < 90; i++){
            if (Slots[i].date.date == d) {
                const char *shiftNames[] = {"Pagi ", "Siang", "Malam"};
                printf("Shift : %s | Dokter : ", shiftNames[Slots[i].shift]);

                if(Slots[i].assigned_amount == 0){
                    printf("belum dijadwalkan\n");
                } else {
                    for(int j = 0; j < Slots[i].assigned_amount; j++){
                        struct Doctor_data *temp = front;
                        while (temp) {
                            if(temp->ID == Slots[i].assigned_doctor_ID[j]){
                                printf("%s", temp->name);
                                if (j < Slots[i].assigned_amount - 1){
                                    printf(", ");
                                }
                                break;
                            }
                            temp = temp->next;
                        }
                    }
                    printf("\n");
                }
            }
        }
    }
}

void print_schedule(struct shift_slot *Slots, struct Doctor_data *front ){

    for(int i = 0 ; i <90 ; i++){
        const char *shiftNames[] = {"Pagi ", "Siang", "Malam"};
        printf("Tanggal : %d | Shift : %s | Dokter : ",
        Slots[i].date.date,
        shiftNames[Slots[i].shift]);

        if(Slots[i].assigned_amount == 0){
            printf("belum dijadwalkan \n");
        }
        else{
            
            for(int j = 0; j < Slots[i].assigned_amount; j++){
                struct Doctor_data *temp = front;
                while (temp)
                {
                    if(temp -> ID == Slots[i].assigned_doctor_ID[j]){
                        printf("%s", temp -> name);
                        if (j < Slots[i].assigned_amount - 1){
                            printf(", ");
                        }
                        break;   
                    }
                    temp = temp -> next;
                }
            }
            printf("\n");
        }
    }

}

void kelola_menu_tampil_jadwal(struct shift_slot *slot, struct Doctor_data *head, int jadwal_dibuat) {
    if (!jadwal_dibuat) {
        printf("Jadwal belum dibuat! Silakan pilih menu 4 terlebih dahulu.\n");
        return;
    }

    int pilihan_tampil;
    do {
        printf("\n--- Menu Tampilkan Jadwal ---\n");
        printf("1. Tampilkan Jadwal per Hari\n");
        printf("2. Tampilkan Jadwal per Minggu\n");
        printf("3. Tampilkan Jadwal Keseluruhan (per Bulan)\n");
        printf("4. Kembali ke Menu Utama\n");

        if (!get_integer_input("Pilih Opsi Tampilan: ", &pilihan_tampil)) {
            pilihan_tampil = 0;
        }

        switch (pilihan_tampil) {
            case 1: {
                int tanggal;
                while (1) {
                    if (get_integer_input("\nMasukkan tanggal (1-30 atau masukkan 0 untuk kembali): ", &tanggal)) {
                        if (tanggal == 0) {
                            break;
                        }
                        if (tanggal >= 1 && tanggal <= 30) {
                            print_schedule_for_day(slot, head, tanggal);
                        } else {
                            printf("Input tidak valid! Harap masukkan tanggal antara 1 dan 30.\n");
                        }
                    }
                }
                break;
            }
            case 2: {
                int minggu;
                while (1) {
                    if (get_integer_input("\nMasukkan minggu (1-5 atau masukkan 0 untuk kembali): ", &minggu)) {
                        if (minggu == 0) {
                            break;
                        }
                        if (minggu >= 1 && minggu <= 5) {
                            print_schedule_for_week(slot, head, minggu);
                        } else {
                            printf("Input tidak valid! Harap masukkan minggu antara 1 dan 5.\n");
                        }
                    }
                }
                break;
            }
            case 3:
                print_schedule(slot, head);
                break;
            case 4:
                printf("Kembali ke menu utama\n");
                break;
            default:
                printf("Pilihan tampilan tidak valid.\n");
        }
    } while (pilihan_tampil != 4);
}

void jadwal_to_csv(struct shift_slot *slot, struct Doctor_data *head){
    FILE*point_file=fopen("jadwal_shift.csv", "w");
    if (point_file==NULL){
        printf("file jadwal_shift.csv tidak di-write");
        return;
    }
    fprintf(point_file, "Tanggal,Shift,Dokter(ID)\n");
    for(int i = 0 ; i <90 ; i++){
        const char *shiftNames[] = {"Pagi", "Siang", "Malam"};
        fprintf(point_file, "%d,%s,",slot[i].date.date,shiftNames[slot[i].shift]);
        if (slot[i].assigned_amount==0){
            fprintf(point_file, "Belum Dijadwalkan");
        }
        for(int j = 0; j < slot[i].assigned_amount; j++){
            struct Doctor_data *temp = head;
            while (temp){
                if(temp -> ID == slot[i].assigned_doctor_ID[j]){ 
                    if ((temp->ID)< 10){                      
                        fprintf(point_file,"%s(00%d)", temp -> name, temp -> ID );
                    }
                    else if((temp->ID)< 100){
                        fprintf(point_file,"%s(0%d)", temp -> name, temp -> ID );
                    }
                    else{
                        fprintf(point_file,"%s(%d)", temp -> name, temp -> ID );
                    }
                    if (j < slot[i].assigned_amount - 1){
                        fprintf(point_file, ",");
                    }
                    break;   
                }
                temp = temp -> next;
            }
        }
        fprintf(point_file,"\n");
    }
    fclose(point_file);
}

void print_pelanggaran(struct Doctor_data *front, struct shift_slot *slot){
    printf("list pelanggaran : \n");
    int count = 1;
    bool found = false;
    const char *shiftNames[] = {"Pagi ", "Siang", "Malam"};
    for(int i = 0; i < 90; i++){
        for(int j = 0; j < slot[i].assigned_amount; j++){
            struct Pelanggaran info = slot[i].List_pelanggaran[j];

            if(info.choosen_doctor != NULL){
                struct Doctor_data *dokter = info.choosen_doctor;
                found = true;
                if(info.Pelanggaran_cuti){
                    printf("%d. Tanggal : %d | Shift : %s | Nama : %s | Jenis Pelanggaran : Hari Libur \n", count,slot[i].date.date,shiftNames[slot[i].shift], dokter->name );
                    count++;
                }
                else if (info.Pelanggaran_max_shift)
                {
                    printf("%d. Tanggal : %d | Shift : %s | Nama : %s | Jenis Pelanggaran : Melebihi kuota mingguan \n", count,slot[i].date.date,shiftNames[slot[i].shift], dokter->name );
                    count++;
                }
                else if (info.Pelanggaran_preferensi){
                    printf("%d. Tanggal : %d | Shift : %s | Nama : %s | Jenis Pelanggaran : Bukan preferensi shift \n", count,slot[i].date.date,shiftNames[slot[i].shift], dokter->name );
                    count++;
                }
            
            }
            
        }
    }
    
    if(found == false){
        printf("Tidak ada pelanggaran yang tercatat.\n");
    }
    printf("\n");

    bool kosong = false;
    printf("Daftar Slot yang Tidak Terisi : \n");
    for(int i = 0; i < 90; i++){
        if(slot[i].assigned_amount == 0){
            kosong = true;
            printf("Slot kosong : Tanggal %d, Shift %s\n",
            slot[i].date.date,
            shiftNames[slot[i].shift]);
            
        }
    }

    if(kosong == false){
        printf("Semua slot telah berhasil diisi.\n");
    }
    else{
        printf("Kekurangan dokter. Silahkan Tambah Dokter\n");
    }
}

void print_unassigned (struct Doctor_data * front){

    printf("\n");
    printf("Daftar dokter yang belum mendapat jadwal \n");
    struct Doctor_data *temp = front;
    int count = 0;

    while (temp)
    {
        if(temp ->totalAssignedShifts == 0){
            count++;
            printf("Dr. %s \n", temp->name);
           
        }

        temp = temp -> next;
    }

    if(count == 0){
        printf("semua dokter telah mendapatkan jadwal \n");
    }
}


