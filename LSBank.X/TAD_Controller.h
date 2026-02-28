#ifndef TAD_CONTROLLER_H
#define	TAD_CONTROLLER_H

#define NEW_DAY "> LSBank - New Day!\n"

#define OPEN_EXTERIOR_DOOR "> LSBank - Open exterior door\n"
#define CLOSE_EXTERIOR_DOOR "> LSBank - Close exterior door\n"
#define OPEN_INTERIOR_DOOR "> LSBank - Open interior door\n"
#define CLOSE_INTERIOR_DOOR "> LSBank - Close interior door\n"
#define OPEN_BOTH_DOORS "> LSBank - Open both doors\n"
#define CLOSE_BOTH_DOORS "> LSBank - Close both doors\n"

#define ENTER_PIN "> LSBank - Enter PIN: "
#define RESET_SYSTEM "> LSBank - Reset System: "
#define EXIT_REQUEST "> LSBank - Exit Requested: "

#define PERMISSION_DENIED "> LSBank - Permission denied\n"
#define THIEF_INTERCEPTED "> LSBank - Thief intercepted\n"

#define PASSWORD "0308YAN"
#define YES "Yes"
#define NO "No"

void Controller_Init (void);

void motorController (void);

#endif