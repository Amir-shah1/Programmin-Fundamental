#define MAX_ROOMS 25

// Get list of all rooms from file
char **getAllRoomsList(char *data_file, int length);

// Get timetable for specific semester and section
char **getTimeTable(char *file_name, int semester, char section);

// Check which rooms are currently free
char **checkCurrentFreeRooms(char *file_name, int total_rooms);

// Get current time slot
char *getCurrentSlot();

// Get list of all occupied room number for a specific day and time slot
char **checkFreeSlotsForDay(char *file_name, char *rooms_file, char *selected_day, char *selected_time_slot);

// Print time slots
void printTimeSlots();
