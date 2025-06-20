#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "daftar_dokter.csv"
#define MAX_LINE 256

// Struct untuk data dokter
typedef struct data_dokter{
    int ID;
    char name[50];
    int maxShiftsPerWeek;
    int prefersShift[3]; // 0: pagi, 1: siang, 2: malam
    int restDay;
    int assignedShiftsPerWeek[5];
    int totalAssignedShifts;
    struct data_dokter *prev, *next;
} DataDokter;

// Fungsi untuk membaca CSV dan membuat linked list
DataDokter* load_dokter_dari_csv(){
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Gagal membuka file CSV");
        return NULL;
    }

    char line[MAX_LINE];
    DataDokter *head = NULL, *tail = NULL;
    fgets(line, sizeof(line), file); // Lewati header

    while (fgets(line, sizeof(line), file)){
        DataDokter *newNode = (DataDokter*)malloc(sizeof(DataDokter));
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
void simpan_dokter_ke_csv(DataDokter *head){
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        perror("Gagal membuka file untuk menulis");
        return;
    }

    fprintf(file, "Nama,ID,MaxShiftPerWeek,Pagi,Siang,Malam,RestDay\n");
    DataDokter *curr = head;
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

// Fungsi menambahkan data dokter ke akhir linked list
void tambah_dokter(DataDokter **head_ref){
    DataDokter *newNode = (DataDokter *)malloc(sizeof(DataDokter));
    getchar(); // bersihkan newline buffer

    printf("Masukkan nama: ");
    fgets(newNode->name, sizeof(newNode->name), stdin);
    newNode->name[strcspn(newNode->name, "\n")] = '\0'; // hapus newline

    int valid = 0;
    do{
        printf("Masukkan ID (3 digit): ");
        scanf("%d", &newNode->ID);
        if (cek_id_sama(*head_ref, newNode->ID)){
            printf("ID sudah digunakan. Masukkan ID lain.\n");
        }
        else valid = 1;
    } while (!valid);

    do{
        printf("Masukkan maksimal shift/minggu: ");
        scanf("%d", &newNode->maxShiftsPerWeek);
        if (newNode->maxShiftsPerWeek < 1 || newNode->maxShiftsPerWeek > 7){
            printf("Batas maksimal jadwal shift per minggu adalah 7 hari dan minimal 1 hari.\n");
        }
    } while (newNode->maxShiftsPerWeek < 1 || newNode->maxShiftsPerWeek > 7);

    for (int i = 0; i < 3; i++){
        do {
            if (i == 0)
                printf("Preferensi shift pagi (0 atau 1): ");
            else if (i == 1)
                printf("Preferensi shift siang (0 atau 1): ");
            else
                printf("Preferensi shift malam (0 atau 1): ");

            scanf("%d", &newNode->prefersShift[i]);

            if (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1){
                printf("Input tidak valid!\n");
            }
        } while (newNode->prefersShift[i] != 0 && newNode->prefersShift[i] != 1);
    }

    do{
        printf("Masukkan tanggal cuti (1-30): ");
        scanf("%d", &newNode->restDay);
        if (newNode->restDay < 1 || newNode->restDay > 30){
            printf("Tanggal tidak valid!\n");
        }
    } while (newNode->restDay < 1 || newNode->restDay > 30);

    newNode->totalAssignedShifts = 0;
    for (int i = 0; i < 5; i++){
        newNode->assignedShiftsPerWeek[i] = 0;
    }

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
void hapus_dokter(DataDokter **head_ref, int targetID){
    DataDokter *curr = *head_ref;
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

// Menu utama
int main(){
    DataDokter *head = load_dokter_dari_csv();
    int pilihan, id;

    do{
        printf("\n=== Menu Dokter ===\n");
        printf("1. Tampilkan dokter\n2. Tambah dokter\n3. Hapus dokter\n4. Simpan dan keluar\nPilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tampilkan_dokter(head);
                break;
            case 2:
                tambah_dokter(&head);
                break;
            case 3:
                printf("Masukkan ID yang ingin dihapus: ");
                scanf("%d", &id);
                hapus_dokter(&head, id);
                break;
            case 4:
                simpan_dokter_ke_csv(head);
                printf("Data disimpan. Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 4);

    return 0;
}
