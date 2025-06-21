#ifndef SCHEDULE_H
#define SCHEDULE_H

void generate_schedule(struct shift_slot *slot, struct Doctor_data *front );
void assign_doctor(struct shift_slot *slots, struct Doctor_data *front );
void print_schedule(struct shift_slot *Slots, struct Doctor_data *front );
int check_assigned_status(int doctor_ID, struct shift_slot *Slots, int current_index);
int prevent_duplicate(int doctor_ID, struct shift_slot *Slots);
void print_unassigned (struct Doctor_data * front);
void jadwal_to_csv(struct shift_slot *slot, struct Doctor_data *head);

#endif
