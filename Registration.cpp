/*
 Group 1

  Yahir Hernandez-Aguilar
  Damodien Johnson
  Josh Ayioka
  Connor Malosky
*/


// Program to implement a class registration system

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int STUIDSIZE = 5;  // Maximum size for a student ID
const int COURSEIDSIZE = 9;  // Maximum size for a course ID

const char STUFILE[] = "students.txt";
const char COURSEFILE[] = "courses.txt";

// Structure used to define a course registration
struct Course
{
    char id[COURSEIDSIZE];
    int section;
    int credit;
};

// Type definition to define the data type for the list data
typedef Course ListItemType;

// Node structure
struct Node
{
    ListItemType item;
    Node* next;
};

// Structure used to associate a student with the classes they are taking
struct Student
{
    char id[STUIDSIZE];
    Node* courses;
};

int searchStudent(Student students[], int startPos, int endPos, const char studentID[]);
void getHeadCount(int& headCount);
void loadStudents(Student students[], const int headCount);
void loadCourses(Student students[], const int headCount); 
void displayStudentStruct(Student students[], const int headCount);
void removeCourse(Student students[], int selectedStu); 
bool validStudentSelected(const bool attemptedToFind, const int selectedStudent);
static void displaySchedule(Node* head);

int main()
{
    // Menu choice option
    int choice;


    // Updates how many students was read from the file
    int headCount = 0;
    getHeadCount(headCount);

    // Dynamic array of students structure created
    Student* students = new Student[headCount];

    // load the students into the dynamic array
    loadStudents(students, headCount);

    //load the courses in the dynamic array
    loadCourses(students, headCount);

    // Display the whole struct. TESTING PURPOSES
    displayStudentStruct(students, headCount);



    /*
     If a student is found, selectedStudent will be the numerical array position they are located in.
     If selectedStudent is still -1, then no student was found in the array (invalid userID was typed in).

     attemptedToFind means if the user at least attempted to find a userID, valid or invalid.

     Meaning, if attemptedToFind is false, the user never inputted a stuID in the first place, so "No student selected"
     should be displayed. If the user did attempt to find a userID, then attemptedToFind would be set to true.

     If attemptedToFind was set to true, and selectedStudent is -1, then we do nothing, just as the demo shows.
     If attemptedToFind was set to true, and selectedStudent is any other number, then we execute
     the actual operation, just as the demo shows.
    */

    int selectedStudent = -1;
    bool attemptedToFind = false;

    // character array for whatever the user types in
    const int MAXUSERINPUT = 200;
    char userInput[MAXUSERINPUT];


    /*
     Tests to see the student structure created at this point in the program (TEST purposes only)
     To see the student structure, un-comment the two lines of code below
    */

    //displayStudentStruct(students, headCount);
    //cout << endl << endl << endl;

    do
    {
        cout << "1. Select Student by ID\n";
        cout << "2. Display Selected Student's Schedule\n";
        cout << "3. Add a Course to Student's Schedule\n";
        cout << "4. Remove a Course from Student's Schedule\n";
        cout << "5. Exit\n";
        cout << endl;
        cout << "Select: ";

        // Gets the numeric entry from the menu
        cin >> choice;

        // Makes the 'enter' key that was pressed after the numeric entry be ignored
        cin.ignore();

        switch (choice)
        {
        case 1:

            cout << endl << "Enter Student ID: ";
            cin.getline(userInput, MAXUSERINPUT);

            // Regardless if the user typed a valid user ID or not, note that an attempt was made to do so
            attemptedToFind = true;

            // find the array position the student is in the students array
            selectedStudent = searchStudent(students, 0, headCount - 1, userInput);

            if (selectedStudent == -1)
            {
                cout << "Student ID not found" << endl << endl;
            }
            else
            {
                cout << "Student selected " << endl << endl;
            }
            break;

        case 2:
            displaySchedule(students[selectedStudent].courses);
            break;
        case 3:
            break;
        case 4:

            if (validStudentSelected(attemptedToFind, selectedStudent))
            {
                removeCourse(students, selectedStudent);
            }
            break;
        }

    } while (choice != 5);


    // deletes the student structure array inside the HEAP
    delete[] students;


    return 0;
}


/*
The functions you write MUST have detailed
comments above their declarations explaining
1) the purpose of the function
2) the purpose of each parameter the function requires
3) the purpose of the return value, if the function has one.
Follow the examples for the functions you've been given in past assignments.
*/

/*
 This function returns the amount of students that are in a file.
 The value that is updated is a referenced value in main.
 This function is used to determine how big our dynamic array must be
 AND the number of students there are in a file.

 By definition of the file structure in the assignment tab,
 we simply count how many lines there are in the file, and getline() does that in
 a while loop. At the end, we put back the file marker back to the beginning of the file.
*/
void getHeadCount(int& headCount)
{
    ifstream inFile;
    inFile.open(STUFILE);

    if (!inFile.is_open())
    {
        cerr << "\nERROR: Input file cannot be opened.";
        exit(1);
    }

    // stores an entire line, which means a line existed, so take note
    char countLine[STUIDSIZE];

    // Reads lines until the end of the file is reached
    while (inFile.getline(countLine, STUIDSIZE))
        ++headCount;

    // resets the cursor to be back at the beginning of the file and clear EOF state
    inFile.clear();
    inFile.seekg(0, ios::beg);

    inFile.close();
}

/*
 This function loads the student ID's read from a file into our student structure array.
 We need to be given the students array structure to update accodingly, and how many students there are.
 We only work with how many students existed in the file, and we go through line by line in the file
 and update the appropriate index in the students array structure. At the end, we set
 every courses to NULL, which will be handled by another function.

 The process will look like this

 0 A101 | NULL
 1 A200 | NULL
 2 B300 | NULL
 and so on...
*/

void loadStudents(Student students[], const int headCount)
{
    ifstream inFile;
    inFile.open(STUFILE);

    if (!inFile.is_open())
    {
        cerr << "\nERROR: Input file cannot be opened.";
        exit(1);
    }

    // to store ID characters
    char stuID[STUIDSIZE];
    int i = 0;

    // run for all students that existed in the file
    while (i != headCount)
    {
        //store the ID in the temp array
        inFile.getline(stuID, STUIDSIZE);

        // copy that array in temp into the students array structure at i, up to STUIDSIZE - 1 characters
        strncpy(students[i].id, stuID, STUIDSIZE - 1);

        // unlike getline(), strncpy does not append \0, so we'll manually add it just in case
        students[i].id[STUIDSIZE - 1] = '\0';

        // we only care about adding student ID's. Adding courses must be done by another function
        // (this ensures the function doesn't do too much tasks)
        students[i].courses = NULL;
        ++i;
    }

    // resets the cursor to be back at the beginning of the file and clear EOF state
    inFile.clear();
    inFile.seekg(0, ios::beg);

    inFile.close();

}






//DAMODIEN JOHNSON'S PART BELOW: loadCourses

/* This function reads the file of courses listed and loades each course name, section, and credit
hours of each student. The functions needs botht he amount of students available and the student 
array itself to excecute. After caucously opening the file (still runs if not), the function creates
a new node and assigns each read line from the course list until reaching "END"; otherwise either moving 
to the next student or ending the function when we run out of file to read*/
void loadCourses(Student students[], const int headCount)
{
    //Open course file
    ifstream inFile;
    inFile.open(COURSEFILE);

    //Error handle
    if (!inFile.is_open())
    {
        cerr << "\nERROR: Input file cannot be opened.";
        return;
    }

    //To store course info characters
    char courseId[COURSEIDSIZE];

    int i = 0;                                              //current student we're looking at
    Node* head = NULL;                                      //head of current students list
    Node* current = NULL;                                   //points to the last node in the current students list


    //Starts reading until end of the file
    while (i < headCount && inFile.getline(courseId, COURSEIDSIZE))
    {

        //Checking to see if the read line is END, if it is; go onto
        //the next student

        if (strcmp(courseId, "END") == 0)                           //strcmp: compares two string parameters, if they
                                                                    //do not differ, then will return '0'.
        {
            students[i].courses = head;

            head = NULL;                                            //these declarations resets the pointers back
            current = NULL;                                         //to NULL, preventing them from pointing to the
                                                                    //previous list addresses

            i++;
        }
        else 
        {
            //read the rest of the course (section + credit)
            int section, credit;
            inFile >> section;
            inFile >> credit;
            inFile.ignore(1000, '\n'); //clearing newline so next getline works

            //creating new node
            Node* temp = new Node;
            temp->next = NULL;

            //copy course ID using strncpy
            strncpy(temp->item.id, courseId, COURSEIDSIZE);
            temp->item.id[COURSEIDSIZE - 1] = '\0';

            //store section and credit
            temp->item.section = section;
            temp->item.credit = credit;

            //attach node to the end of the list
            if (head == NULL)
            {
                head = temp;
                current = temp;
            }
            else
            {
                current->next = temp;
                current = temp;
            }
        }



    }

    inFile.close();


}







/*
 Displays the student structure to the screen.
 We need to be given students array structure to print to screen and how many students there are in the file.

 FOR TESTING PURPOSES ONLY
*/
void displayStudentStruct(Student students[], const int headCount)
{
    for (int i = 0; i < headCount; i++)
    {
        cout << students[i].id << " | ";

        Node* curr = students[i].courses;

        if (curr == NULL)
        {
            cout << "NO COURSES";
        }
        else
        {
            while (curr != NULL)
            {
                cout << curr->item.id << " "
                    << curr->item.section << " "
                    << curr->item.credit;

                if (curr->next != NULL)
                    cout << " -> ";

                curr = curr->next;
            }
        }
        cout << endl;
    }
    cout << endl << "Number of students in the file " << headCount << endl;
}



/*
 This function finds a student in the student array via binary search.
 We must know where to start in the given students array and where to end.
 We will then compare the passed studentID to a string in the array.

 If a student is found, we return the int position in the students array.
 Otherwise, we return -1 to indicate they are not in the array
*/
int searchStudent(Student students[], int startPos, int endPos, const char studentID[])
{
    // Base Case 1
    if (startPos > endPos)
        return -1;

    // Base Case 2
    int mid = (startPos + endPos) / 2;
    int compared = strcmp(students[mid].id, studentID);

    // a 0 indicates that the compared strings are identical
    if (compared == 0)
        return mid;

    // Recusive cases
    // a returned positive value from strcmp indicates that the current
    // string in the students array is "higher" up than the target, so
    // search the lower half. Vice versa for returned negative values

    /*
    A101
    A102
    B200
    B300
    D490

    As an abstract example, strcmp("B200","A102") returns a positive value
    because B200 is higher on the list than A102, so search the lower half to find "A102"
    (note that this is not how strcmp actually works, but its an abstract thought)
    */
    if (compared > 0)
        return searchStudent(students, startPos, mid - 1, studentID);
    else
        return searchStudent(students, mid + 1, endPos, studentID);






}//DAMODIEN JOHNSON'S PART BELOW: removeCourse


/*removeCourse function asks and removes the requested course from one student
 chosen earlier in the program. This function uses two parameters, the student structure
 and selected students, to find out whihc student we're working with. It then, ater asking
 which course to delete, goes through a loop to find and target the requested node to delete*/
void removeCourse(Student students[],int selectedStudent) {

    //if student has no courses, display "student has not signed up for any classes"
   
    if (students[selectedStudent].courses == NULL)
    {
        cout << "Student has not signed up for any classes";
    }

    else
    {
        int courseOption;
        //ask which course they want to remove
        cout << "Choose course to remove: ";
        cin >> courseOption;
        cin.ignore();


        //validate the option
            if (courseOption < 1)
            {
                cout << "Invalid option./n" << endl;
            }
            else
            {
                Node* head = students[selectedStudent].courses;

                if (head == NULL)
                {
                    cout << "Student has no courses to remove./n" << endl;
                }

                else if (courseOption == 1)
                {
                    //remove the head node
                    Node* toDelete = head;
                    students[selectedStudent].courses = head->next;
                    delete toDelete;
                    cout << "Course removed./n" << endl;
                }
                else
                {
                    //initializes a node that walks to the node BEFORE the one we want to delete,
                    //this helps us check and use ->next correctly later
                    int pos = 1;
                    Node* prev = head;

                    while (prev != NULL && pos < (courseOption - 1))
                    {
                        prev = prev->next;
                        pos++;
                    }

                    //if prev is NULL or there is no node after prev, the option was too big.
                    if (prev == NULL || prev->next == NULL)
                    {
                        cout << "Invalid option./n" << endl;
                    }
                    else
                    {
                        //deletes the next node from the prev node hat walked ahead and checked
                        Node* toDelete = prev->next;
                        prev->next = toDelete->next;
                        delete toDelete;
                        cout << "Course removed./n" << endl;
                    }
                }
            }
    }


}

/*
 This function mainly performs the same if-branch menu found
 at 2, 3, and 4 operations. The passed parameters need to be
 the bool value on whether the user at least attempted to seach a user ID,
 and the selected student supposedly found in the student array.

 Returned false values display the message according to their conditions, indicating
 an invalid student was selected.Otherwise a returned true value means that a valid student has been selected
*/
bool validStudentSelected(const bool attemptedToFind, const int selectedStudent)
{
    if (!attemptedToFind)
    {
        cout << "No student selected\n\n";
        return false;
    }
    else if (selectedStudent == -1)
    {
        cout << endl << endl << endl;
        return false;
    }

    return true;
}

/*
This function simply displays the courses that the selected student
signed up for and a message if they did not sign up for any. It does
so by pulling the values directly out of the linked list and printing
them to the screen.
*/
static void displaySchedule(Node* head) {
    int courseNum = 1;
    Node* temp = head;
    if (temp == NULL)
        cout << "Student has not signed up for any classes";
    while (temp != NULL) {
        cout << courseNum << "." << temp->item.id << "  " << "Sect: " << temp->item.section << "  " << "Credit: " << temp->item.credit;
        cout << endl;
        temp = temp->next;
        courseNum++;
    }
    cout << endl;

}
