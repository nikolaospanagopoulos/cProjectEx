#include "students.h"
#include <stdio.h>
#include <stdlib.h>

// εκτυπώνουμε το μενού
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
    printStudent(current->data);
    current = current->next;
  }
}

Result load(char *filename, list *l) {
  FILE *file = fopen(filename, "rb");

  // Άν το αρχείο δεν υπάρχει, το φτιάχνουμε και φτιάχνουμε μία άδεια λίστα
  if (file == NULL) {
    *l = malloc(sizeof(struct listR));
    if (*l == NULL) {
      return MALLOC_ERR;
    }
    (*l)->head = NULL;
    (*l)->tail = NULL;

    // φτιάχνουμε το αρχείο και ελέγχουμε οτι υπάρχει
    file = fopen(filename, "wb");
    if (file == NULL) {
      return F_WRITE_ERR;
    }
    // κλείνουμε το αρχείο γιατί ειναι ´αδειο
    fclose(file);

    return NO_ERR;
  }

  // άν το αρχείο υπάρχει, φτιάχνουμε μια λίστα
  *l = malloc(sizeof(struct listR));
  if (*l == NULL) {
    return MALLOC_ERR;
  }
  (*l)->head = NULL;
  (*l)->tail = NULL;

  // βάζουμε στη λίστα , τα δεδομένα απο το αρχείο
  student st;
  while (fread(&st, sizeof(student), 1, file)) {
    addStudent(st, *l);
  }

  // κλείνουμε το αρχείο για να ελευθερώσουμε μνήμη
  fclose(file);
  return NO_ERR;
}

Result save(char *filename, list l) {
  FILE *file = fopen(filename, "wb");
  // Δείχνουμε error  σε περίπτωση που δεν μπορούμε να δημιουργήσουμε το αρχείο
  if (!file) {
    return F_WRITE_ERR;
  }
  // προσωρινή μεταβλητή που περιέχει κάθε (student) που γράφουμε στο αρχείο
  node current = l->head;
  while (current) {
    fwrite(&current->data, sizeof(student), 1, file);
    current = current->next;
  }
  // κλείνουμε το αρχείο για να ελευθερώσουμε μνήμη
  fclose(file);
  return NO_ERR;
}

Result addStudent(student st, list l) {
  // φτιάχνουμε ένα καινούριο node στη λίστα
  node newN = (node)malloc(sizeof(struct nodeR));
  if (!newN) {
    return MALLOC_ERR;
  }
  // βάζουμε στη λίστα τον μαθητή
  newN->data = st;
  newN->next = NULL;
  newN->previous = l->tail;

  // σε περίπτωση που η λίστα δεν είναι άδεια, βάζουμε τον καινούριο μαθητή στο
  // τέλος και τον κάνουμε το καινούριο tail
  if (l->tail) {
    l->tail->next = newN;
  } else {
    // η λίστα ήταν άδεια και ο μαθητής είναι πλέον και το head και το tail της
    // λίστας
    l->head = newN;
  }
  l->tail = newN;
  return NO_ERR;
}
Result findStudentNode(unsigned long id, list l, node *np) {
  // προσωρινή μεταβλητή
  node current = l->head;
  while (current != NULL) {
    // ελέγχουμε κάθε μαθητή στη λίστα μέχρι να βρούμε αυτόν που το id του
    // ταιριάζει σε αυτό που περάσαμε στο function σαν όρισμα
    if (current->data.id == id) {
      *np = current;
      return NO_ERR;
    }
    // το current δείχνει πλέον στον επόμενο μαθητή
    current = current->next;
  }
  if (!*np) {
    printf("student not found with id: %lu\n", id);
  }
  *np = NULL;
  return NO_ERR;
}

Result findStudent(unsigned long id, list l, student *st) {
  // μεταβλητή που θα περιέχει το node που περιέχει τον μαθητή που ψάχνουμε
  node np;
  Result res = findStudentNode(id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    // βάζουμε στο όρισμα sp τον μαθητή που υπάρχει μέσα στο np node
    *st = np->data;
  } else {
    // σε περίπτωση που δεν τον βρούμε, βάζουμε το -1 σαν id
    st->id = -1;
  }
  return NO_ERR;
}
Result deleteStudentById(unsigned long id, list l) {
  // node που θα περιέχει το student που θέλουμε να διαγράψουμε
  node np;
  // βρίσκουμε το σωστό node και το βάζουμε στο np
  Result res = findStudentNode(id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    if (np->previous != NULL) {
      // αν υπάρχει προηγούμενο node
      // κάνουμε το προηγούμενο node στη λίστα να δείχνει στο επόμενο ( μετά απο
      // αυτό που θέλουμε να διαγράψουμε )
      np->previous->next = np->next;
    } else {
      // σε περίπτωση που δεν υπάρχει προηγούμενο node , τοτε το head της λίστας
      // απλά δείχνει στο επόμενο
      // από αυτό που θέλουμε να διαγράψουμε
      l->head = np->next;
    }
    if (np->next != NULL) {
      // αν υπάρχει επόμενο τότε το προηγούμενο του επομένου δείχνει στο
      // προηγούμενο από αυτό που θέλουμε να διαγράψουμε
      np->next->previous = np->previous;
    } else {
      l->tail = np->previous;
    }
    // ελευθερώνουμε μνήμη
    free(np);
  } else {
    printf("student doesn't exist with id: %lu \n", id);
  }
  return NO_ERR;
}
Result updateStudent(student st, list l) {
  // node που θα περιέχει τον μαθητή που θέλουμε να κάνουμε update
  node np;
  Result res = findStudentNode(st.id, l, &np);
  if (res != NO_ERR)
    return res;
  if (np != NULL) {
    // το node που βρήκαμε πλέον θα περιέχει τον μαθητή που περνάμε σαν όρισμα
    // στη συνάρτηση
    np->data = st;
  } else {
    return SYNTAX_ERR;
  }
  return NO_ERR;
}
// συνάρτηση που ελέγχει αν το Result r είναι error ή όχι
int isError(Result r) { return r != NO_ERR; }

// συνάρτηση που δημιουργεί ενα "τυχαίο" id
unsigned long generateId() { return (unsigned long)rand(); }
