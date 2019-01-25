# threaded-tape

## please use raw mode to read

a program that will read different parts of a tape. The tape itself is represented as a 1 dimensional byte array of n size.The user will input commands that will allow the tape to move back and forth by k steps. Each step will output to a file the data being read.

Each head is placed at an offset from the base or starting point of the tape (index 0), this offset is specified by the user. You are tasked with writing the command handler that will invoke functions related to tape head creation. Each head will write to its own respective file. If it is thread 1, it will write out to ./head1, thread 2 will write out to ./head2 and so on.

The main thread needs to wait until all threads have finished reading and writing out to a file. If it attempts to run another command while it is still reading/writing, it could compromise this process, therefore the main thread needs to wait until all other threads have completed and all threads will need to wait until the main thread runs a command.

Your program will use a filename from command line arguments and you will need to read in the binary data. You may treat the data as unsigned bytes.

If no filename is specified in the command line arguments the program should output:

Tape Not Inserted
If a file does not exist

Cannot Read Tape

The output format when a read head is set HEAD:

HEAD <id> at <offset>
When a READ command is ran, the program needs to output when it has finished reading.:

Finished Reading
Important:

The tape file itself a binary file of n bytes. No other information or data is provided with it.

The output of the file should just contain a stream of bytes that was read from the tape and should be rereadable as a tape itself.

pthreads and mutex locks for synchronisation between threads on this task.

Assumptions/Clarifications:

Offset from head can be negative, you will need to use the modulus operator (beware of an overflow).
You may assume that a thread will always start and a file will be created for that thread.
Commands:

HEAD <offset: signed, relative to the base>
READ <offset: signed, relative to the cursor, cursor starts at base>
QUIT
Examples

Example 1 Invalid File:

Tape Not Inserted
Example 2 File Does Not Exist:

Cannot Read Tape
Example 3 Head Example:

HEAD 50
HEAD 1 at +50

HEAD -20
HEAD 2 at -20
Example 4 Read:

READ 4000
Finished Reading

READ -2000
Finished Reading
