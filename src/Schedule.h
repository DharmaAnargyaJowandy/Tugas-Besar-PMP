#ifndef SCHEDULE_H
#define SCHEDULE_H

void generate_schedule(struct shift_slot *slot, struct Doctor_data *front );
void assign_doctor(struct shift_slot *slots, struct Doctor_data *front );
void print_schedule(struct shift_slot *Slots, struct Doctor_data *front );
int check_assigned_status(int doctor_ID, struct shift_slot *Slots, int current_index);
int prevent_duplicate(int doctor_ID, struct shift_slot *Slots);
void print_unassigned (struct Doctor_data * front);
void fill_empty_shift(struct shift_slot *slot, struct Doctor_data *head);
int jumlah_dokter(struct Doctor_data *head);
struct Doctor_data *nonpreferensi_tersedikit(struct shift_slot *slot, struct Doctor_data *head, int which_shift, int week);
void jadwal_to_csv(struct shift_slot *slot, struct Doctor_data *head);

#endif
