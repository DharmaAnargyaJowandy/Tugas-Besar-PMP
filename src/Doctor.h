#ifndef DOCTOR_H
#define DOCTOR_H

struct Doctor_data* load_dokter_dari_csv();
void simpan_dokter_ke_csv(struct Doctor_data *head);
void tampilkan_dokter(struct Doctor_data *head);
void tambah_dokter(struct Doctor_data **head_ref);
void hapus_dokter(struct Doctor_data **head_ref, int targetID);
void statistik(struct Doctor_data *front);

#endif