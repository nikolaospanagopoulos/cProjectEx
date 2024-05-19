#include "students.h"
#include <stdio.h>
#include <stdlib.h>

void printMenu() {
  printf("1. Add student\n");
  printf("2. Delete student by ID\n");
  printf("3. Search student by ID\n");
  printf("4. Update student by ID\n");
  printf("5. Print all students\n");
  printf("6. Save and exit\n");
}

void printStudent(student st) { printf("ID: %lu, Name: %s\n", st.id, st.name); }

void printStudents(list l) {
  node current = l->head;

  while (current) {
    printStudent(l->head->data);
    current = current->next;
  }
}

Result load(char *filename, list *l) {
  FILE *file = fopen(filename, "rb");

  // If the file doesn't exist, create it and initialize an empty list
  if (file == NULL) {
    *l = malloc(sizeof(struct listR)); // Correct memory allocation
    if (*l == NULL)
      return MALLOC_ERR;
    (*l)->head = NULL;
    (*l)->tail = NULL;

    // Create the file in write mode to ensure it exists
    file = fopen(filename, "wb");
    if (file == NULL)
      return F_WRITE_ERR;
    fclose(file);

    return NO_ERR;
  }

  // If the file exists, read its contents into the list
  *l = malloc(sizeof(struct listR)); // Correct memory allocation
  if (*l == NULL)
    return MALLOC_ERR;
  (*l)->head = NULL;
  (*l)->tail = NULL;

  student st;
  while (fread(&st, sizeof(student), 1, file)) {
    addStudent(st, *l);
  }

  fclose(file);
  return NO_ERR;
}

Result save(char *filename, list l) {
  FILE *file = fopen(filename, "wb");
  // in case we cannot open file
  if (!file) {
    return F_WRITE_ERR;
  }
  node current = l->head;
  while (current) {
    fwrite(&current->data, sizeof(student), 1, file);
    current = current->next;
  }
  // close file to free memory
  fclose(file);
  return NO_ERR;
}

Result addStudent(student st, list l) {
  node newN = (node)malloc(sizeof(struct nodeR));
  if (!newN) {
    return MALLOC_ERR;
  }
  // add student to list
  newN->data = st;
  newN->next = NULL;
  newN->previous = l->tail;

  // in case tail exists we add it next to it and then newN is the tail
  if (l->tail) {
    l->tail->next = newN;
  } else {
    // means list was empty so the head is the newN
    l->head = newN;
  }
  // the newN is the tail
  l->tail = newN;
  return NO_ERR;
}
Result findStudentNode(unsigned long id, list l, node *np) {
  node current = l->head;
  while (current != NULL) {
    if (current->data.id == id) {
      *np = current;
      return NO_ERR;
    }
    current = current->next;
  }
  if (!*np) {
    printf("student not found with id: %lu\n", id);
  }
  *np = NULL;
  return NO_ERR;
}

Result findStudent(unsigned long id, list l, student *st) {
  node np;
  Result res = findStudentNode(id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    *st = np->data;
  } else {
    st->id = -1;
  }
  return NO_ERR;
}
Result deleteStudentById(unsigned long id, list l) {
  node np;
  Result res = findStudentNode(id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    if (np->previous != NULL) {
      np->previous->next = np->next;
    } else {
      l->head = np->next;
    }
    if (np->next != NULL) {
      np->next->previous = np->previous;
    } else {
      l->tail = np->previous;
    }
    free(np);
  } else {
    printf("student doesn't exist with id: %lu \n", id);
  }
  return NO_ERR;
}
Result updateStudent(student st, list l) {
  node np;
  Result res = findStudentNode(st.id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    np->data = st;
  } else {
    return SYNTAX_ERR;
  }
  return NO_ERR;
}
// Function to check if there was an error
int isError(Result r) { return r != NO_ERR; }

// Function to generate a pseudo-random and unique ID
unsigned long generateId() { return (unsigned long)rand(); }
