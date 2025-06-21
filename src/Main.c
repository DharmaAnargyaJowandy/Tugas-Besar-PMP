#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Doctor.h"
#include "anomali_preferensi.h"
#include "common.h"
#include "Schedule.h"




int main(){
    struct Doctor_data *head = load_dokter_dari_csv();
    int pilihan, id;
    struct shift_slot slot[90];
    int jadwal_dibuat = 0;

    do{
        printf("\n=== Menu Dokter ===\n");
        printf("1. Tampilkan dokter\n2. Tambah dokter\n3. Hapus dokter\n4. Membuat Jadwal\n5. Menampilkan Jadwal\n6. Tampilkan Statistik\n7. Simpan dan keluar\nPilih: ");
        scanf("%d", &pilihan);
        printf("\n");


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
                generate_schedule(slot, head);
                assign_doctor(slot, head);
                fill_empty_shift(slot, head);
                print_pelanggaran(head, slot);
                print_unassigned(head);
                break;
            case 5:
                print_schedule(slot, head);
                break;
            case 6:
                statistik(head);
                break;
            case 7:
                dokter_to_csv(head);
                jadwal_to_csv(slot, head);
                printf("Data disimpan. Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 7);

    return 0;
}
