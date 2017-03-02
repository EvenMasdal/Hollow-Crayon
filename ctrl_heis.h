
extern bool moving;

typedef enum { false, true } bool;

void ctrl_init(void);

void ctrl_emergency_stop(void);

void ctrl_requests(void);

void ctrl_move(void);

void ctrl_floor_sensor(void);
