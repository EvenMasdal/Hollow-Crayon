

void q_set_request(int floorDir);

int q_get_next_floor(int last_floor, int last_dir);

int q_get_next_direction(int last_floor, int last_dir);

void q_clear_order(int floor);

void q_clear_queue(void);

int q_floor_and_dir_to_array_int(int floor, int dir);

int q_array_int_to_floor(int array_int);

//int q_array_int_to_dir(int array_int);

