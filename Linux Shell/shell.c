 #include <unistd.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <time.h>
 #include <sys/wait.h>
 #include <sys/utsname.h>
 #include <fcntl.h> //for open() flags, file creation permissions

#define MAX_IN 64

//prototype
int createChild();
void helpScreenPrint();
void timeBomb();
void quotes();

int main(int argc, char ** argv){
   char userIn[MAX_IN]; 
   pid_t id; //int
   char filePath[1096]; //for cwd
   time_t mytime; //long, for d/t 
   struct tm * timeinfo; //tm: struct which stores date and time; timeinfo holds d&t after populated
   struct utsname unameData;


      while(1){
         printf("\nchristian@coolestTerminal: "); //starting prompt
         fgets(userIn, MAX_IN, stdin); //take in user input with a restriction of 64 characters
         userIn[strcspn(userIn, "\n")] = '\0'; //replaces \n with \0 (\n is produced when user hits enter to submit there terminal command)

         //close command to end app
         if(strcmp(userIn, "done") == 0){
            //printf("\nGoodbye\n");
            //exit(0);
            break;
         }
         else if(strcmp(userIn, "help") == 0){
            helpScreenPrint(); //prints help screen w/ list of available commands
            continue;
         }

         id = createChild();
         if (id == 0){

            if(strcmp(userIn, "cwd") == 0){
               getcwd(filePath, sizeof(filePath));
               printf("\nWorking Directory: %s\n", filePath); //prints working directory (includes filepath)
               }

            else if(strcmp(userIn, "ls") == 0){
               execl("/usr/bin/ls", "/usr/bin/ls", (char *)NULL); //list files in directory
               //printf("%d", getpid());
               exit(0);
               }

            else if(strcmp(userIn, "clr") == 0){
               printf("\033[H\033[J"); //clears terminal, \033[H: moves cursor 1st col - 1st row, \033[J: clears screen from the cursor onward
               }

            else if(strcmp(userIn, "d/t") == 0){
               time (&mytime); //time function retrieves current calendar time, stores in mytime
               timeinfo = localtime (&mytime); //converts mytime to local time
               printf("Current local date and time: %s", asctime (timeinfo)); //converts timeinfo into readable (natural language) date & time
               }

            else if(strcmp(userIn, "createf") == 0){
               printf("\nFile Name: ");
               char fileName[FILENAME_MAX]; //initialize file name
               fgets(fileName, FILENAME_MAX, stdin); //take in user input for name of file
               fileName[strcspn(fileName, "\n")] = '\0'; //removes \n from string name
               int file = open(fileName, O_RDWR | O_CREAT, 0644); //open file with Read/Write permissions OR create if non existent; 0644 = Owner: R/W, others: Read Only
               
               if(file == -1){
                  perror("Creation of file failed");
               }
               printf("%s has been created.\n", fileName);
               }

            else if(strcmp(userIn, "uname") == 0){
               uname(&unameData); 
               printf("\n%s\n", unameData.sysname);
               }

            else if(strcmp(userIn, "oops") == 0){
               timeBomb();
               }

            else if(strcmp(userIn, "quote") == 0){
                quotes();
               }

         else{
            printf("\n%s, not recognized please try again\n", userIn);
            exit(0);
             }
         exit(0);
         }
      }
}

//function to fork, create child process, and confirm creation or lack there of (error)
int createChild() {
   pid_t my_id = fork();

   switch(my_id){
      case -1:
         perror("**Child Process Creation Failed**");
         return -1;
      case 0:
         //printf("\nChild Process created; PID: %d\n", getpid());
         return 0;
      default:
         waitpid(my_id, NULL, 0);
         return my_id;
   }
}

void helpScreenPrint(){
   printf(
      "\n*******Available Commands*******\n\n"
      "done: to close application\n"
      "cwd: to check current Working Directory\n"
      "ls: to check files within current directory\n"
      "clr: to clesr terminal\n"
      "d/t: to produce current local date & time\n"
      "createf: create a file within current directory\n"
      "oops: call this command only under dire circumstances\n"
      "rizz: use this command if you're single and ready to mingle\n");

}

void quotes() {
    const char *pickuplines[] = {
        "💬 Are you a gigabit WiFi signal? Because I feel a strong connection. 😉🦾",
        "💬 Are you made of copper and tellurium? Because you're Cu-Te. 😘",
        "💬 Are you a computer keyboard? Because you're just my type. ⌨🙈",
        "💬 You must be a software update because you've just improved my life's performance. ❤️‍🔥",
        "💬 I must have a bad cache because I can't stop thinking about you. 🤒"
    };

    srand(time(NULL));
    int index = rand() % 5;
    printf("%s\n", pickuplines[index]);
}

void timeBomb() {
    for (int i = 5; i > 0; i--) {
        printf("💣 Self-destruct in %d...\n", i);
        sleep(1);
    }
    printf("💥 SYSTEM FAILURE. Terminal shutting down...\n");
    exit(0);
}



