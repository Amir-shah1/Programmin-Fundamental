#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ROOMS 25

/* Function Declarations
 * getAllRoomsList: Reads room numbers from a file and returns them in an array
 * getTimeTable: Retrieves timetable entries for a specific semester and section
 * checkCurrentFreeRooms: Determines which rooms are currently unoccupied
 * getCurrentSlot: Returns the current time slot based on system time
 */

//==============================================================================
/**
 * getAllRoomsList - Reads all room numbers from a file into an array
 * @param data_file: Path to the file containing room numbers
 * @param total_rooms: Maximum number of rooms to read
 * @return: Array of strings containing room numbers
 *
 * Function reads room numbers line by line from a file, where each line
 * contains one room number. Returns a dynamically allocated array of strings.
 */
char **getAllRoomsList(char *data_file, int total_rooms)
{
    // Open file and check for errors
    FILE *file = fopen(data_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open the file %s.\n", file);
        exit(1);
    }

    // Allocate memory for array of room number strings
    char **room_array = malloc(sizeof(char *) * total_rooms);
    for (int i = 0; i < total_rooms; i++)
    {
        room_array[i] = NULL; // Initialize all pointers to NULL for safety
    }

    // Read room numbers line by line and store in array
    char line[6];
    int count = 0;
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        room_array[count] = malloc(strlen(line) + 1);
        strcpy(room_array[count], line);
        count++;
    }
    fclose(file);
    return room_array;
}

//==============================================================================
/**
 * getTimeTable - Retrieves timetable entries for specific semester and section
 * @param file_name: Path to timetable file
 * @param semester: Target semester number
 * @param section: Target section character
 * @return: Array of strings containing matching timetable entries
 *
 * Reads timetable CSV file and returns entries matching the specified
 * semester and section. Each entry contains day, time, subject, instructor, and room.
 */
char **getTimeTable(char *file_name, int semester, char section)
{
    // Open timetable file and check for errors
    FILE *timetable_file = fopen(file_name, "r");
    if (timetable_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open the file %s.\n", file_name);
        return NULL;
    }

    // Initialize array to store matching timetable entries
    int maximum_clsses = 20;
    char **weekly_classes_array;
    weekly_classes_array = malloc(sizeof(char *) * maximum_clsses);
    for (int i = 0; i < maximum_clsses; i++)
    {
        weekly_classes_array[i] = NULL;
    }

    // Read and process each line of the timetable
    char timetable_line[100];
    int count = 0;
    while (fgets(timetable_line, sizeof(timetable_line), timetable_file))
    {
        // Remove newline and parse CSV fields
        timetable_line[strcspn(timetable_line, "\n")] = 0;
        int entry_semester;
        char entry_section, day[20], time[20], subject[50], instructor[50], room[20];
        sscanf(timetable_line, "%d,%c,%[^,],%[^,],%[^,],%[^,],%s",
               &entry_semester, &entry_section, day, time, subject, instructor, room);

        // Store entries matching target semester and section
        if (entry_semester == semester && entry_section == section)
        {
            weekly_classes_array[count] = malloc(strlen(timetable_line) + 1);
            if (weekly_classes_array[count] == NULL)
            {
                fprintf(stderr, "Memory allocation failed!\n");
                exit(1);
            }
            strcpy(weekly_classes_array[count], timetable_line);
            count++;
        }
    }

    fclose(timetable_file);
    return weekly_classes_array;
}

//==============================================================================
/**
 * getCurrentSlot - Determines current time slot based on system time
 * @return: String representing current time slot or "1" if outside valid times
 *
 * Returns the current academic time slot (e.g., "9:00-10:30") based on system time.
 * Only returns valid slots during weekdays between 9 AM and 5 PM.
 */
char *getCurrentSlot()
{
    char *current_time_slot_str = (char *)malloc(15 * sizeof(char));

    // Get current system time
    time_t current_timestamp = time(NULL);
    struct tm *current_time = localtime(&current_timestamp);

    // Check if within working hours (9 AM to 5 PM)
    int current_hour = current_time->tm_hour;
    if (current_hour < 9 || current_hour > 17)
    {
        return "Invalid"; // Return 1 for invalid time
    }

    // Convert to 12-hour format and determine current time slot
    int current_minute = current_time->tm_min;
    if (current_hour > 12)
    {
        current_hour -= 12;
    }


    // Return appropriate time slot based on current time
    if ((current_hour == 9) || (current_hour == 10 && current_minute <= 30))
    {
        strcpy(current_time_slot_str, "9:00-10:30");
        return current_time_slot_str;
    }
    if ((current_hour == 9) || (current_hour == 10 && current_minute <= 30))
    {
        strcpy(current_time_slot_str, "9:00-10:30");
        return current_time_slot_str;
    }
    else if ((current_hour == 10 && current_minute > 30) || (current_hour == 11 && current_minute <= 59))
    {
        strcpy(current_time_slot_str, "10:30-12:00");
        return current_time_slot_str;
    }
    else if ((current_hour == 12) || (current_hour == 1))
    {
        strcpy(current_time_slot_str, "12:00-2:00");
        return current_time_slot_str;
    }
    else if ((current_hour == 2) || (current_hour == 3 && current_minute <= 30))
    {
        strcpy(current_time_slot_str, "2:00-3:30");
        return current_time_slot_str;
    }
    else if ((current_hour == 3 && current_minute > 30) || (current_hour == 4 && current_minute < 59))
    {
        strcpy(current_time_slot_str, "3:30-5:00");
        return current_time_slot_str;
    }
    // ... [similar blocks for other time slots]

    return NULL;
}

//==============================================================================
/**
 * checkCurrentFreeRooms - Determines which rooms are free in current time slot
 * @param file_name: Path to timetable file
 * @param total_rooms: Total number of rooms in the file
 * @return: Array of strings containing free room numbers
 *
 * Checks timetable against current time to determine which rooms are not
 * currently scheduled for use. Returns NULL on weekends or file errors.
 */
char **checkCurrentFreeRooms(char *file_name, int total_rooms)
{
    // Initialize array for free rooms
    char **current_free_rooms = malloc(sizeof(char *) * total_rooms);
    for (int i = 0; i < total_rooms; i++)
    {
        current_free_rooms[i] = NULL;
    }

    // Get current time and day
    time_t current_timestamp = time(NULL);
    struct tm *current_time = localtime(&current_timestamp);

    char *weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    char current_day[20];
    strcpy(current_day, weekdays[current_time->tm_wday]);

    // Return empty array;
    if (strcmp(current_day, "Saturday") == 0 || strcmp(current_day, "Sunday") == 0)
    {
        fprintf(stderr, "No slots available on weekends.\n");
        return current_free_rooms;
    }

    // Open and validate timetable file
    FILE *timetable_file = fopen(file_name, "r");
    if (!timetable_file)
    {
        fprintf(stderr, "Error: Unable to open file %s.\n", timetable_file);
        exit(1);
    }

    // Get current time slot
    char *current_time_slot_str = getCurrentSlot();

    char header[100];
    fgets(header, sizeof(header), timetable_file); // Skip header line

    // Read timetable and identify occupied rooms
    int count = 0;
    char timetable_entry[100];
    while (fgets(timetable_entry, sizeof(timetable_entry), timetable_file))
    {
        timetable_entry[strcspn(timetable_entry, "\n")] = 0;
        char entry_day[20], entry_time[20], room[20];
        sscanf(timetable_entry, "%*d,%*c,%[^,],%[^,],%*[^,],%*[^,],%s",
               entry_day, entry_time, room);
        if ((strcmp(entry_day, current_day) == 0) && (strcmp(entry_time, current_time_slot_str) == 0))
        {
            current_free_rooms[count] = malloc(strlen(room) + 1);
            strcpy(current_free_rooms[count], room);
            count++;
        }
    }

    // Remove duplicates by setting them to NULL
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        if (current_free_rooms[i] == NULL)
            continue;

        // Compare with all following elements
        for (int j = i + 1; j < MAX_ROOMS; j++)
        {
            if (current_free_rooms[j] != NULL && strcmp(current_free_rooms[i], current_free_rooms[j]) == 0)
            {
                current_free_rooms[j] = NULL; // Set duplicate to NULL
            }
        }
    }
    printf("\nFree slots for %s, Time Slot %s:\n", current_day, current_time_slot_str);
    fclose(timetable_file);
    return current_free_rooms;
}

//==============================================================================
/**
 * checkFreeSlotsForDay - Checks which rooms are free for a specific day and time slot
 * @param file_name: Path to timetable file
 * @param rooms_file: Path to file containing room numbers
 * @return: Array of strings containing free room numbers
 *
 * Reads timetable and room files to determine which rooms are free for a
 * specific day and time slot. Returns NULL if no rooms are available.
 */
char **checkFreeSlotsForDay(char *file_name, char *rooms_file, char *selected_day, char *selected_time_slot)
{
    // Open and validate input files
    FILE *timetable_file = fopen(file_name, "r");
    if (!timetable_file)
    {
        fprintf(stderr, "Error: Unable to open timetable file.\n");
        return NULL;
    }

    FILE *all_rooms_file = fopen(rooms_file, "r");
    if (!all_rooms_file)
    {
        fprintf(stderr, "Error: Unable to open rooms file.\n");
        fclose(timetable_file);
        return NULL;
    }

    // Read all available rooms
    char **available_rooms = getAllRoomsList(rooms_file, MAX_ROOMS);


    // Process timetable data
    char entry_day[15] = {0};
    char entry_time[15] = {0};
    char room_number[15] = {0};
    char timetable_entry[100];
    char **occupied_rooms =  malloc(sizeof(char *) * MAX_ROOMS);
    int occupied_count = 0;

    // Find occupied rooms for selected day and time
    while (fgets(timetable_entry, sizeof(timetable_entry), timetable_file))
    {
        sscanf(timetable_entry, "%*d,%*c,%[^,],%[^,],%*[^,],%*[^,],%s", entry_day, entry_time, room_number);
        if((strcmp(selected_day, entry_day) == 0) && (strcmp(selected_time_slot, entry_time) == 0))
        {
            occupied_rooms[occupied_count] = malloc(strlen(room_number) + 1);
            strcpy(occupied_rooms[occupied_count], room_number);
            occupied_count++;
        }
    }

    // Remove duplicates by setting them to NULL
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        if (occupied_rooms[i] == NULL)
            continue;

        // Compare with all following elements
        for (int j = i + 1; j < MAX_ROOMS; j++)
        {
            if (occupied_rooms[j] != NULL && strcmp(occupied_rooms[i], occupied_rooms[j]) == 0)
            {
                occupied_rooms[j] = NULL; // Set duplicate to NULL
            }
        }
    }

    fclose(timetable_file);
    fclose(all_rooms_file);
    return occupied_rooms;
}


//==============================================================================
/**
 * Displays all available time slots in the timetable
 * Prints the slot number along with start and end times
 */
void printTimeSlots()
{
    printf("\nAvailable Time Slots:\n");
    printf("1. 9:00-10:30\n");
    printf("2. 10:30-12:00\n");
    printf("3. 12:00-2:00\n");
    printf("4. 2:00-3:30\n");
    printf("5. 3:30-5:00\n");
    return;
}