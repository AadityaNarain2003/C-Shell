[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# About

This project is about building your own c shell with custom commands.


Author: @aaditya_narain
Date of completion: 13th Sep,2023.


# Description

The code is made modular where different files contain different commands. Though, there is a lot of redundancy. 
I have used multiple structs to store data and also a bin.txt that stores the pastevents commands.

The code does all the necessary functions of Part A, B and C.

Most of the commands have their own code and header files. 
There are some test directories and files such as test_dir and out.txt.
bin.txt is the file with 


# Assumptions

1. Prompt- I am assuming that prompt gives ~ for only the case where it matches completely with the home directory

2. warp- I am assuming that all commands of the fashion X/X/X are seperate commands

3. peek- No assumptions made

4. pastevents- I am assuming with execute indexes that the latest command is labelled 1 and the furthest 15

5. Foreground and Background processes have been done the way they should run in a normal Ubuntu terminal

6. proclore- will only output for valid pids

7. seek - no assumptions made

8. I/O redirect and piping are working the way they should in terminal

9. activites- assuming that S and T mean sleeping. Using a linkedlist to store the data

10. signals- am assuming that control D ony works when no foreground process is running

11. fg and bg are running as given

12. neonate is working as given, no assumptions made

13. iman - the only assumption i have made is that the commands given in man1/ is getting printed

14. other assumptions include- that piping with | or ending with & start a process in foreground without any error.

