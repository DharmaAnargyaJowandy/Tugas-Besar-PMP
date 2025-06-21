#ifndef ANOMALI_PREFERENSI_H
#define ANOMALI_PREFERENSI_H

#include "common.h"

void fill_empty_shift(struct shift_slot *slot, struct Doctor_data *head);
int jumlah_dokter(struct Doctor_data *head);
struct Pelanggaran nonpreferensi_tersedikit(struct shift_slot *slot, struct Doctor_data *head, int which_shift, int week);
#endif