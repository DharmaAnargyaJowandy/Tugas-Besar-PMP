#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "anomali_preferensi.h"
#include "common.h"
#include "Schedule.h"

// fungsi untuk mengisi shift yang masih kosong karena pelanggaran preferensi
void fill_empty_shift(struct shift_slot *slot, struct Doctor_data *head){
    int i;
    int doc_count=0;
    struct Doctor_data *backup_doc=NULL;

    for (i=0; i<90; i++){
        int week = (slot[i].date.date) / 7;
        if (slot[i].assigned_amount==0){
            int week = (slot[i].date.date) / 7;
            struct Pelanggaran Hasil = nonpreferensi_tersedikit(slot, head, i, week);
            if (Hasil.choosen_doctor != NULL){
                int amount = slot[i].assigned_amount;
                slot[i].List_pelanggaran[amount] = Hasil;
                slot[i].assigned_doctor_ID[0] = Hasil.choosen_doctor->ID;
                slot[i].assigned_amount ++;
                Hasil.choosen_doctor->assignedShiftsPerWeek[week]++;
                Hasil.choosen_doctor->totalAssignedShifts++;
            }
        }

    }
}

// fungsi menghitung jumlah dokter
int jumlah_dokter(struct Doctor_data *head){
    int jumlah=0;
    struct Doctor_data *traverser=head;
    while (traverser!=NULL){
        jumlah++;
        traverser=traverser->next;
    }
    return jumlah;
}

// fungsi untuk mencari dokter dengan pelanggaran preferensi tersedikit
struct Pelanggaran nonpreferensi_tersedikit(struct shift_slot *slot, struct Doctor_data *head, int which_shift, int week){
    struct Pelanggaran hasil;
    struct Doctor_data *fewest_nonprefer_doctor=NULL;
    hasil.choosen_doctor = NULL;
    hasil.Pelanggaran_preferensi = false;
    hasil.Pelanggaran_cuti = false;
    hasil.Pelanggaran_max_shift = false;
    int *nonpreference_individu; // berapa preferensi yang telah dilanggar per individu jika dia dimasukkan di shift itu
    int doc_count;
    int fewest_nonprefer_val=max;
    struct Doctor_data *traverser=head;
    

    int total_dokter=jumlah_dokter(head);
    if(total_dokter == 0){
        printf("tidak ada dokter dalam daftar\n");
        return hasil;
    }

    nonpreference_individu=(int*)calloc(total_dokter,sizeof(int));

    if (nonpreference_individu==NULL){
        printf("tidak ada dokter\n");
        return hasil;
    }
    
    doc_count=0;
    while (traverser!=NULL){

        //Agar pelanggaran tidak melebihi 7 shift setiap minggu
        bool cast_away = false;

        if(traverser->assignedShiftsPerWeek[week] >= ABSOLUT || check_assigned_status(traverser->ID,slot, which_shift) == 0 ){
            cast_away = true;
        }

        
        if (cast_away){
            nonpreference_individu[doc_count] = max;
        }
        else{
            //apabila tidak pada preferensi shift pagi siang atau malam  diberi penalti 1
            if((traverser->prefersShift[slot[which_shift].shift] == 0)){
                nonpreference_individu[doc_count]+= 1;
            }
            //apabila melebihi maximum shift per minggu diberi penalti 3
            if((traverser->assignedShiftsPerWeek[week]) > (traverser->maxShiftsPerWeek)){
                nonpreference_individu[doc_count]+= 3;
            }
            // Apabila hari libur diberi penalti 10
            if((slot[which_shift].date.date == traverser ->restDay)){
                nonpreference_individu[doc_count]+= 10;            
            }

        }
        // set dokter yang sudah ada di shift tersebut ke high value
        
        // catat nilai jumlah nonpreferensi terkecil
        if (fewest_nonprefer_val>nonpreference_individu[doc_count]){
            fewest_nonprefer_val=nonpreference_individu[doc_count];
        }
        traverser=traverser->next;
        doc_count++;
    }


    if (fewest_nonprefer_val == max) {
        free(nonpreference_individu);
        return hasil;
    }

    traverser=head;
    doc_count=0;
    while (traverser!=NULL){
        if (fewest_nonprefer_val==nonpreference_individu[doc_count]){
            fewest_nonprefer_doctor=traverser;
            break;
        }
        doc_count++;
        traverser=traverser->next;
    }

    if (fewest_nonprefer_doctor != NULL)
    {
        hasil.choosen_doctor = fewest_nonprefer_doctor;
        if(slot[which_shift].date.date == fewest_nonprefer_doctor->restDay){
            hasil.Pelanggaran_cuti = true;
        }
        if(fewest_nonprefer_doctor ->prefersShift[slot[which_shift].shift] == 0){
            hasil.Pelanggaran_preferensi = true;
        }
        if(fewest_nonprefer_doctor ->assignedShiftsPerWeek[week] > fewest_nonprefer_doctor ->maxShiftsPerWeek){
            hasil.Pelanggaran_max_shift = true;
        }
    }
    
    free(nonpreference_individu);
    return hasil;
}
