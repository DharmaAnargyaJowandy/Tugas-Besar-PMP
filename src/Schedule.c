#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "Schedule.h"

void generate_schedule(struct shift_slot *slot){
    int index = 0;
    for(int i = 1; i < 31; i++){
        for(int j = 0; j < 3; j++){
            slot[index].shift = j;
            slot[index].assigned_amount = 0;
            slot[index].date.date = i;
            index++;
        }
    }
}

void assign_doctor(struct shift_slot *slots, struct Doctor_data *front ){
    for(int i = 0; i < 90; i++){
        struct shift_slot *slot = &slots[i];
        int week = (slot -> date.date) / 7;

        int choosen_Doctor = -1;
        int minshift = max;
        struct Doctor_data *choosen_doctor_ptr = NULL;
        struct Doctor_data *temp = front;

        while (temp != NULL)
        {
            if((temp ->prefersShift[slot -> shift] == 1) && ((temp->assignedShiftsPerWeek[week]) < (temp->maxShiftsPerWeek))){
                if(temp -> assignedShiftsPerWeek[week] < minshift){
                    choosen_doctor_ptr = temp;
                    minshift = temp -> assignedShiftsPerWeek[week];
                    break;
                }
            }

            temp = temp -> next;
        }

        if(choosen_doctor_ptr != NULL){
            slot->assigned_doctor_ID[0] = choosen_doctor_ptr->ID;
            slot->assigned_amount = 1;
            choosen_doctor_ptr->assignedShiftsPerWeek[week]++;
            choosen_doctor_ptr->totalAssignedShifts++;
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
