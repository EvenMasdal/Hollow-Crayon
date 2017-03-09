
extern int moving;
extern int last_floor;
extern int last_dir;
extern int door_status;


void ctrl_init(void);

void ctrl_emergency_stop(void);

void ctrl_requests(void);

void ctrl_move(void);

void ctrl_hit_floor(int floor);
