# PingPong
It's a simulation of two different process of OS(Linux) communicating with each other and playing ping pong.

# How to run
Open two terminals on your OS.
On the first one, use this command to compile the program "gcc ping.c -o ping && gcc pong.c -o pong". Then enter "./pong" on the first one and you will get the process id of the pong.
On the second terminal, use the command "./ping <process id of pong>" and you will see the communication between them. 
