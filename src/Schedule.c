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


