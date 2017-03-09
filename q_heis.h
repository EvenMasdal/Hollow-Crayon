extern int queue_arr[6];

void q_set_request(int queuePos);

int q_get_next_floor(int last_floor, int last_dir);

int q_get_next_direction(int last_floor, int last_dir);

void q_clear_floor(int floor);

void q_clear_queue(void);

int q_floor_and_dir_to_queuePos(int floor, int dir);

int q_queuePos_to_floor(int queuePos);
