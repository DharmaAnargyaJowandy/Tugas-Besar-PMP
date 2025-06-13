#ifndef DOKTER_LIBRARY_H
#define DOKTER_LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAMA_MAX 50
#define JUMLAH_SHIFT 3
#define MAX_CUTI 7

typedef struct {
    int ID;
    char name[NAMA_MAX];
    int maxShiftsPerWeek;
    int prefersShift[JUMLAH_SHIFT]; // [0]:Pagi, [1]:Siang, [2]:Malam
    int restDayPerWeek[MAX_CUTI];
    int jumlahCuti;
    int assignedShiftsPerWeek[5];
    int totalAssignedShifts;
} DataDokter;

typedef struct Node {
    DataDokter data;
    struct Node* next;
    struct Node* prev;
} Node;

static void tambah_dokter(Node** head, Node** tail, DataDokter data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error: Alokasi memori gagal!\n");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
    printf("\n>> Sukses: Dokter dengan ID %d atas nama %s berhasil ditambahkan.\n", data.ID, data.name);
}

static int hapus_dokter(Node** head, Node** tail, int id) {
    Node* current = *head;
    while (current != NULL) {
        if (current->data.ID == id) {
            if (current == *head) {
                *head = current->next;
                if (*head != NULL) { (*head)->prev = NULL; } 
                else { *tail = NULL; }
            } else if (current == *tail) {
                *tail = current->prev;
                (*tail)->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            printf("\n>> Sukses: Data dokter dengan ID %d telah dihapus.\n", id);
            free(current);
            return 1;
        }
        current = current->next;
    }
    printf("\n>> Gagal: Dokter dengan ID %d tidak ditemukan.\n", id);
    return 0;
}

static void tampil_semua_dokter(Node* head) {
    printf("\n--- Daftar Semua Data Dokter ---\n");
    if (head == NULL) {
        printf("Tidak ada data dokter untuk ditampilkan.\n");
        return;
    }
    Node* current = head;
    int count = 1;
    while (current != NULL) {
        printf("Dokter #%d\n", count);
        printf("  ID                 : %d\n", current->data.ID);
        printf("  Nama               : %s\n", current->data.name);
        printf("  Maks Shift/Minggu  : %d\n", current->data.maxShiftsPerWeek);
        printf("  Preferensi Shift   : Pagi(%d), Siang(%d), Malam(%d)\n",
               current->data.prefersShift[0],
               current->data.prefersShift[1],
               current->data.prefersShift[2]);
        printf("\n");
        current = current->next;
        count++;
    }
}

#endif // DOKTER_LIBRARY_H
