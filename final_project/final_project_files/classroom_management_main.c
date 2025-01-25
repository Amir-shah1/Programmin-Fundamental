#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ROOMS 25
#include "classroom_management.h"



/**
 * Main function - Entry point of the timetable management system
 * Provides a menu-driven interface for various timetable operations
 */
int main(void)
{
    char *timetable_file = "CS_Department_Timetable.csv";
    char *rooms_file = "all_rooms.txt";

    while (1)
    {
        int user_selection = 0;
        do{
            // Display menu options for user interaction
            printf("\nChoose an option:\n");
            printf("1. View your timetable.\n");
            printf("2. Check current-slot free rooms.\n");
            printf("3. Check free rooms for a specific day.\n");
            printf("4. Exit the program.\n");

            scanf("%d", &user_selection);
            while(getchar() != '\n'); // Clear input buffer
        }while(user_selection != 1 && user_selection != 2 && user_selection != 3 && user_selection != 4);
        

        // This condition will print the time-table for given semester and section
        if (user_selection == 1)
        {
            int student_semester;
            char student_section;

            // Input validation for semester and section
            do
            {
                printf("Enter your semester (1-8): ");
                scanf("%d", &student_semester);
                printf("Enter your section (A-D): ");
                scanf(" %c", &student_section);
            } while (student_semester < 1 || student_semester > 8 || (student_section != 'A' && student_section != 'B' &&
                        student_section != 'C' && student_section != 'D'));

            char **time_table;
            time_table = getTimeTable(timetable_file,student_semester, student_section);
            printf("\nTime Table of Semester %d Section %c:\n", student_semester, student_section);
            printf("%-12s%-15s%-20s%-20s%-10s\n", "Day", "Time", "Subject", "Instructor", "Room");
            printf("=======================================================================\n");

            for(int i = 0; time_table[i] != NULL; i++)
            {
                int entry_semester;
                char entry_section, day[20], time[20], subject[50], instructor[50], room[20];
                sscanf(time_table[i], "%d,%c,%[^,],%[^,],%[^,],%[^,],%s", &entry_semester, &entry_section, day, time, subject,  instructor, room);
                printf("%-12s%-15s%-20s%-20s%-10s\n", day, time, subject, instructor, room);
            }
            free(time_table);
        }
        // This condition will print free rooms avialiable for the current time slot
        else if (user_selection == 2)
        {
            char **free_rooms;
            free_rooms = checkCurrentFreeRooms(timetable_file, MAX_ROOMS);
            int check = 0;
            for(int i = 0; free_rooms[i] != NULL; i++)
            {
                printf("%s\n", free_rooms[i]);
                check = 1;
            }
            if(check == 0)
            {
                printf("No room aviable\n");
            }
            free(free_rooms);
        }
        // This condition will print free rooms avialiable for the specific day and time slot
        else if (user_selection == 3)
        {
            // Get user input for day and time slot
            char selected_day[20];
            printf("Enter day (Monday-Friday): ");
            scanf("%s", selected_day);

            int time_selection;
            printTimeSlots();
            do
            {
                printf("\nEnter time slot number (1-5): ");
                scanf("%d", &time_selection);
            } while (time_selection < 1 || time_selection > 5);

            // Convert time slot selection to actual time range
            char selected_time_slot[15];
            switch (time_selection)
            {
            case 1:
                strcpy(selected_time_slot, "9:00-10:30");
                break;
            case 2:
                strcpy(selected_time_slot, "10:30-12:00");
                break;
            case 3:
                strcpy(selected_time_slot, "12:00-2:00");
                break;
            case 4:
                strcpy(selected_time_slot, "2:00-3:30");
                break;
            case 5:
                strcpy(selected_time_slot, "3:30-5:00");
                break;
            }

            char **specific_day_free_rooms;
            specific_day_free_rooms = checkFreeSlotsForDay(timetable_file, rooms_file, selected_day, selected_time_slot);
            int check = 0;
            printf("The free rooms for %s, Time Slot %s are:\n", selected_day, selected_time_slot);
            for(int i = 0; specific_day_free_rooms[i] != NULL; i++)
            {
                printf("%s\n", specific_day_free_rooms[i]);
                check = 1;
            }
            if(check == 0)
            {
                printf("No room aviable\n");
            }
            free(specific_day_free_rooms);
        }
        else if (user_selection == 4)
        {
            printf("Exiting the program. Goodbye!\n");
            break;
        }
    }
    return 0;
}
