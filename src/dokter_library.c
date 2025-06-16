#include "dokter_library.h" 

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void tampilkan_menu() {
    printf("\nMENU MANAJEMEN DOKTER\n");
    printf("1. Tambah Data Dokter Baru\n");
    printf("2. Hapus Data Dokter\n");
    printf("3. Tampilkan Semua Dokter\n");
    printf("4. Keluar\n");
    printf("\n");
    printf("Pilih opsi: ");
}

int main() {
    Node* head = NULL;
    Node* tail = NULL;
    int pilihan;

    while (1) {
        tampilkan_menu();
        if (scanf("%d", &pilihan) != 1) {
            printf("\nInput tidak valid. Harap masukkan angka.\n");
            clear_input_buffer(); 
            continue; 
        }
        clear_input_buffer(); 

        switch (pilihan) {
            case 1: {
                DataDokter dokterBaru;
                
                printf("\nMenambahkan Dokter Baru\n");
                printf("Masukkan ID Dokter (3 digit): ");
                scanf("%d", &dokterBaru.ID);
                clear_input_buffer();

                printf("Masukkan Nama Dokter: ");
                fgets(dokterBaru.name, NAMA_MAX, stdin);
                dokterBaru.name[strcspn(dokterBaru.name, "\n")] = 0;

                printf("Masukkan Maksimum Shift per Minggu: ");
                scanf("%d", &dokterBaru.maxShiftsPerWeek);

                printf("Preferensi Shift Pagi (1=Ya, 0=Tidak): ");
                scanf("%d", &dokterBaru.prefersShift[0]);

                printf("Preferensi Shift Siang (1=Ya, 0=Tidak): ");
                scanf("%d", &dokterBaru.prefersShift[1]);

                printf("Preferensi Shift Malam (1=Ya, 0=Tidak): ");
                scanf("%d", &dokterBaru.prefersShift[2]);
                clear_input_buffer();
                
                tambah_dokter(&head, &tail, dokterBaru);
                break;
            }
            case 2: {
                int id_hapus;
                printf("\nMenghapus Data Dokter\n");
                printf("Masukkan ID dokter yang akan dihapus: ");
                scanf("%d", &id_hapus);
                clear_input_buffer();

                hapus_dokter(&head, &tail, id_hapus);
                break;
            }
            case 3: {
                tampil_semua_dokter(head);
                break;
            }
            case 4: {
                printf("\nTerima kasih telah menggunakan program ini.\n");
                Node* current = head;
                while (current != NULL) {
                    Node* next = current->next;
                    free(current);
                    current = next;
                }
                return 0;
            }
          
            default: {
                printf("\n>> Pilihan tidak valid. Harap masukkan angka antara 1-4.\n");
                break;
            }
        }
    }

    return 0;
}
