#ifndef SCHEDULE_H
#define SCHEDULE_H

void generate_schedule(struct shift_slot *slot);
void assign_doctor(struct shift_slot *slots, struct Doctor_data *front );
void print_schedule(struct shift_slot *Slots, struct Doctor_data *front );


#endif