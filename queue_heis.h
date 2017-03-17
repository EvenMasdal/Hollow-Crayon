extern int queue_arr[6];

void queue_set_request(int queuePos);

int queue_get_next_floor(int last_floor, int last_dir);

int queue_get_next_direction(int last_floor, int last_dir);

void queue_clear_floor(int floor);

void queue_clear_queue(void);

int queue_floor_and_dir_to_queuePos(int floor, int dir);

int queue_queuePos_to_floor(int queuePos);
