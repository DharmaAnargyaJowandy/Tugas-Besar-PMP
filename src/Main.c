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
                jadwal_dibuat = 1;
                break;
            case 5: {
                if (!jadwal_dibuat) {
                    printf("Jadwal belum dibuat! Silakan pilih menu 4 terlebih dahulu.\n");
                    break;
                }

                int pilihan_tampil;
                do {
                    printf("\n--- Menu Tampilkan Jadwal ---\n");
                    printf("1. Tampilkan Jadwal per Hari\n");
                    printf("2. Tampilkan Jadwal per Minggu\n");
                    printf("3. Tampilkan Jadwal Keseluruhan (per Bulan)\n");
                    printf("4. Kembali ke Menu Utama\n");

                    if (!get_integer_input("Pilih Opsi Tampilan: ", &pilihan_tampil)) {
                        pilihan_tampil = 0; 
                    }

                    switch (pilihan_tampil) {
                        case 1: {
                            int tanggal;
                            while (1) { 
                                if (get_integer_input("\nMasukkan tanggal (1-30 atau masukkan 0 untuk kembali): ", &tanggal)) {
                                    if (tanggal == 0) {
                                        break;
                                    }
                                    if (tanggal >= 1 && tanggal <= 30) {
                                        print_schedule_for_day(slot, head, tanggal);
                                    } else {
                                        printf("Input tidak valid! Harap masukkan tanggal antara 1 dan 30.\n");
                                    }
                                }
                            }
                            break;
                        }
                        case 2: { 
                            int minggu;
                            while (1) { 
                                if (get_integer_input("\nMasukkan minggu (1-5 atau masukkan 0 untuk kembali): ", &minggu)) {
                                    if (minggu == 0) {
                                        break; 
                                    }
                                    if (minggu >= 1 && minggu <= 5) {
                                        print_schedule_for_week(slot, head, minggu);
                                    } else {
                                        printf("Input tidak valid! Harap masukkan minggu antara 1 dan 5.\n");
                                    }
                                }
                            }
                            break;
                        }
                        case 3:
                            print_schedule(slot, head);
                            break;
                        case 4:
                            printf("Kembali ke menu utama\n");
                            break;
                        default:
                            printf("Pilihan tampilan tidak valid.\n");
                    }
                } while (pilihan_tampil != 4);
                break;
            }
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
