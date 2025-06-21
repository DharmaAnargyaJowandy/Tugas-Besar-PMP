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
        printf("1. Tampilkan dokter\n2. Tambah dokter\n3. Hapus dokter\n4. Membuat dan Menampilkan Jadwal\n5. Tampilkan Statistik\n6. Simpan dan keluar\nPilih: ");
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
            case 4:{
                int pilihan_jadwal;
                printf("\n--- Menu Tampilkan Jadwal ---\n");
                printf("1. Tampilkan Jadwal per Hari\n");
                printf("2. Tampilkan Jadwal per Minggu\n");
                printf("3. Tampilkan Jadwal per Bulan\n");
                if (get_integer_input("Pilih: ", &pilihan_jadwal)) { 
                    if (!jadwal_dibuat) {
                        printf("Membuat data jadwal\n");
                        generate_schedule(slot, head);
                        assign_doctor(slot, head);
                    }
                    switch (pilihan_jadwal) {
                        case 1: {
                            int tanggal;
                            if (get_integer_input("Masukkan tanggal (1-30): ", &tanggal)) {
                                if (tanggal >= 1 && tanggal <= 30) {
                                    print_schedule_for_day(slot, head, tanggal);
                                } else {
                                    printf("Pilihan tidak valid.\n");
                                }
                            }
                            break;
                        }
                        case 2:{
                            int minggu;
                            if (get_integer_input("Masukkan minggu (1-5): ", &minggu)) {
                                if (minggu >= 1 && minggu <= 5) {
                                    print_schedule_for_week(slot, head, minggu);
                                } else {
                                    printf("Pilihan tidak valid.\n");
                                }
                            }
                            break;
                        }
                        case 3:
                            print_schedule(slot, head);
                            print_unassigned(head);
                            break;
                        default:
                            printf ("Pilihan tidak valid.\n");
                            break;
                    }
                }
                break;
            }
              
            case 5:
                statistik(head);
                break;
            case 6:
                dokter_to_csv(head);
                jadwal_to_csv(slot, head);
                printf("Data disimpan. Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 6);

    return 0;
}
