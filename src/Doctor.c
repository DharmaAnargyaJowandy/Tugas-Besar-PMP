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
int compare_doctors_by_id(const void *a, const void *b) {
    struct Doctor_data *docA = *(struct Doctor_data **)a;
    struct Doctor_data *docB = *(struct Doctor_data **)b;
    return (docA->ID - docB->ID);
}

// Fungsi menampilkan seluruh data dokter (diurutkan berdasarkan ID)
void tampilkan_dokter(struct Doctor_data *head){
    int count = 0;
    struct Doctor_data *curr = head;
    while (curr) {count++; curr = curr->next;}

    if (count == 0) {
        printf("Belum ada data dokter.\n");
        return;
    }

    struct Doctor_data **array = malloc(sizeof(struct Doctor_data*) * count);
    if (!array) {
        perror("Gagal alokasi memori untuk sorting");
        return;
    }
    curr = head;
    for (int i = 0; i < count; i++){
        array[i] = curr;
        curr = curr->next;
    }
    qsort(array, count, sizeof(struct Doctor_data*), compare_doctors_by_id);
    printf("Daftar Dokter (Diurutkan berdasarkan ID):\n");
    for (int i = 0; i < count; i++){
        printf("ID: %03d | Nama: %s | Max Shift/Minggu: %d | Pref: %d-%d-%d | Tanggal Cuti: %d\n",
               array[i]->ID, array[i]->name, array[i]->maxShiftsPerWeek,
               array[i]->prefersShift[0], array[i]->prefersShift[1], array[i]->prefersShift[2], array[i]->restDay);
    }
    free(array);
}

// Fungsi cek duplikat ID
int cek_id_sama(struct Doctor_data *head, int ID){
    while (head){
        if (head->ID == ID) return 1;
        head = head->next;
    }
    return 0;
}

// Fungsi menambahkan data dokter
void tambah_dokter(struct Doctor_data **head_ref) {
    struct Doctor_data *newNode = (struct Doctor_data *)malloc(sizeof(struct Doctor_data));
    if (!newNode) {
        perror("Gagal alokasi memori untuk dokter baru");
        return;
    }
    char buffer[100];
    int c; 
    int initial_char = getchar();
    if (initial_char != '\n' && initial_char != EOF) {
        ungetc(initial_char, stdin);
    } else if (initial_char == EOF) {
        return;
    }

    printf("Masukkan nama: ");
    fgets(newNode->name, sizeof(newNode->name), stdin);
    if (strchr(newNode->name, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF);
    } else {
        newNode->name[strcspn(newNode->name, "\n")] = '\0';
    }
    int valid = 0;

    do {
        printf("Masukkan ID (3 digit): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &newNode->ID) != 1 || newNode->ID < 1 || newNode->ID > 999) {
            printf("Input tidak valid! Masukkan angka 3 digit (1-999).\n");
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
            printf("Input tidak valid atau melebihi batas!\n");
    } while (newNode->maxShiftsPerWeek < 1 || newNode->maxShiftsPerWeek > 7);

    do {
        valid = 1; // Diasumsikan valid, kecuali jika input 0-0-0
        for (int i = 0; i < 3; i++) {
            do {
                if (i == 0)
                    printf("Preferensi shift pagi (0 atau 1): ");
                else if (i == 1)
                    printf("Preferensi shift siang (0 atau 1): ");
                else
                    printf("Preferensi shift malam (0 atau 1): ");

                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &newNode->prefersShift[i]) != 1 || 
                    (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1)) {
                    printf("Input tidak valid!\n");
                    newNode->prefersShift[i] = -1;
                }
            } while (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1);
        }

        // Cek apakah semua preferensi adalah 0
        if (newNode->prefersShift[0] == 0 && 
            newNode->prefersShift[1] == 0 && 
            newNode->prefersShift[2] == 0) {
            printf("Minimal input shift 1 kali.\n");
            valid = 0; // Input tidak valid, ulangi
        }
    } while (!valid);

    do {
        printf("Masukkan tanggal cuti (1-30, atau 0 jika tidak ada): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &newNode->restDay) != 1 || newNode->restDay < 0 || newNode->restDay > 30)
            printf("Tanggal tidak valid! Masukkan angka antara 0 dan 30.\n");
    } while (newNode->restDay < 0 || newNode->restDay > 30); 

    newNode->totalAssignedShifts = 0;
    for (int i = 0; i < 5; i++) newNode->assignedShiftsPerWeek[i] = 0;

    newNode->next = NULL;
    if (!*head_ref) {
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

