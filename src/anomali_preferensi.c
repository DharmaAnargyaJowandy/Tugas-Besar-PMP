#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anomali_preferensi.h"
#include "common.h"
#include "Schedule.h"

void fill_empty_shift(struct shift_slot *slot, struct Doctor_data *head){
    int i;
    int doc_count=0;
    int initial=0;
    int pelanggaranPerweek[30/7];
    for (int j=1; j<5; j++){
        pelanggaranPerweek[j]=initial;
    }
    struct Doctor_data *backup_doc=NULL;

    for (i=0; i<90; i++){
        int week = (slot[i].date.date) / 7;
        if (slot[i].assigned_amount==0){
            backup_doc=nonpreferensi_tersedikit(slot, head, i, week);
            if (backup_doc!=NULL){
                pelanggaranPerweek[week]++;
                slot[i].assigned_doctor_ID[0] = backup_doc->ID;
                slot[i].assigned_amount = 1;
                backup_doc->assignedShiftsPerWeek[week]++;
                backup_doc->totalAssignedShifts++;
            }
            else{
                printf("tidak ada dokter yang tersedia untuk shift kosong\n");
            }
        }

    }
    for(int j=1; j<5;j++){
        printf("Jumlah pelanggaran preferensi pada minggu ke - %d: %d\n", j, pelanggaranPerweek[j]);
    } 
}

int jumlah_dokter(struct Doctor_data *head){
    int jumlah=0;
    struct Doctor_data *traverser=head;
    while (traverser!=NULL){
        jumlah++;
        traverser=traverser->next;
    }
    return jumlah;
}

struct Doctor_data *nonpreferensi_tersedikit(struct shift_slot *slot, struct Doctor_data *head, int which_shift, int week){
    int total_dokter;
    int doc_count;
    int fewest_nonprefer_val=max;
    int *nonpreference_individu; // berapa preferensi yang telah dilanggar per individu jika dia dimasukkan di shift itu
    struct Doctor_data *traverser=head;
    struct Doctor_data *fewest_nonprefer_doctor=NULL;
    total_dokter=jumlah_dokter(head);
    nonpreference_individu=(int*)malloc(total_dokter*sizeof(int));
    if (nonpreference_individu==NULL){
        printf("tidak ada dokter\n");
        return NULL;
    }
    memset(nonpreference_individu,0,total_dokter*sizeof(int));
    doc_count=0;
    while (traverser!=NULL){
        // set dokter yang sudah ada di shift tersebut ke high value
        for (int doctor_id=0; doctor_id<11; doctor_id++){
            if (slot[which_shift].assigned_doctor_ID[doctor_id]==traverser->ID){
                nonpreference_individu[doc_count]=max;
            }
        }
        if (nonpreference_individu[doc_count]!=max){
            // hitung jumlah nonpreferensi yang terjadi untuk seorang dokter jika dimasukkan di shift itu 
            if((traverser->prefersShift[slot[which_shift].shift] == 0)){
                nonpreference_individu[doc_count]++;
            }
            if((traverser->assignedShiftsPerWeek[week]) > (traverser->maxShiftsPerWeek)){
                nonpreference_individu[doc_count]++;
            }
            if(check_assigned_status(traverser ->ID,slot, which_shift)==0){
                nonpreference_individu[doc_count]++;            
            }
            if((slot[which_shift].date.date == traverser ->restDay)){
                nonpreference_individu[doc_count]++;            
            }
        }

            // catat nilai jumlah nonpreferensi terkecil
            if (fewest_nonprefer_val>nonpreference_individu[doc_count]){
                fewest_nonprefer_val=nonpreference_individu[doc_count];
            }
        traverser=traverser->next;
        doc_count++;
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
    free(nonpreference_individu);
    return fewest_nonprefer_doctor;
}