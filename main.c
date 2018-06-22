#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3 *db = NULL;
char *errMsg = {0};

void showMenu() {
  printf("MENU:\n");
  printf("0 - the oldest student.\n" );
	printf("1 - student with max sum of marks.\n");
	printf("2 - sum of marks by course.\n");
	printf("3 - average score by course.\n");
	printf("4 - average score by group.\n");
	printf("5 - EXIT. \n");
}

void sql_execute(char command[500]){
  if(sqlite3_exec(db, command, 0, 0, &errMsg) != SQLITE_OK){
    printf("SQL error: %s\n", errMsg );
    sqlite3_free(errMsg);
  }
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s : %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void select_oldest(){
  char command[800] = {0};
  sprintf(command, "select Course,Name from StudentInfo inner join Units on UnID=UnitID \
where birthYear=(select min(birthYear) from StudentInfo inner join Units on UnID=UnitID \
where Course=1) or birthYear=(select min(birthYear) from StudentInfo inner join Units \
on UnID=UnitID where Course=2) group by Course;");
  sqlite3_exec(db, command, callback, 0, &errMsg);

}

void select_sum(){
  char command[800] = {0};
  sprintf(command, "select Course, sum(Mark) from (StudentInfo inner join Units)\
inner join Marks on idSt = StID and UnID = UnitID group by Course;");
  sqlite3_exec(db, command, callback, 0, &errMsg);

}

void select_avg_course(){
  char command[800] = {0};
  sprintf(command, "select Course, avg(Mark) from (StudentInfo inner join Units) inner join Marks on idSt = StID and UnID = UnitID group by Course;");
  sqlite3_exec(db, command, callback, 0, &errMsg);
}

void select_avg_group(){
  char command[800] = {0};
  sprintf(command, "select Groupp, avg(Mark) from (StudentInfo inner join Units) inner join Marks on idSt = StID and UnID = UnitID group by Groupp;");
  sqlite3_exec(db, command, callback, 0, &errMsg);
}

int main(int argc, char* argv[]) {
   if(sqlite3_open("students.db", &db)) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return 1;
   }
   int cur = -1;
   while (cur != 5) {
     showMenu();
     scanf("%d", &cur);
     switch (cur) {
       case 0:
         select_oldest();
         break;
       case 1:
         select_sum();
         break;
       case 2:
         select_sum();
         break;
       case 3:
         select_avg_course();
         break;
       case 4:
         select_avg_group();
         break;
       default:
         break;
     }

   }
   sqlite3_close(db);
   return 0;
}
