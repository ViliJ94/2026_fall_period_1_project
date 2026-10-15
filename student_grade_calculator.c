//
// Created by vilij on 1/10/2026.
//

/* student grade calculator
Write a program that calculates and displays the final grades for a student based on their scores in
different subjects. The program should allow the user to input scores for multiple subjects and then
calculate the average grade and overall grade for the student.
Requirements:
• The program should start by asking the user for their name.
• Then, it should ask the user how many subjects they want to calculate grades for.
• For each subject, the program should prompt the user to enter the subject name and the
score (as a percentage).
• Calculate the average grade based on the scores and display it.
• Determine and display the overall grade based on the following grading scale:
o 5: 90-100
o 4: 80-89
o 3: 70-79
o 2: 60-69
o 1: 50-59
o 0: Below 50
• The program must also write the same report to a file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_SUBJECTS 10 // who has more than 10 subjects at the same time am I right? 10 might be overkill but hey
#define BUFFER_SIZE 128

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
    int grade;
} Subject;

/*
 * reads integers from user and makes sure that it is in the allowed range
*/


int read_int(const char *prompt, int min_val, int max_val)
{
    char buffer[BUFFER_SIZE];
    int value = 0;
    char extra;
    int valid = 0;

    while (!valid)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error reading input, try again please!\n");
        }
        else
        {
            char *newline = strchr(buffer, '\n');
            if (newline == NULL)
            {
                // disregard the remaining characters from the input buffer
                int c = getchar();
                while (c != '\n' && c != EOF) {
                    c = getchar();
                }
                printf("Error: Input too long, try again please!\n");
            }
            else
            {
                *newline = '\0'; // remove the newline character

                if (strlen(buffer) == 0)
                {
                    printf("Error: Input can't be empty, try again please!\n");
                }
                else if (sscanf(buffer, "%d %c", &value, &extra) != 1)
                {
                    printf("Error: Input must be an integer, try again please!\n");
                }
                else if (value < min_val || value > max_val)
                {
                    printf("Error: Input must be between %d - %d, try again please!\n", min_val, max_val);
                }
                else
                {
                    valid = 1;
                }
            }
        }
    }
    return value;
}

/*
 * reads a non-empty string from user and ensures it fits in destination array
*/
void read_string (const char *prompt, char *dest, int max_len)
{
    char buffer[BUFFER_SIZE];
    int valid = 0;

    while (!valid) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error reading input, try again please!\n");
        }
        else
        {
            char *newline = strchr(buffer, '\n');
            if (newline == NULL)
            {
                // disregard the remaining characters from the input buffer
                int c = getchar();
                while (c != '\n' && c != EOF)
                {
                    c = getchar();
                }
                printf("Error: Input too long! Try again please!\n");
            }
            else
            {
                *newline = '\0'; // remove the newline character

                if (strlen(buffer) == 0)
                {
                    printf("Error: Input can't be empty, try again please!\n");
                }
                else if ((int)strlen(buffer) >= max_len)
                {
                    printf("Error: Input too long, max length %d! Try again please!\n", max_len - 1);
                }
                else
                {
                    strncpy(dest, buffer, max_len - 1);
                    dest[max_len - 1] = '\0';
                    valid = 1;
                }
            }
        }
    }
}


/*
 * calculates the grade (0-5) based on percentage score
*/

int calculate_grade(int score)
{
    int grade = 0;

    if (score >= 90)
    {
        grade = 5;
    }
    else if (score >= 80)
    {
        grade = 4;
    }
    else if (score >= 70)
    {
        grade = 3;
    }
    else if (score >= 60)
    {
        grade = 2;
    }
    else if (score >= 50)
    {
        grade = 1;
    }
    else
    {
        grade = 0;
    }
    return grade;
}

/*
 * collects student name & subject info from user
 * returns number of subjects entered
*/

int collect_student_data(char *student_name, Subject subjects[])
{
    read_string("Welcome to Student Grade Calculator (Trademark pending)!\nPlease, enter your name: \n", student_name, MAX_NAME_LENGTH);

    int num_subjects = read_int("How many subjects do you want to calculate grades for? Enter the amount: \n", 1, MAX_SUBJECTS);

    int i = 0;
    while (i < num_subjects)
    {
        printf("\nEnter subject %d name:\n", i + 1);
        read_string("", subjects[i].name, MAX_NAME_LENGTH);

        printf("\nEnter subject %s score (between 0-100):\n", subjects[i].name);
        subjects[i].score = read_int("", 0, 100);

        subjects[i].grade = calculate_grade(subjects[i].score);

        i++;
    }
    return num_subjects;
}

/*
 * write the report to a file
 * allows the user to overwrite or create a file
 * or append an existing file
*/

int write_report_file(const char *student_name, const Subject subjects[], int num_subjects, double average_score, double average_grade, int overall_grade)
{
    char file_name[BUFFER_SIZE];
    char mode[2] = "w";
    FILE *file = NULL;
    int success = 0;

    while (!success)
    {
        read_string("Remember to add .txt at the end of the file!\nEnter the name of the file you wish to save the report to: \n", file_name, sizeof(file_name));

        printf("How do you want to do this?\n");
        printf("1. Create a new file or overwrite an existing file\n");
        printf("2. Append to an existing file\n");

        int choice = read_int("Enter your choice (1 or 2): \n", 1, 2);

        if (choice == 2)
        {
            // check if file exists before attempt to append it
            FILE *check = fopen(file_name, "r");
            if (check == NULL)
            {
                fprintf(stderr, "Error: File '%s' does not exist. Cannot append!\n\n", file_name);
            }
            else
            {
                fclose(check);
                mode[0] = 'a';
                file = fopen(file_name, mode);

                if (file == NULL)
                {
                    fprintf(stderr, "Error: file '%s' could not be opened!\n\n", file_name);
                }
                else
                {
                    success = 1;
                }
            }
        }
        else if (choice == 1)
        {
            mode[0] = 'w';
            file = fopen(file_name, mode);

            if (file == NULL)
            {
                fprintf(stderr, "Error: file '%s' could not be opened!\n\n", file_name);
            }
            else
            {
                success = 1;
            }
        }
    }


    fprintf(file, "--------------------------------------------------\n");
    fprintf(file, "Student: %s\n", student_name);
    fprintf(file, "--------------------------------------------------\n");
    fprintf(file, "%-26s %8s %12s\n", "Subject", "Score", "Grade");
    fprintf(file, "--------------------------------------------------\n");

    int i = 0;
    while (i < num_subjects)
    {
        fprintf(file, "%-26s %7d%% %12d\n", subjects[i].name, subjects[i].score, subjects[i].grade);
        i++;
    }
    fprintf(file,"\nAverage score: %.2f%%\n", average_score);
    fprintf(file, "\nAverage grade: %.2f\n", average_grade);
    fprintf(file, "\nOverall grade: %d\n", overall_grade);
    fprintf(file, "--------------------------------------------------\n\n");

    fclose(file);
    printf("Report saved to '%s'!\n", file_name);
    return 1;
}




/*
 * print report to console
*/

void print_report_console(const char *student_name, const Subject subjects[], int num_subjects, double average_score, double average_grade, int overall_grade)
{
    printf("--------------------------------------------------\n");
    printf("Student: %s\n", student_name);
    printf("--------------------------------------------------\n");
    printf("%-26s %8s %12s\n", "Subject", "Score", "Grade");
    printf("--------------------------------------------------\n");

    int i = 0;
    while (i < num_subjects)
    {
        printf("%-26s %7d%% %12d\n", subjects[i].name, subjects[i].score, subjects[i].grade);
        i++;
    }
    printf("\nAverage score: %.2f%%\n", average_score);
    printf("\nAverage grade: %.2f\n", average_grade);
    printf("\nOverall grade: %d\n", overall_grade);
    printf("--------------------------------------------------\n");
}

int main(void) {
    char student_name[MAX_NAME_LENGTH];
    Subject subjects[MAX_SUBJECTS];

    // gather info (student name & subjects) from user
    int num_subjects = collect_student_data(student_name, subjects);

    // calculate the average of the grades
    double grade_sum = 0.0;
    double score_sum = 0.0;
    int i = 0;
    while (i < num_subjects)
    {
        grade_sum += subjects[i].grade;
        score_sum += subjects[i].score;
        i++;
    }
    double average_grade = grade_sum / num_subjects;
    double average_score = score_sum / num_subjects;
    int overall_grade = calculate_grade((int)average_score);

    // print report to console
    print_report_console(student_name, subjects, num_subjects, average_score, average_grade, overall_grade);

    // write report to file
    write_report_file(student_name, subjects, num_subjects, average_score, average_grade, overall_grade);

    return 0;
}
