#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* 

  * * * * * * * * * * * * * * * * * * * * * *
*   SUBMISSION FOR PUROLATOR'S HACKATON 2021  *
*               by JOSE GEORGES               *
*                                             *  
*         SECRET SANTA GENERATOR in C         *
  * * * * * * * * * * * * * * * * * * * * * * 

*/

#define MINIMUM_PARTICIPANTS 3

/* represents each individual participating in the secret santa */
struct participant
{
    char name[100];
    char email[100];
};

// fgets adds a \n char after the user finishes entering input
void remove_new_line(char* string)
{
    size_t length = strlen(string);
    if((length > 0) && (string[length-1] == '\n'))
    {
        string[length-1] ='\0';
    }
}

// printf doesn't add a new line automatically
void add_new_line()
{
    printf("\n");
}

// A utility function to swap two values
void swap (struct participant *a, struct participant *b)
{
    struct participant temp = *a;
    *a = *b;
    *b = temp;
}

// A function to generate a random permutation of arr[]
void randomize( struct participant arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
        add_new_line();

        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

void send_email_to_participant(struct participant personGifting, struct participant personReceiving) {
    char cmd[500];  // to hold the command.
    sprintf(cmd,"printf \"Subject: Secret Santa!!\nPlease buy a gift for %s.\" | msmtp %s --tls-certcheck=off", personReceiving.name, personGifting.email); // prepare command.
    system(cmd);   
}
 
int main(int argc, char *argv[]) {
    add_new_line();
    puts("Welcome to my secret santa generator!");
    add_new_line();

    int participantsArgumentNotPresent = 0;
    long participantsCount;
    struct participant participants[100];
    struct participant secretSantas[100];

    // getting values from the parameters
    while (*argv != NULL)
    {
            if(strcmp(*argv, "-p") == 0 || strcmp(*argv, "-participants") == 0) {
                participantsArgumentNotPresent = 1;
            }
            participantsCount = atol(*argv);
            argv++;
    }

    /* Retrieve the amount of participants */
    if(participantsArgumentNotPresent == 0) {
        char *p, s[100];
        puts("Please enter the amount of participants:");
        while (fgets(s, sizeof(s), stdin)) {
            participantsCount = strtol(s, &p, 10);
            if (p == s || *p != '\n') {
                puts("Please enter an integer: ");
            } else if (participantsCount < MINIMUM_PARTICIPANTS) {
                printf("The minimum amount of participants is %d, please enter a number greater or queal than %d: ", MINIMUM_PARTICIPANTS, MINIMUM_PARTICIPANTS);
                add_new_line();
            } else break;
        }
        add_new_line();
    }

    printf("Playing with %ld parcitipants", participantsCount);
    add_new_line();
    add_new_line();

    // /* Retrieve the information of each participant */
    for(int x=0;x<participantsCount;x++) {
        printf("Please enter participant\'s name (%d out of %ld): ", x+1, participantsCount);
        fgets(participants[x].name, sizeof(participants[x].name), stdin);
        remove_new_line(participants[x].name);
        printf("Please enter %s\'s email: ", participants[x].name);
        fgets(participants[x].email, sizeof(participants[x].email), stdin);
        remove_new_line(participants[x].email);
        
        printf("Participant entered: %s (%s)", participants[x].name, participants[x].email);
        add_new_line();
        add_new_line();
    }

    /* Assign a person from the group to each participant */
    for(int x=0;x<participantsCount;x++) {
        secretSantas[x] = participants[x];
    }
    randomize(secretSantas, participantsCount);


    /* Email each participant who they got */
    for(int x=0;x<participantsCount;x++) {
        send_email_to_participant(participants[x], secretSantas[x]);
    }

    puts("Emails have been sent. Have fun playing and don't be cheap! =)");

    return 0;
}