#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Doctor.h"
#include "common.h"
#include "Schedule.h"


int main(){
    struct Doctor_data *head = load_dokter_dari_csv();
    int pilihan, id;
    struct shift_slot slot[90];

    do{
        printf("\n=== Menu Dokter ===\n");
        printf("1. Tampilkan dokter\n2. Tambah dokter\n3. Hapus dokter\n4. Membuat dan Menampilkan Jadwal\n5. Simpan dan keluar\nPilih: ");
        scanf("%d", &pilihan);

        system("cls");

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
                generate_schedule(slot);
                assign_doctor(slot, head);
                print_schedule(slot, head);
                break;
            case 5:
                simpan_dokter_ke_csv(head);
                printf("Data disimpan. Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 5);

    return 0;
}
