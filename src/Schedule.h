#ifndef SCHEDULE_H
#define SCHEDULE_H

void generate_schedule(struct shift_slot *slot);
void assign_doctor(struct shift_slot *slots, struct Doctor_data *front );
void print_schedule(struct shift_slot *Slots, struct Doctor_data *front );
int check_assigned_status(int doctor_ID, struct shift_slot *Slots, int current_index);


#endif