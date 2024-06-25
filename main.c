#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_LENGTH 50

//for login page..
typedef struct
{
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;


//for main page..
//Define student and course structures...
struct StudentInfo
{
    char ID[10];
    char Name[20];
    char Email[30];
    char Phone[20];
    int  NumberOfCourse;
};

struct CourseInfo
{
    char StudentID[10];
    char Code[10];
    char Name[20];
};

struct StudentInfo Students[100];
struct CourseInfo Courses[500];

//start for login function..
void registerUser();
int loginUser();
void loadUsers();
void saveUsers();
int isValidPassword(const char *password);

User users[MAX_USERS];
int userCount = 0;
//end for login function..

//for searching functions..
int i,j;
int TotalStudents = 0;
int TotalCourse = 0;
char StudentID[10];
FILE *AllStudents;
FILE *AllCourses;
FILE *ExistingAllStudents;
FILE *TempAllStudents;
FILE *ExistingAllCourses;
FILE *TempAllCourses;
void gotoxy(int x,int y)
{
	COORD CRD;
    CRD.X = x;
    CRD.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),CRD);
}


void saveStudents();
void loadStudents();
void saveCourses();
void loadCourses();

bool IsRunning = true;
void Menu();
void AddNewStudent();
void ShowAllStudents();
int  SearchStudent(char StudentID[10]);
void EditStudent(int StudentFoundIndex);
void DeleteStudent(int StudentIndex);
void DeleteAllStudents();
int  IsAlreadyExists(char GivenLine[30],char InfoType, char StudentID[300]);
void ErrorAndRestart(char *Error[100]);
void DeleteCourseByIndex(int CourseIndex);
void DeleteStudentByIndex(int CourseIndex);
void UserGuideline();
void AboutUs();
void GoBackOrExit();
void ExitProject();

//void DataSeed();
////end searching functions..
int main()
{
    int choice;
    SetConsoleTitle("Student Management System | IIUC");

    loadUsers();

    while (1)
    {
        printf("\n\n\t*********HOME PAGE*********");
        printf("\n\n\t     1. REGISTER\n\n\t     2. LOGIN\n\n\t     3. EXIT");
        printf("\n\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("color 5f");
            registerUser();

            break;
        case 2:
            if (loginUser() == 0)
            {
                //yDataSeed();
                loadStudents();
                loadCourses();

                while(IsRunning)
                {
                    Menu();
                    int Option;
                    scanf("%d",&Option);
                    switch(Option)
                    {
                    case 0:
                        IsRunning = false;
                        ExitProject();
                        break;
                    case 1:
                        system("cls");
                        system("COLOR 03");
                        printf("\n\t\t ** Add A New Student **\n\n");
                        AddNewStudent();
                        GoBackOrExit();
                        break;
                    case 2:
                        system("cls");
                        system("color 9f");
                        printf("\n\t\t ** All Students **\n\n");
                        ShowAllStudents();
                        GoBackOrExit();
                        break;
                    case 3:
                    {
                        system("cls");
                        printf("\n\t\t ** Search Students **\n\n");
                        printf(" Enter The Student ID: ");
                        scanf("%s",StudentID);
                        int IsFound = SearchStudent(StudentID);
                        if(IsFound<0)
                        {
                            printf(" No Student Found\n\n");
                        }
                        printf("\n");
                        GoBackOrExit();
                        break;
                    }
                    case 4:
                        system("cls");
                        printf("\n\t\t ** Edit a Student **\n\n");
                        printf(" Enter The Student ID: ");
                        scanf("%s",StudentID);
                        int StudentFoundIndex = SearchStudent(StudentID);

                        if(StudentFoundIndex>=0)
                        {
                            EditStudent(StudentFoundIndex);
                        }
                        else
                        {
                            printf(" No Student Found\n\n");
                        }
                        GoBackOrExit();
                        break;
                    case 5:
                        system("cls");

                        printf("\n\t\t ** Delete a Student **\n\n");
                        printf(" Enter The Student ID: ");
                        scanf("%s",StudentID);

                        int DeleteStudentFoundIndex = SearchStudent(StudentID);

                        if(DeleteStudentFoundIndex>=0)
                        {
                            char Sure = 'N';
                            getchar();
                            printf("\n\n");
                            printf(" Are you sure want to delete this student? (Y/N): ");
                            scanf("%c",&Sure);

                            if(Sure == 'Y' || Sure == 'y')
                            {
                                DeleteStudent(DeleteStudentFoundIndex);
                            }
                            else
                            {
                                printf(" Your Data is Safe.\n\n");
                                GoBackOrExit();
                            }

                        }
                        else
                        {
                            printf(" No Student Found\n\n");
                            GoBackOrExit();
                        }

                        break;
                    case 6:
                    {
                        char Sure = 'N';
                        getchar();
                        system("cls");
                        printf("\n\t\t ** Delete ALL Students **\n\n");

                        printf(" Are you sure want to delete all the students? (Y/N): ");
                        scanf("%c",&Sure);
                        if(Sure == 'Y' || Sure == 'y')
                        {
                            DeleteAllStudents();
                        }
                        else
                        {
                            printf(" Your Data is Safe.\n\n");
                            GoBackOrExit();
                        }
                        break;
                    }

                    case 7:
                        system("cls");
                        break;
                    case 8:
                        system("cls");
                        UserGuideline();
                        GoBackOrExit();
                        break;
                    case 9:
                        system("cls");
                        AboutUs();
                        GoBackOrExit();
                        break;
                    default:
                        ExitProject();
                        break;
                    }
                }

                return 0;
            }
            else
            {
                printf("\n\n\tIncorrect username or password.\n");
            }
            break;
        case 3:
            saveUsers();
            exit(0);
        default:
            printf("\n\tInvalid choice. Please try again.\n");
        }
    }

    return 0;
}


//start login functions..

void registerUser()
{
//    if (userCount >= MAX_USERS)
//    {
//        printf("\n\tUser limit reached. Cannot register more users.\n");
//        return;
//    }

    User newUser;
    printf("\n\tEnter Username: ");
    scanf("%s", newUser.username);

    while (1)
    {
        printf("\n\tEnter Password: ");

        scanf(" %s", newUser.password);
                        int passLen = strlen(newUser.password);
                 printf("Password: ");
                for(int j = 0; j < passLen; j++) {
                    printf("*");
                }

        if (isValidPassword(newUser.password))
        {
            break;
        }
        else
        {
            printf("\n\tError: Password must contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n");
        }
    }

    users[userCount++] = newUser;
    saveUsers();
    printf("\n\tUser registered successfully!\n");
}

int loginUser()
{
    char username[MAX_LENGTH], password[MAX_LENGTH];
    printf("\n\tEnter Username: ");
    scanf("%s", username);
    printf("\n\tEnter Password: ");
    scanf(" %s", password);

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return 0;  // Success
        }
    }

    return -1;  // Failure
}

void loadUsers()
{
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL)
    {
        printf("\n\tNo user data found. Starting fresh.\n");
        return;
    }

    fread(&userCount, sizeof(int), 1, file);
    fread(users, sizeof(User), userCount, file);
    fclose(file);
}

void saveUsers()
{
    FILE *file = fopen("users.dat", "wb");
    if (file == NULL)
    {
        printf("\n\tError saving user data.\n");
        return;
    }

    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fclose(file);
}

int isValidPassword(const char *password)
{
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;

    for (int i = 0; i < strlen(password); i++)
    {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else if (ispunct(password[i])) hasSpecial = 1;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}
//end login functions..

//start mainpage functions..
void Menu()
{
    printf("\n\n\t**Student Management System Using C**\n\n");
    printf("\t\t\tMAIN MENU\n");
    printf("\t\t=======================\n");
    printf("\t\t[1] Add A New student.\n");
    printf("\t\t[2] Show All students.\n");
    printf("\t\t[3] Search A student.\n");
    printf("\t\t[4] Edit A student.\n");
    printf("\t\t[5] Delete A student.\n");
    printf("\t\t[6] Delete All students.\n");
    printf("\t\t[7] Clear The window.\n");
    printf("\t\t[8] User Guideline.\n");
    printf("\t\t[9] About Us.\n");
    printf("\t\t[0] Exit the Program.\n");
    printf("\t\t=======================\n");
    printf("\t\tEnter The Choice: ");
}

void AddNewStudent()
{
    char StudentID[12];
    char Name[30];
    char Phone[14];
    char Email[30];
    int NumberOfCourses;
    char CourseCode[5];
    char CourseName[30];

    int IsValidID = 0;
    while(!IsValidID)
    {
        printf(" Enter The ID: ");
        scanf("%s", StudentID);
        if(IsAlreadyExists(StudentID, 'i', StudentID) > 0)
        {
            printf(" Error: This ID already exists.\n\n");
            IsValidID = 0;
        }
        else if(strlen(StudentID) > 10)
        {
            printf(" Error: ID cannot be more than 10 characters.\n\n");
            IsValidID = 0;
        }
        else if(strlen(StudentID) <= 0)
        {
            printf(" Error: ID cannot be empty.\n\n");
            IsValidID = 0;
        }
        else
        {
            IsValidID = 1;
        }
    }

    int IsValidName = 0;
    while(!IsValidName)
    {
        printf(" Enter The Name: ");
        scanf(" %[^\n]s", Name);
        if(strlen(Name) > 20)
        {
            printf(" Error: Name cannot be more than 20 characters.\n\n");
            IsValidName = 0;
        }
        else if(strlen(Name) <= 0)
        {
            printf(" Error: Name cannot be empty.\n\n");
            IsValidName = 0;
        }
        else
        {
            int isAlpha = 1;
            for(int i = 0; i < strlen(Name); i++)
            {
                if(!((Name[i] >= 'A' && Name[i] <= 'Z') || (Name[i] >= 'a' && Name[i] <= 'z') || Name[i] == ' '))
                {
                    isAlpha = 0;
                    break;
                }
            }
            if(isAlpha)
            {
                IsValidName = 1;
            }
            else
            {
                printf(" Error: Name should contain only alphabetic characters and spaces.\n");
            }
        }
    }

    int IsValidEmail = 0;
    while(!IsValidEmail)
    {
        printf(" Enter The Email: ");
        scanf("%s", Email);
        if(IsAlreadyExists(Email, 'e', StudentID) > 0)
        {
            printf(" Error: This Email already exists.\n\n");
            IsValidEmail = 0;
        }
        else if(strlen(Email) > 30)
        {
            printf(" Error: Email cannot be more than 30 characters.\n\n");
            IsValidEmail = 0;
        }
        else if(strlen(Email) <= 0)
        {
            printf(" Error: Email cannot be empty.\n\n");
            IsValidEmail = 0;
        }
        else if(strchr(Email, '@') == NULL)
        {
            printf(" Error: Email must contain '@' character.\n\n");
            IsValidEmail = 0;
        }
        else
        {
            IsValidEmail = 1;
        }
    }

    int IsValidPhone = 0;
    while(!IsValidPhone)
    {
        printf(" Enter The Phone: ");
        scanf("%s", Phone);
        if(IsAlreadyExists(Phone, 'p', StudentID) > 0)
        {
            printf(" Error: This Phone Number already exists.\n\n");
            IsValidPhone = 0;
        }
        else if(strlen(Phone) != 11)
        {
            printf(" Error: Phone number must be exactly 11 characters.\n\n");
            IsValidPhone = 0;
        }
        else
        {
            int isDigit = 1;
            for(int i = 0; i < 11; i++)
            {
                if(!isdigit(Phone[i]))
                {
                    isDigit = 0;
                    break;
                }
            }
            if(isDigit)
            {
                IsValidPhone = 1;
            }
            else
            {
                printf(" Error: Phone number must contain only digits.\n\n");
                IsValidPhone = 0;
            }
        }
    }

    int IsValidNumberOfCourse = 0;
    while(!IsValidNumberOfCourse)
    {
        printf(" Number of courses: ");
        scanf("%d", &NumberOfCourses);
        if(NumberOfCourses < 1 || NumberOfCourses > 4)
        {
            printf(" Error: Number of courses must be between 1 and 4.\n\n");
            IsValidNumberOfCourse = 0;
        }
        else
        {
            IsValidNumberOfCourse = 1;
        }
    }

    strcpy(Students[TotalStudents].ID, StudentID);
    strcpy(Students[TotalStudents].Name, Name);
    strcpy(Students[TotalStudents].Phone, Phone);
    strcpy(Students[TotalStudents].Email, Email);
    Students[TotalStudents].NumberOfCourse = NumberOfCourses;
    TotalStudents++;

    for(int i = 0; i < NumberOfCourses; i++)
    {
        printf(" Enter Course %d Code: ", i + 1);
        scanf("%s", CourseCode);

        printf(" Enter Course %d Name: ", i + 1);
        scanf(" %[^\n]s", CourseName);

        strcpy(Courses[TotalCourse].StudentID, StudentID);
        strcpy(Courses[TotalCourse].Code, CourseCode);
        strcpy(Courses[TotalCourse].Name, CourseName);
        TotalCourse++;
    }

    saveStudents();
    saveCourses();

    printf("\n Student Added Successfully.\n\n");
}



void ShowAllStudents()
{
    printf("|==========|====================|==============================|====================|=============|\n");
    printf("|    ID    |        Name        |            Email             |       Phone        |  NO.Course  |\n");
    printf("|==========|====================|==============================|====================|=============|\n");

    for(i=0; i<TotalStudents; i++)
    {
        printf("|");
        printf("%s",Students[i].ID);
        for(j=0; j < (10-strlen(Students[i].ID)); j++)
        {
            printf(" ");
        }
        printf("|");
        printf("%s",Students[i].Name);
        for(j=0; j < (20-strlen(Students[i].Name)); j++)
        {
            printf(" ");
        }
        printf("|");
        printf("%s",Students[i].Email);
        for(j=0; j < (30-strlen(Students[i].Email)); j++)
        {
            printf(" ");
        }
        printf("|");
        printf("%s",Students[i].Phone);
        for(j=0; j < (20-strlen(Students[i].Phone)); j++)
        {
            printf(" ");
        }
        printf("|");
        printf("%d",Students[i].NumberOfCourse);
        for(j=0; j < 12; j++)
        {
            printf(" ");
        }
        printf("|\n");
        printf("|----------|--------------------|------------------------------|--------------------|-------------|\n");

    }
    printf("\n");
}

int SearchStudent(char StudentID[10])
{
    system("cls");
    int StudentFoundIndex = -1;

    int i;
    for(i=0; i<TotalStudents; i++)
    {
        if(strcmp(StudentID,Students[i].ID) == 0)
        {
            StudentFoundIndex = i;
            printf("\n One Student Found for ID: %s\n\n",StudentID);
            printf(" Student Informations\n");
            printf("-------------------------\n");

            printf(" ID:    %s\n",Students[i].ID);
            printf(" Name:  %s\n",Students[i].Name);
            printf(" Email: %s\n",Students[i].Email);
            printf(" Phone: %s\n",Students[i].Phone);
            printf("\n Total Number of Courses: %d\n",Students[i].NumberOfCourse);
        }
    }
    int CourseCount = 0;
    int j;
    for(j=0; j<TotalCourse; j++)
    {
        if(strcmp(StudentID,Courses[j].StudentID) == 0)
        {
            CourseCount++;
            printf(" Course %d Code: %s\n",CourseCount,Courses[j].Code);
            printf(" Course %d Name: %s\n",CourseCount,Courses[j].Name);
        }
    }

    return StudentFoundIndex;
}

void EditStudent(int StudentFoundIndex)
{
    printf("\n\t\t ** Update The New Student **\n\n");

    char NewName[30];
    char NewPhone[14];
    char NewEmail[30];
    int NewNumberOfCourses;
    char StudentID[12];
    strcpy(StudentID, Students[StudentFoundIndex].ID);
    int OldTotalNumberOfCourse = Students[StudentFoundIndex].NumberOfCourse;

    int IsValidName = 0;
    while(!IsValidName)
    {
        printf(" Enter The New Name (0 to skip): ");
        scanf(" %[^\n]s", NewName);
        if(strcmp(NewName, "0") == 0)
        {
            IsValidName = 1;
        }
        else if(strlen(NewName) > 20)
        {
            printf(" Error: Name cannot be more than 20 characters.\n\n");
        }
        else if(strlen(NewName) <= 0)
        {
            printf(" Error: Name cannot be empty.\n\n");
        }
        else
        {
            int isAlpha = 1;
            for(int i = 0; i < strlen(NewName); i++)
            {
                if(!((NewName[i] >= 'A' && NewName[i] <= 'Z') || (NewName[i] >= 'a' && NewName[i] <= 'z') || NewName[i] == ' '))
                {
                    isAlpha = 0;
                    break;
                }
            }
            if(isAlpha)
            {
                IsValidName = 1;
            }
            else
            {
                printf(" Error: Name should contain only alphabetic characters and spaces.\n");
            }
        }
    }

    int IsValidEmail = 0;
    while(!IsValidEmail)
    {
        printf(" Enter The New Email (0 to skip): ");
        scanf("%s", NewEmail);
        if(strcmp(NewEmail, "0") == 0)
        {
            IsValidEmail = 1;
        }
        else if(strlen(NewEmail) > 30)
        {
            printf(" Error: Email cannot be more than 30 characters.\n\n");
        }
        else if(strlen(NewEmail) <= 0)
        {
            printf(" Error: Email cannot be empty.\n\n");
        }
        else if(strchr(NewEmail, '@') == NULL)
        {
            printf(" Error: Email must contain '@' character.\n\n");
        }
        else if(IsAlreadyExists(NewEmail, 'e', StudentID) > 0)
        {
            printf(" Error: This Email already exists.\n\n");
        }
        else
        {
            IsValidEmail = 1;
        }
    }

    int IsValidPhone = 0;
    while(!IsValidPhone)
    {
        printf(" Enter The New Phone (0 to skip): ");
        scanf("%s", NewPhone);
        if(strcmp(NewPhone, "0") == 0)
        {
            IsValidPhone = 1;
        }
        else if(strlen(NewPhone) != 11)
        {
            printf(" Error: Phone number must be exactly 11 characters.\n\n");
        }
        else
        {
            int isDigit = 1;
            for(int i = 0; i < 11; i++)
            {
                if(!isdigit(NewPhone[i]))
                {
                    isDigit = 0;
                    break;
                }
            }
            if(isDigit)
            {
                if(IsAlreadyExists(NewPhone, 'p', StudentID) > 0)
                {
                    printf(" Error: This Phone Number already exists.\n\n");
                }
                else
                {
                    IsValidPhone = 1;
                }
            }
            else
            {
                printf(" Error: Phone number must contain only digits.\n\n");
            }
        }
    }

    int IsValidNumberOfCourse = 0;
    while(!IsValidNumberOfCourse)
    {
        printf(" Number of New courses (1-4, 0 to skip): ");
        scanf("%d", &NewNumberOfCourses);
        if(NewNumberOfCourses > 4 || NewNumberOfCourses < 0)
        {
            printf(" Error: A student can have a maximum of 4 and a minimum of 1 course.\n\n");
        }
        else if(NewNumberOfCourses == 0)
        {
            IsValidNumberOfCourse = 1;
        }
        else
        {
            IsValidNumberOfCourse = 1;
        }
    }

    if(strcmp(NewName, "0") != 0)
    {
        strcpy(Students[StudentFoundIndex].Name, NewName);
    }

    if(strcmp(NewEmail, "0") != 0)
    {
        strcpy(Students[StudentFoundIndex].Email, NewEmail);
    }

    if(strcmp(NewPhone, "0") != 0)
    {
        strcpy(Students[StudentFoundIndex].Phone, NewPhone);
    }

    if(NewNumberOfCourses != 0)
    {
        int FirstCourseIndex;
        for(int dc = 0; dc < TotalCourse; dc++)
        {
            if(strcmp(StudentID, Courses[dc].StudentID) == 0)
            {
                FirstCourseIndex = dc;
                break;
            }
        }

        for(int dc = 0; dc < OldTotalNumberOfCourse; dc++)
        {
            DeleteCourseByIndex(FirstCourseIndex);
        }

        char CourseCode[5];
        char CourseName[30];
        for(int i = 1; i <= NewNumberOfCourses; i++)
        {
            printf(" Enter New Course %d Code: ", i);
            scanf("%s", CourseCode);

            printf(" Enter New Course %d Name: ", i);
            scanf(" %[^\n]s", CourseName);

            strcpy(Courses[TotalCourse].StudentID, StudentID);
            strcpy(Courses[TotalCourse].Code, CourseCode);
            strcpy(Courses[TotalCourse].Name, CourseName);
            TotalCourse++;
        }
        Students[StudentFoundIndex].NumberOfCourse = NewNumberOfCourses;
    }

    saveStudents();
    saveCourses();

    printf(" Student Updated Successfully.\n\n");
}


void DeleteStudent(int StudentIndex)
{
    int d;
    int FirstCourseIndexs;
    struct StudentInfo ThisStudents;
    ThisStudents = Students[StudentIndex];
    for(d=0; d<TotalCourse; d++)
    {
        if(strcmp(ThisStudents.ID,Courses[d].StudentID) == 0)
        {
            FirstCourseIndexs = d;
            break;
        }
    }
    for(d=1; d<=ThisStudents.NumberOfCourse; d++)
    {
        DeleteCourseByIndex(FirstCourseIndexs);
    }
    DeleteStudentByIndex(StudentIndex);
    saveStudents();
    saveCourses();
    printf(" Student Deleted Successfully.\n\n");
    GoBackOrExit();
}

void DeleteAllStudents()
{
    TotalStudents = 0;
    TotalCourse = 0;
    saveStudents();
    saveCourses();
    printf(" All Students Deleted Successfully.\n\n");
    GoBackOrExit();
}


void DeleteCourseByIndex(int CourseIndex)
{
    int c;
    for(c=0; c<TotalCourse-1; c++)
    {
        if(c>=CourseIndex)
        {
            Courses[c] = Courses[c+1];
        }
    }
    TotalCourse--;

}

void DeleteStudentByIndex(int CourseIndex)
{
    int s;
    for(s=0; s<TotalStudents-1; s++)
    {
        if(s>=CourseIndex)
        {
            Students[s] = Students[s+1];
        }
    }
    TotalStudents--;
}


int IsAlreadyExists(char GivenLine[300],char InfoType, char StudentID[300])
{
    int EmailExists = 0;
    int PhoneExists = 0;
    int IDExists = 0;
    int ep;

    for(ep=0; ep<TotalStudents; ep++)
    {
        if(strcmp(GivenLine,Students[ep].ID) == 0)
        {
            IDExists++;
        }
        if(strcmp(GivenLine,Students[ep].Email) == 0 && strcmp(StudentID,Students[ep].ID) != 0 )
        {
            EmailExists++;
        }
        if(strcmp(GivenLine,Students[ep].Phone) == 0 && strcmp(StudentID,Students[ep].ID) != 0)
        {
            PhoneExists++;
        }

    }

    if(InfoType == 'i')
    {
        return IDExists;
    }
    else if(InfoType == 'e')
    {
        return EmailExists;
    }
    else if(InfoType == 'p')
    {
        return PhoneExists;
    }
    else
    {
        return 0;
    }
}

void ErrorAndRestart(char *error[100])
{
    printf("%s\n",error);
    int i = 0;
    printf("Restarting the program: ");
    for(i=0; i<10; i++)
    {
        printf(".");
        Sleep(500);
    }
    system("cls");
    main();
}

void UserGuideline()
{
    printf("\n\t\t ** How it Works? **\n\n");
    printf(" -> You will only able to store the Student's ID, Name, Email, Phone, Number of Courses.\n");
    printf(" -> A student can have maximum 4 courses and minimum 1 course.\n");
    printf(" -> Student ID can be maximum 10 characters long and unique for every students.\n");
    printf(" -> Student Name can be maximum 20 characters long.\n");
    printf(" -> Student Email can be maximum 30 characters long and unique for every students.\n");
    printf(" -> Student Phone can be maximum 20 characters long and unique for every students.\n");
    printf(" -> Course code can be maximum 10 characters long.\n");
    printf(" -> Course Name can be maximum 20 characters long.\n\n");
}

void AboutUs()
{
    {
        printf("\n\t\t ** About US? **\n\n");

        printf("\tVarsity Management System Project\n\n");
        printf("\tOverview\n\n");


        printf("\tThe Varsity Management System is a comprehensive C language project designed\n\tto manage and store student information for a university.\n\tThe system efficiently handles various operations related to student records, \n\tproviding an intuitive interface for users to interact with the database.\n\n");

        printf("\tKey Features\n\n");
        printf("\t1. Student Record Management:\n\n");
        printf("\t  * Add Student: Allows users to add new student records with details such as name, ID,\n\t    department, and courses enrolled.\n");
        printf("\t  * Update Student: Enables updating existing student information.\n");
        printf("\t  * Delete Student: Facilitates the deletion of student records from the system.\n\n");

        printf("\t2. Search Functionality:\n\n");
        printf("\t  * By ID: Users can search for students using their unique ID.\n\n");

        printf("\t3. Display Records:\n\n");
        printf("\t  * All Students: Displays a list of all students in the system.\n\n");
        printf("\tDeveloped By:\n");

        printf("\t\tMd.Samsul Arafin Sazid\n");
        printf("\t\tID:C241025\n");
        printf("\t\tEmail:arafinsazid@gmail.com\n");
        printf("\t\tMobile:01301679136\n\n");

        printf("\t\tJonayed Ahmed\n");
        printf("\t\tID:C241002\n");
        printf("\t\tEmail:jonayedfahimiiuc@gmail.com\n");
        printf("\t\tMobile:01793713965\n\n");

        printf("\t\tJunaid Bin Jahangir\n");
        printf("\t\tID:C241021\n");
        printf("\t\tEmail:jbj.jawad@gmail.com\n");
        printf("\t\tMobile:01635126797\n\n\n\n");

        printf("   NB: This project is made with 'C' language.\n");
    }
}


void GoBackOrExit()
{
    getchar();
    char Option;
    printf(" Go back(b)? or Exit(0)?: ");
    scanf("%c",&Option);
    if(Option == '0')
    {
        ExitProject();
    }
    else
    {
        system("cls");
    }
}

void ExitProject()
{
    system("cls");
    int i;
    char ThankYou[100]     = " ========= Thank You =========\n";
    char SeeYouSoon[100]   = " ========= See You Soon ======\n";
    for(i=0; i<strlen(ThankYou); i++)
    {
        printf("%c",ThankYou[i]);
        Sleep(40);
    }
    for(i=0; i<strlen(SeeYouSoon); i++)
    {
        printf("%c",SeeYouSoon[i]);
        Sleep(40);
    }
    exit(0);
}

void saveStudents()
{
    FILE *file = fopen("students.txt", "w");
    if (file == NULL)
    {
        printf("\n\tError saving student data.\n");
        return;
    }

    fwrite(&TotalStudents, sizeof(int), 1, file);
    fwrite(Students, 200, TotalStudents, file);
    fclose(file);
}

void loadStudents()
{
    FILE *file = fopen("students.txt", "r");
    if (file == NULL)
    {
        printf("\n\tNo student data found. Starting fresh.\n");
        return;
    }

    fread(&TotalStudents, sizeof(int), 1, file);
    fread(Students,200, TotalStudents, file);
    fclose(file);
}

void saveCourses()
{
    FILE *file = fopen("courses.txt", "w");
    if (file == NULL)
    {
        printf("\n\tError saving courses data.\n");
        return;
    }

    fwrite(&TotalCourse, sizeof(int), 1, file);
    fwrite(Courses, 200, TotalCourse, file);
    fclose(file);
}

void loadCourses()
{
    FILE *file = fopen("courses.txt", "r");
    if (file == NULL)
    {
        printf("\n\tNo course data found. Starting fresh.\n");
        return;
    }

    fread(&TotalCourse, sizeof(int), 1, file);
    fread(Courses,200, TotalCourse, file);
    fclose(file);
}
/*void DataSeed()
{


    //-- student 1
    strcpy(Students[0].ID,"S-1");
    strcpy(Students[0].Name,"Student 1");
    strcpy(Students[0].Phone,"016111111111");
    strcpy(Students[0].Email,"student-1@gmail.com");
    Students[0].NumberOfCourse=1;

    strcpy(Courses[0].StudentID,"S-1");
    strcpy(Courses[0].Code,"CSE-1");
    strcpy(Courses[0].Name,"Course - 1");

    //-- student 2
    strcpy(Students[1].ID,"S-2");
    strcpy(Students[1].Name,"Student 2");
    strcpy(Students[1].Phone,"016111111112");
    strcpy(Students[1].Email,"student-2@gmail.com");
    Students[1].NumberOfCourse=2;

    strcpy(Courses[1].StudentID,"S-2");
    strcpy(Courses[1].Code,"CSE-1");
    strcpy(Courses[1].Name,"Course - 1");

    strcpy(Courses[2].StudentID,"S-2");
    strcpy(Courses[2].Code,"CSE-2");
    strcpy(Courses[2].Name,"Course - 2");


    //-- student 3
    strcpy(Students[2].ID,"S-3");
    strcpy(Students[2].Name,"Student 3");
    strcpy(Students[2].Phone,"016111111113");
    strcpy(Students[2].Email,"student-3@gmail.com");
    Students[2].NumberOfCourse=3;

    strcpy(Courses[3].StudentID,"S-3");
    strcpy(Courses[3].Code,"CSE-1");
    strcpy(Courses[3].Name,"Course - 1");

    strcpy(Courses[4].StudentID,"S-3");
    strcpy(Courses[4].Code,"CSE-2");
    strcpy(Courses[4].Name,"Course - 2");

    strcpy(Courses[5].StudentID,"S-3");
    strcpy(Courses[5].Code,"CSE-3");
    strcpy(Courses[5].Name,"Course - 3");

    TotalStudents = 3;
    TotalCourse = 6;
}*/
//end mainpage functions..
