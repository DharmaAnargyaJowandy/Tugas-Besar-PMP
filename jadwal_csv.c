#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jadwal_csv.h"
#include "common.h"

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
        for(int j = 0; j < slot[i].assigned_amount; j++){
            struct Doctor_data *temp = head;
            while (temp){
                if(temp -> ID == slot[i].assigned_doctor_ID[j]){                        
                    fprintf(point_file,"%s(%d)", temp -> name, temp -> ID );
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