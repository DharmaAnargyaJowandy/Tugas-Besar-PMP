#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <stdbool.h>

void generate_schedule(struct shift_slot *slot, struct Doctor_data *front );
void assign_doctor(struct shift_slot *slots, struct Doctor_data *front );
void print_schedule(struct shift_slot *Slots, struct Doctor_data *front );
bool get_integer_input(const char* prompt, int* result);
void print_schedule_for_day(struct shift_slot *Slots, struct Doctor_data *front, int day);
void print_schedule_for_week(struct shift_slot *Slots, struct Doctor_data *front, int week);
int check_assigned_status(int doctor_ID, struct shift_slot *Slots, int current_index);
int prevent_duplicate(int doctor_ID, struct shift_slot *Slots);
void print_unassigned (struct Doctor_data * front);
void jadwal_to_csv(struct shift_slot *slot, struct Doctor_data *head);

#endif
