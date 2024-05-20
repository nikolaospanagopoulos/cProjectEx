#include "students.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  char *filename = argv[1];
  list studentList;
  Result res = load(filename, &studentList);
  if (isError(res)) {
    printf("Error loading file.\n");
    return 1;
  }

  int choice;
  while (1) {
    printMenu();
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 6) {
      break;
    }

    student st;
    unsigned long id;
    switch (choice) {
    case 1:
      printf("Enter student name: ");
      scanf("%s", st.name);
      st.id = generateId();
      res = addStudent(st, studentList);

      if (isError(res)) {
        printf("Error adding student.\n");
      }
      break;
    case 2:
      printf("Enter student ID to delete: ");
      scanf("%lu", &id);
      res = deleteStudentById(id, studentList);
      if (isError(res)) {
        printf("Error deleting student.\n");
      }
      break;
    case 3:
      printf("Enter student ID to search: ");
      scanf("%lu", &id);
      res = findStudent(id, studentList, &st);
      if (isError(res)) {
        printf("Error finding student.\n");
      } else if (st.id == (unsigned long)-1) {
        printf("Student not found.\n");
      } else {
        printStudent(st);
      }
      break;
    case 4:
      printf("Enter student ID to update: ");
      scanf("%lu", &id);
      res = findStudent(id, studentList, &st);
      if (isError(res) || st.id == (unsigned long)-1) {
        printf("Student not found.\n");
      } else {
        printf("Enter new student name: ");
        scanf("%s", st.name);
        res = updateStudent(st, studentList);
        if (isError(res)) {
          printf("1\n");
        }
      }
      break;
    case 5:
      printStudents(studentList);
      break;
    default:
      printf("Invalid choice.\n");
    }
  }

  res = save(filename, studentList);

  // free memory of nodes
  while (studentList->head) {
    node cur = studentList->head->next;
    free(studentList->head);
    studentList->head = cur;
  }
  // free list memory
  if (studentList) {
    free(studentList);
  }

  if (isError(res)) {
    printf("Error saving file.\n");
  }

  return 0;
}
