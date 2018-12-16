#ifndef USERS_H
#define USERS_H

int checkUserData(char *username, char *password);
int addUserData(char *username, char *password);
int changeUserPassword(char *username, char *password);
void listAllUsers();

#endif