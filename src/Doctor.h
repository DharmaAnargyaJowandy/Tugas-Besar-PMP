#ifndef DOCTOR_H
#define DOCTOR_H

struct Doctor_data* load_dokter_dari_csv();
void dokter_to_csv(struct Doctor_data *head);
int cek_id_sama(struct Doctor_data *head, int ID);
void tampilkan_dokter(struct Doctor_data *head);
void tambah_dokter(struct Doctor_data **head_ref);
void hapus_dokter(struct Doctor_data **head_ref, int targetID);
void statistik(struct Doctor_data *front);

#endif