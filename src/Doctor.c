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
        line[strcspn(line,"\r\n")] = '\0';
        if (line[0] == '\0') continue;
        struct Doctor_data *newNode = (struct Doctor_data*)malloc(sizeof(struct Doctor_data));
        if (!newNode) continue;
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
void dokter_to_csv(struct Doctor_data *head){
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

// Fungsi menampilkan seluruh data dokter (diurutkan berdasarkan ID)
void tampilkan_dokter(DataDokter *head){
    // Salin ke array pointer
    int count = 0;
    DataDokter *curr = head;
    while (curr) {count++; curr = curr->next;}
    DataDokter **array = malloc(sizeof(DataDokter*) * count);
    curr = head;
    for (int i = 0; i < count; i++){
        array[i] = curr;
        curr = curr->next;
    }
    // Bubble sort by ID
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++){
            if (array[j]->ID > array[j+1]->ID){
                DataDokter *temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
    // Cetak hasil
    for (int i = 0; i < count; i++){
        printf("ID: %03d | Nama: %s | Max Shift/Minggu: %d | Pref: %d-%d-%d | Tanggal Cuti: %d\n",
               array[i]->ID, array[i]->name, array[i]->maxShiftsPerWeek,
               array[i]->prefersShift[0], array[i]->prefersShift[1], array[i]->prefersShift[2], array[i]->restDay);
    }
    free(array);
}

// Fungsi cek duplikat ID
int cek_id_sama(DataDokter *head, int ID){
    while (head){
        if (head->ID == ID) return 1;
        head = head->next;
    }
    return 0;
}

// Fungsi menambahkan data dokter
void tambah_dokter(DataDokter **head_ref) {
    DataDokter *newNode = (DataDokter *)malloc(sizeof(DataDokter));
    char buffer[100];
    getchar(); // bersihkan newline buffer

    printf("Masukkan nama: ");
    fgets(newNode->name, sizeof(newNode->name), stdin);
    newNode->name[strcspn(newNode->name, "\n")] = '\0'; // hapus newline

    int valid = 0;
    do {
        printf("Masukkan ID (3 digit): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &newNode->ID) != 1 || newNode->ID < 1 || newNode->ID > 999) {
            printf("Input tidak valid! Masukkan angka 3 digit (1–999).\n");
            continue;
        }
        if (cek_id_sama(*head_ref, newNode->ID)) {
            printf("ID sudah digunakan. Masukkan ID lain.\n");
        } else {
            valid = 1;
        }
    } while (!valid);

    do {
        printf("Masukkan maksimal shift/minggu (maksimal 7): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &newNode->maxShiftsPerWeek) != 1 || newNode->maxShiftsPerWeek < 1 || newNode->maxShiftsPerWeek > 7)
            printf("Input tidak valid atau melebihi batas!");
    } while (newNode->maxShiftsPerWeek < 1 || newNode->maxShiftsPerWeek > 7);

    for (int i = 0; i < 3; i++) {
        do {
            if (i == 0)
                printf("Preferensi shift pagi (0 atau 1): ");
            else if (i == 1)
                printf("Preferensi shift siang (0 atau 1): ");
            else
                printf("Preferensi shift malam (0 atau 1): ");

            fgets(buffer, sizeof(buffer), stdin);
            if (sscanf(buffer, "%d", &newNode->prefersShift[i]) != 1 || (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1)) {
                printf("Input tidak valid!\n");
                newNode->prefersShift[i] = -1; // Meminta ulang
            }
        } while (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1);
    }

    do {
        printf("Masukkan tanggal cuti (1-30): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &newNode->restDay) != 1 || newNode->restDay < 1 || newNode->restDay > 30)
            printf("Tanggal tidak valid!\n");
    } while (newNode->restDay < 1 || newNode->restDay > 30);

    newNode->totalAssignedShifts = 0;
    for (int i = 0; i < 5; i++) newNode->assignedShiftsPerWeek[i] = 0;

    newNode->next = NULL;
    if (!*head_ref) {
        newNode->prev = NULL;
        *head_ref = newNode;
        return;
    }

    DataDokter *tail = *head_ref;
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

