#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Doctor.h"
#include "common.h"

// Fungsi untuk membaca CSV dan membuat linked list
struct Doctor_data* load_dokter_dari_csv(){
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Gagal membuka file CSV");
        return NULL;
    }

    char line[MAX_LINE];
    struct Doctor_data *head = NULL, *tail = NULL;
    fgets(line, sizeof(line), file); // Lewati header

    while (fgets(line, sizeof(line), file)){
        struct Doctor_data *newNode = (struct Doctor_data*)malloc(sizeof(struct Doctor_data));
        if (!newNode) continue;
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");

        strcpy(newNode->name, token);
        newNode->ID = atoi(strtok(NULL, ","));
        newNode->maxShiftsPerWeek = atoi(strtok(NULL, ","));
        for (int i = 0; i < 3; i++)
            newNode->prefersShift[i] = atoi(strtok(NULL, ","));
        newNode->restDay = atoi(strtok(NULL, ","));

        newNode->totalAssignedShifts = 0;
        for (int i = 0; i < 5; i++)
            newNode->assignedShiftsPerWeek[i] = 0;

        newNode->prev = tail;
        newNode->next = NULL;
        if (tail) tail->next = newNode;
        else head = newNode;
        tail = newNode;
    }

    fclose(file);
    return head;
}

// Fungsi menyimpan linked list ke CSV
void simpan_dokter_ke_csv(struct Doctor_data *head){
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        perror("Gagal membuka file untuk menulis");
        return;
    }

    fprintf(file, "Nama,ID,MaxShiftPerWeek,Pagi,Siang,Malam,RestDay\n");
    struct Doctor_data *curr = head;
    while (curr){
        fprintf(file, "%s,%03d,%d,%d,%d,%d,%d\n", curr->name, curr->ID, curr->maxShiftsPerWeek,
                curr->prefersShift[0], curr->prefersShift[1], curr->prefersShift[2], curr->restDay);
        curr = curr->next;
    }

    fclose(file);
}

// Fungsi menampilkan seluruh data dokter
void tampilkan_dokter(struct Doctor_data *head){
    while (head) {
        printf("ID: %03d | Nama: %s | Max/Minggu: %d | Pref: %d-%d-%d | Cuti: %d | total shift : %d\n",
               head->ID, head->name, head->maxShiftsPerWeek,
               head->prefersShift[0], head->prefersShift[1], head->prefersShift[2], head->restDay, head ->totalAssignedShifts);
        head = head->next;
    }
}

// Fungsi menambahkan data dokter ke akhir linked list
void tambah_dokter(struct Doctor_data **head_ref){
    struct Doctor_data *newNode = (struct Doctor_data *)malloc(sizeof(struct Doctor_data));
    printf("Masukkan nama: ");
    scanf("%s", newNode->name);
    printf("Masukkan ID (3 digit): ");
    scanf("%d", &newNode->ID);
    printf("Masukkan maksimal shift/minggu: ");
    scanf("%d", &newNode->maxShiftsPerWeek);
    printf("Preferensi shift pagi: ");
    scanf("%d", &newNode->prefersShift[0]);
    printf("Preferensi shift siang: ");
    scanf("%d", &newNode->prefersShift[1]);
    printf("Preferensi shift malam: ");
    scanf("%d", &newNode->prefersShift[2]);
    printf("Masukkan tanggal cuti (1-30): ");
    scanf("%d", &newNode->restDay);

    newNode->totalAssignedShifts = 0;
    for (int i = 0; i < 5; i++) newNode->assignedShiftsPerWeek[i] = 0;

    newNode->next = NULL;
    if (!*head_ref){
        newNode->prev = NULL;
        *head_ref = newNode;
        return;
    }

    struct Doctor_data *tail = *head_ref;
    while (tail->next) tail = tail->next;
    tail->next = newNode;
    newNode->prev = tail;
}

// Fungsi menghapus dokter berdasarkan ID
void hapus_dokter(struct Doctor_data **head_ref, int targetID){
    struct Doctor_data *curr = *head_ref;
    while (curr && curr->ID != targetID) curr = curr->next;
    if (!curr) {
        printf("ID %d tidak ditemukan.\n", targetID);
        return;
    }

    if (curr->prev) curr->prev->next = curr->next;
    else *head_ref = curr->next;
    if (curr->next) curr->next->prev = curr->prev;

    free(curr);
    printf("Dokter dengan ID %d telah dihapus.\n", targetID);
}

void statistik(struct Doctor_data *front){

    int min_sft = max;
    int max_sft = 0;
    int count = 0;
    double sum = 0.0;
    double sum_squares = 0.0;

    if (front == NULL) {
        printf("Daftar dokter kosong, tidak ada statistik untuk ditampilkan.\n");
        return;
    }

    struct Doctor_data *temp = front;

    while (temp)
    {
        int total = temp -> totalAssignedShifts;

        if(total > max_sft){
            max_sft = total;
        }

        if(total < min_sft){
            min_sft = total;
        }

        sum = sum + total;
        sum_squares = sum_squares + (total * total);
        count ++;

        temp =  temp -> next;
    }

    int range = max_sft - min_sft;;

    double average = sum/count;

    double variance = (sum_squares/ count) - ( average * average);

    double standard_deviation = sqrt(variance);

    printf("Statistik Beban Kerja Dokter\n");
    printf("Jumlah dokter         : %d\n", count);
    printf("Total shift tertinggi : %d\n", max_sft);
    printf("Total shift terendah  : %d\n", min_sft);
    printf("Range                 : %d\n", range);
    printf("Rata-rata Shift       : %.2f\n", average);
    printf("Standard deviation    : %.2f\n", standard_deviation);
}

