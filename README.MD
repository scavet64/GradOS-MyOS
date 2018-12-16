# My Grad OS Project
This is my custom operating system for the Fall 2018 Graduate Operating Systems Class.
It is developed in c and is a standalone.

## Building

The make file was tailored to my environment so it would copy the resulting ISO into my share directory. This allowed me to test on my Host Machine very efficiently. The Make file will compile all of the c files and link them together. Finally it will inject the grub bootloader and build a final ISO file that can be used in VirtualBox.

```sh
make clean
make all
```

## Usage example

When the operating system is first launched, you will be presented with a login screen. There are several default users on the system to test with.

| Username | Password  |
|----------|-----------|
| ADMIN    | PASSWORD  |
| Edward   | fullmetal |
| ZeroTwo  | franxx    |
| LeLouch  | geass     |

Choose any of the example users above and login.

Once you are logged in, several commands are available to you.

The Max input length that a user can enter is 250 characters.

**Commands are case sensitive**

- Arithmetic
    - ADD param1 param2
        - Adds param 1 and param2 and displays the result
    - SUB param1 param2
        - Subtracts param 2 from param1 and displays the result
    - MULT param1 param2
        - Multiplies param 1 and param2 and displays the result
    - DIV param1 param2
        - Integer Divides param 1 and param2 and displays the result.

- Users:
    - USER REGISTER param1
        - Creates a new user account with the name of param1.
    - USER PASSWORD param1
        - Modifies the password of the account matching param1
    - USER LIST
        - Lists the current users that the system knows about
    - LOGOUT
        - This will log the user out of their session

- Misc:
    - CLEAR
        - This will clear the terminal window
    - END
        - This will END the session

## Testing Steps

1. Launch the OS
1. Enter `ADMIN` as the username and press the enter key
1. Enter `PASSWORD` as the password and press the enter key
1. Enter `ADD 2 2` and press the enter key. Verify the output is correct
1. Enter `ADD 2 -5` and press the enter key. Verify the output is correct
1. Enter `ADD -5 2` and press the enter key. Verify the output is correct
1. Enter `ADD` and press the enter key. Verify there is an error shown
1. Enter `ADD 3` and press the enter key. Verify there is an error shown
1. Enter `SUB 2 2` and press the enter key. Verify the output is correct
    - **Note** At this point the screen should start scrolling
1. Enter `SUB 2 -5` and press the enter key. Verify the output is correct
1. Enter `SUB -5 2` and press the enter key. Verify the output is correct
1. Enter `SUB` and press the enter key. Verify there is an error shown
1. Enter `SUB 3` and press the enter key. Verify there is an error shown
1. Enter `MULT 2 2` and press the enter key. Verify the output is correct
1. Enter `MULT 2 -5` and press the enter key. Verify the output is correct
1. Enter `MULT -5 2` and press the enter key. Verify the output is correct
1. Enter `MULT` and press the enter key. Verify there is an error shown
1. Enter `MULT 3` and press the enter key. Verify there is an error shown
1. Enter `DIV 2 2` and press the enter key. Verify the output is correct
1. Enter `DIV 2 -5` and press the enter key. Verify the output is correct
1. Enter `DIV -5 2` and press the enter key. Verify the output is correct
1. Enter `DIV` and press the enter key. Verify there is an error shown
1. Enter `DIV 3` and press the enter key. Verify there is an error shown
1. Enter `DIV 3 0` and press the enter key. Verify there is an error shown
1. Enter `USER LIST` and press the enter key. Verify a list of users is shown
1. Enter `USER PASSWORD ZeroTwo` and press the enter key. The system should ask for a new password
1. Enter `newpass` and press the enter key. The system should now give you a success message
1. Enter `LOGOUT` and press the enter key. The system should now log you out and prompt for a new username and password
1. Enter `ZeroTwo` as the username and press the enter key
1. Enter `newpass` as the password and press the enter key. You should be logged in as ZeroTwo now
1. Enter `CLEAR` and press the enter key. The screen should clear
1. Enter `END` and press the enter key. The system will halt

## Known Limitations/Issues

* Commands are case sensitive
* Buffer overflow can occur during arithmetic functions (Handled in atoi function)
* Without malloc, there are a few hacky methods in order to handle parsing of the command
* Creating a new user is not working due to malloc related issues. The data being entered into the array is being overwritten.