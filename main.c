#include "students.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    // ελέγχουμε αν οι παράμετροι που περνάμε στο πρόγραμμα ειναι 2 (executable
    // και όνομα αρχείου)
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  // παίρνουμε το όνομα του αρχείου από τις παραμέτρους του προγράμματος
  char *filename = argv[1];
  // δημιουργούμε μια list που θα γεμίσουμε ανοίγοντας το αρχείο
  list studentList;
  Result res = load(filename, &studentList);
  if (isError(res)) {
    printf("Error loading file.\n");
    return 1;
  }

  // Επιλογές χρήστη
  int choice;
  while (1) {
    printMenu();
    printf("Enter choice: ");
    // βάζουμε την επιλογή του χρήστη στο choice
    scanf("%d", &choice);

    if (choice == 6) {
      break;
    }

    student st;
    unsigned long id;
    switch (choice) {
    case 1:
      // περίπτωση 1: προσθήκη μαθητή στη λίστα
      printf("Enter student name: ");
      scanf("%s", st.name);
      st.id = generateId();
      res = addStudent(st, studentList);

      if (isError(res)) {
        printf("Error adding student.\n");
      }
      break;
    case 2:
      // περίπτωση 2: διαγραφή μαθητή από τη λίστα
      printf("Enter student ID to delete: ");
      scanf("%lu", &id);
      res = deleteStudentById(id, studentList);
      if (isError(res)) {
        printf("Error deleting student.\n");
      }
      break;
    case 3:
      // περίπτωση 3: αναζήτηση μαθητή με βάση το id
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
      // περίπτωση 4: ανανέωση στοιχείων μαθητή
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
      // περίπτωση 5: εκτύπωση στοιχείων όλων των μαθητών
      printStudents(studentList);
      break;
    default:
      printf("Invalid choice.\n");
    }
  }

  // αποθηκεύουμε το αρχείο
  res = save(filename, studentList);

  // ελευθερώνουμε τη μνήμη για κάθε node
  while (studentList->head) {
    node cur = studentList->head->next;
    free(studentList->head);
    studentList->head = cur;
  }
  // ελευθερώνουμε τη μνήμη για την λίστα
  if (studentList) {
    free(studentList);
  }

  if (isError(res)) {
    printf("Error saving file.\n");
  }

  return 0;
}
