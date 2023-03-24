#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <conio.h>
#include <wincon.h>
#include <afxres.h>

// GLOBAL VARIABLES:
// Global variables are not best practise. Instead, you should pass arguments in functions
// and return values. Use malloc() for bigger structures that can't be returned and free()
// after you done with them. As C doesn't have a garbage collector.
// When I created this project I didn't knew these best practises, so I built it with the
// assumption that the fewer declarations and the fewer times code is repeated, the best.

char **token = NULL;
char *pointer = NULL;
char currentWorkingDirectory[350] = {};
char userInput[1000] = {};
double programVersion = 1.0;

struct dirent *dir = NULL;
FILE *fileRead = NULL, *fileWrite = NULL;
DIR *d = NULL;

void welcomeScreen();
void currentDirectory();
int tokenizer();
void showDirectory(char *path);
void changeDirectory(char *path);
void list();
void help(char *command);
void wrong();
void create(char *fileName);
void readfile(char *fileName);
void del(char *fileName);
void move();
void echo();
void type();
void sort(char *file);
void tree(char *path, int base);
void clearScreen(HANDLE hConsole);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {

    HANDLE handle = NULL;
    bool terminateProgram = false;
    const char *availableCommands = "list help dir ls cd move create read del echo type sort tree cls exit";
    int tokenCounter;

    welcomeScreen();

    while (terminateProgram == false) {

        currentDirectory();
        printf("<Shelldom v%0.1f> %s>",programVersion, currentWorkingDirectory);
        tokenCounter = tokenizer();

        switch (tokenCounter)
        {
            case 0:
                printf("You need to type a command! Type \"list\" for available commands!");
                break;

            case 1:
                if (strcmp(token[0], "exit") == 0)
                    terminateProgram = true;
                else if (strcmp(token[0], "list") == 0)
                    list();
                else if (strcmp(token[0], "help") == 0)
                    printf("Help must be followed by an available command, type \"list\"!\n");
                else if(strcmp(token[0], "dir") == 0 || strcmp(token[0], "ls") == 0)
                    showDirectory(currentWorkingDirectory);
                else if(strcmp(token[0], "cd") == 0)
                    printf("Current Directory:\n\t\t%s\n", currentWorkingDirectory);
                else if (strcmp(token[0], "create") == 0)
                    printf("Create must be followed by a [file_name].");
                else if(strcmp(token[0], "tree") == 0)
                    tree(currentWorkingDirectory, 0);
                else if (strcmp(token[0], "cls") == 0) {
                    handle = GetStdHandle(STD_OUTPUT_HANDLE);
                    clearScreen(handle);
                    welcomeScreen();
                }
                else
                    wrong();
                break;

            case 2:
                if (strcmp(token[0], "dir") == 0 || strcmp(token[0], "ls") == 0){
                    if (strcmp(token[1], "-l") == 0)
                        showDirectory(currentWorkingDirectory);
                    else
                        showDirectory(token[1]);
                }
                else if (strcmp(token[0], "cd") == 0)
                    changeDirectory(token[1]);
                else if (strcmp(token[0], "help") == 0 && strstr(availableCommands, token[1]) != NULL)
                    help(token[1]);
                else if (strcmp(token[0], "create") == 0 && token[1] != NULL)
                    create(token[1]);
                else if (strcmp(token[0], "read") == 0 && token[1] != NULL)
                    readfile(token[1]);
                else if (strcmp(token[0], "del") == 0 && token[1] != NULL)
                    del(token[1]);
                else if (strcmp(token[0], "sort") == 0)
                    sort(token[1]);
                else if(strcmp(token[0], "tree") == 0)
                    tree(token[1], 0);
                else
                    wrong();
                break;

            case 3 ... 5:
                if (strcmp(token[0], "dir") == 0 || strcmp(token[0], "ls") == 0 && tokenCounter == 3)
                    showDirectory(token[2]);
                else if (strcmp(token[0], "create") == 0 && strcmp(token[1],"-b") == 0 && token[2] != NULL)
                    create(token[2]);
                else if (strcmp(token[0], "read") == 0 && strcmp(token[1],"-b") == 0 && token[2] != NULL)
                    readfile(token[2]);
                else if (strcmp(token[0], "move") == 0 && tokenCounter == 3){
                    move();
                }
                else if (strcmp(token[0], "echo") == 0) {
                    if (tokenCounter == 4 && strcmp(token[2], ">") == 0)
                        echo();
                    else
                        wrong();
                }
                else if (strcmp(token[0], "type") == 0 && strcmp(token[1], "/a") == 0 &&
                         strcmp(token[2], "*.*") == 0 && strcmp(token[3], ">") == 0 && tokenCounter == 5) {
                    type();
                }
                else
                    wrong();
                break;

            default:
                wrong();
        }

        printf("\n");
    }
    free(token);
    return 0;
}

void welcomeScreen() {
    printf("\t\t********************************************************\n");
    printf("\t\t**       __        ___            __   __   v1.0      **\n");
    printf("\t\t**      /__` |__| |__  |    |    |  \\ /  \\  |\\/|      **\n");
    printf("\t\t**      .__/ |  | |___ |___ |___ |__/ \\__/  |  |      **\n");
    printf("\t\t**        A custom CLI software (for windows)         **\n");
    printf("\t\t**        A project for \"Operating Systems I\"         **\n");
    printf("\t\t**           by Anestis Zioulis (AEM: 4247)           **\n");
    printf("\t\t**    Student at Int. Hellenic University (Greece)    **\n");
    printf("\t\t********************************************************\n");
    printf("\t\t        Type \"list\" for available commands!\n\n");
}

void currentDirectory() {

    /*
     * Shows the current working directory, before the cursor.
     */

    strcpy(currentWorkingDirectory, "");
    getcwd(currentWorkingDirectory, 350);
    strcat(currentWorkingDirectory, "\\");

}

int tokenizer() {

    /*
     * Splits userInput into tokens, counts them and returns that value.
     */

    const char *whiteSpace = " \t\n\f\r\v";
    int i, tokenCounter;

    fgets(userInput, 1000, stdin);

    for (i = 0; i < sizeof(userInput); i++) { //Make all user input into lower case chars.
        userInput[i] = (char) tolower(userInput[i]);
    }

    i = 0;
    tokenCounter = 1;         //Assume the user is going to give a command.
    if(userInput[0] == '\n'){  //In case he presses enter (empty command), change counter to 0.
        tokenCounter=0;
    }
    while (userInput[i] != '\0'){ //Count the words inside the string by counting spaces.
        if (userInput[i++] == ' ')
            tokenCounter++;
    }

    token = (char **) malloc((tokenCounter + 1) * sizeof(char *)); //Allocate memory for token in order to save tokens in it
    pointer = strtok(userInput, whiteSpace); //Create the first token

    i = 0;
    while (pointer != NULL) { //Save tokens for each word until you encounter NULL
        token[i++] = pointer;
        pointer = strtok(NULL, whiteSpace);
    }

    if (tokenCounter == 0) {
        token[1] = NULL;
    }

    return tokenCounter;
}

void showDirectory(char *path) {

    /*
     * Displays the content of the current directory or the <path> provided.
     */

    char directory[350] = {0};
    strcpy(directory,path);

    d = opendir(directory);

    if (!path) {
        d = opendir(".");
    }
    else if (d) {
        d = opendir(directory);
    }
    else {
        printf("Please type an existing dir!\n");
    }

    while ((dir = readdir(d)) != NULL) { //displays the files and folders in a vertical list.
        if(strcmp(token[1], "-l") == 0)
            printf("  %s\t", dir->d_name);
        else
            printf("  %s\n", dir->d_name);
    }
    printf("\n");
    closedir(d);
}

void changeDirectory(char *path) {

    /*
     * Changes the current working directory, if it exists.
     */

    char directory[350] = {0};
    strcpy(directory,path);

    d = opendir(directory);

    if (d) {
        chdir(directory);
        while ((dir = readdir(d)) != NULL) { //Displays the files and folders in a horizontal list.
            printf("  %s\t", dir->d_name);
        }
        printf("\n");
    }
    else {
        printf("Please type an existing dir!\n");
    }

    closedir(d);
}

void list() {

    /*
     * It prints a list of the available commands in screen.
     */

    printf("\nFor more information on a specific command, type \"HELP [command_name]\"\n"
           "\nDIR/LS\tDisplays a column or list of files and subdirectories in a directory."
           "\nMOVE\tMoves a file from one directory to another."
           "\nECHO\tAppends an alphanumeric value, if followed by the redirection char [>],\n"
           "\tinto the file provided. Asks for permission if the file exists."
           "\nTYPE\tCombines all the text files in the current directory into one file.\n"
           "\tIf the file exists asks for confirmation."
           "\nSORT\tSorts 32 numbers (byte size) from a binary file and prints them on screen."
           "\nLIST\tPrints a list with the available commands and a short description."
           "\nHELP\tProvides extra info for a command."
           "\nCD\tChanges working directory or prints the current working directory."
           "\nCREATE\tCreates a text or binary file with numbers from 1-255."
           "\nREAD\tReads text or binary files."
           "\nDEL\tDeletes a file, similar files or all files, on the current directory."
           "\nTREE\tDisplays the structure of the current folder, sub-folders and files."
           "\nCLS\tClear screen (cls), clears the screen buffer to reduce clutter."
           "\nEXIT\tQuits the program.\n"
    );
}

void help(char *command) {

    /*
     * It prints helpful text and examples, for each command, in screen.
     */

    if (strcmp(command, "dir") == 0 || strcmp(command, "ls") == 0) {
        printf("\nDIR/LS can be followed by [-l] and can be followed by a [path]!\n\n"
               "ex : \"dir\"\n"
               "ex : \"ls\"\n"
               "ex : \"dir -l c:\"\n"
               "ex : \"ls c:/deep/\"\n"
               "ex : \"dir ../\"\n"
               "ex : \"ls -l ../linux/impostor\"\n");
    }
    else if (strcmp(command, "move") == 0) {
        printf("\nMOVE must be followed by [old_folder/file_name] and [new_folder]!\n\n"
               "ex : \"move c:/mario.txt d:/adventures/\"\n"
               "ex : \"move princess.jpg f:/img/\"\n"
               "ex : \"move ../monition.pre E:/nd/is/near/\"\n");
    }
    else if (strcmp(command, "echo") == 0) {
        printf("\nECHO must be followed by an [alphanumeric_value], the redirection char [>]\n\n"
               "and a [file_name]!\n\n"
               "ex : \"echo john55 > about.txt\"\n"
               "ex : \"echo AbigSTRINGwithText99 > sandiego.txt\"\n");
    }
    else if (strcmp(command, "type") == 0) {
        printf("\nTYPE must be followed by [/a] [*.*] [>] and a [file_name]!\n\n"
               "ex : \"type /a *.* > about.txt\"\n"
               "ex : \"type /a *.* > YotaFive.txt\"\n");
    }
    else if (strcmp(command, "sort") == 0) {
        printf("\nSORT must be followed by a binary [file_name]!\n\n"
               "ex : \"sort numbers.bin\"\n"
               "ex : \"sort pokerface.bin\"\n");
    }
    else if (strcmp(command, "list") == 0) {
        printf("\nLIST is functioning as is!\n\n"
               "ex : \"list\"\n");
    }
    else if (strcmp(command, "cd") == 0) {
        printf("\nCD can be used as is or followed by a [path]!\n\n"
               "ex : \"cd\"\n"
               "ex : \"cd f:/\"\n"
               "ex : \"cd ..\"\n"
               "ex : \"cd ../backToTheFuture/\"\n");
    }
    else if (strcmp(command, "create") == 0) {
        printf("\nCREATE can be followed by an option [-b] and a [file_name]!\n\n"
               "ex : \"create -b data.bin\"\n"
               "ex : \"create -b binaryFile\"\n"
               "ex : \"create textFileWithoutExtensionIsBad\"\n"
               "ex : \"create numbers.txt\"\n");
    }
    else if (strcmp(command, "read") == 0) {
        printf("\nREAD must be followed by a [file]!\n\n"
               "ex : \"read data.bin\"\n"
               "ex : \"read numbers.txt\"\n");
    }
    else if (strcmp(command, "del") == 0) {
        printf("\nDEL must be followed by a [file] or [file*] or [*.*]!\n\n"
               "ex : \"del data.bin\"\n"
               "ex : \"del dat*\"\n"
               "ex : \"del *.*\"\n");
    }
    else if (strcmp(command, "tree") == 0) {
        printf("\nTREE is functioning as is!\n\n"
               "ex : \"tree\"\n");
    }
    else if (strcmp(command, "cls") == 0) {
        printf("\nCLS is functioning as is!\n\n"
               "ex : \"cls\"\n");
    }
    else if (strcmp(command, "exit") == 0) {
        printf("\nEXIT is functioning as is!\n\n"
               "ex : \"exit\"\n");
    }
    else {
        printf("\nHelp must be followed by a command.\n\n"
               "ex : \"help move\"\n"
               "ex : \"help dir\"\n"
               "Type \"list\" to show available commands\n");
    }
}

void wrong() {
    printf("\t\tWrong command! Type \"list\" for all commands!");
}

void create(char *filename) {

    /*
     * It creates text or binary files and saves integer numbers
     * from 1-255 in them, until 0 is provided.
     */

    bool isBinaryFile = false;
    int i=0;

    currentDirectory();
    strcat(currentWorkingDirectory, filename);
    printf("Type only numbers from 1-255, or 0 to exit, followed by enter!\nnumbers:\n");

    if (strstr(filename,".bin") != NULL || strcmp(token[1],"-b") == 0)
        isBinaryFile = true;

    if (isBinaryFile)
        fileWrite = fopen(currentWorkingDirectory, "w");
    else
        fileWrite = fopen(currentWorkingDirectory, "wb");

    if (fileWrite != NULL){
        scanf("%d",&i);

        while(i != 0){
            if(i > 0 && i <= 255){
                if (isBinaryFile){
                    fwrite(&i, sizeof(char), 1, fileWrite);
                }
                else {
                    fprintf(fileWrite,"%d\n",i);
                }
            }
            else{
                printf("Only numbers between 1-255 are accepted!\n");
            }
            scanf("%d",&i);
        }
        fclose(fileWrite);
    }
    else{
        printf("Can't open the file: %s\n",filename);
    }
    printf("Procedure completed!\n");
    fflush(stdin);
}

void readfile(char *fileName) {

    /*
     * It reads and prints numbers from text or binary files.
     * It checks the file extension to change modes.
     */

    bool isBinaryFile = false;

    currentDirectory();
    strcat(currentWorkingDirectory, fileName);
    if (strstr(fileName,".bin") != NULL || strcmp(token[1],"-b") == 0)
        isBinaryFile = true;

    if (isBinaryFile == true)
        fileRead = fopen(currentWorkingDirectory, "rb");
    else
        fileRead = fopen(currentWorkingDirectory, "r");

    if (fileRead != NULL)
    {
        unsigned char c;

        if (isBinaryFile == true) {
            while(fread(&c, sizeof(char), 1, fileRead) == 1)
            {
                printf("%d ", c);
            }

        } else {
            while(fscanf(fileRead,"%c",&c) == 1)
            {
                printf("%c", c);
            }
        }
        printf("\n");
        fclose(fileRead);
    }
    else printf("\tCan't open the file: \"%s\".\n",fileName);
}

void del(char *fileName) {

    /*
     * It deletes file/s on the current folder.
     * If provided with a full file name, it only deletes that file.
     * If provided with a * in the file name, it deletes all similar files with the string before the *.
     * If provided with *.* it deletes all files in the current folder.
     */

    char *e;
    char file[250] = {0};
    int index, removeCheck;

    currentDirectory();
    strcpy(file,fileName);

    //If fileName contain * it deletes multiple files
    if(strchr(fileName, '*')){
        e = strchr(fileName, '*');
        index = (int)(e - fileName);

        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                //skip "." and ".." (folder hierarchy, they are not deletable).
                if (strcmp(dir->d_name,".") == 0 && strcmp(dir->d_name,"..") == 0)
                    continue;
                //if filename == *.* deletes all files in the current folder
                if(!strcmp(fileName,"*.*")){
                    strcpy(file, currentWorkingDirectory);
                    strcat(file, "\\");
                    strcat(file, dir->d_name);
                    removeCheck = remove(file);
                    if (removeCheck)
                        printf("Can't remove: [%s]!\n",dir->d_name);
                    else
                        printf("File [%s] removed!\n",dir->d_name);
                }

                //delete similar files to the filename
                if(strncmp(dir->d_name, fileName, index) == 0 && fileName[0] != '*'){
                    strcpy(file, currentWorkingDirectory);
                    strcat(file, "\\");
                    strcat(file, dir->d_name);
                    removeCheck = remove(file);
                    if (removeCheck)
                        printf("Can't remove: [%s]!\n",dir->d_name);
                    else
                        printf("File [%s] removed!\n",dir->d_name);
                }
            }
        }
        closedir(d);
    } else { //delete file with a specific name
        strcpy(file, currentWorkingDirectory);
        strcat(file,fileName);
        removeCheck = remove(file);
        if (removeCheck)
            printf("Can't remove file: %s\n",fileName);
        else
            printf("File [%s] removed!\n",fileName);
    }
}

void move() {

    /*
     * It moves files from an initial folder to a target folder.
     */

    //check for backslashes.
    if (strstr(token[1], "\\") != NULL || strstr(token[2], "\\") != NULL) {
        printf("You must use forward slash '/' for paths.\n");
        return;
    }

    char initialDirectory[350] = {0};
    char startPath[350] = {0};
    char fileName[150] = {0};
    char endPath[350] = {0};
    char endFile[350] = {0};
    char option[1] = {0};
    char *del;
    char c;
    int i, removeCheck;

    currentDirectory();
    strcpy(initialDirectory, currentWorkingDirectory);
    strcpy(startPath, token[1]);
    strcpy(endPath, token[2]);

    if(startPath[0] == '/'){
        strcpy(startPath,".");
        strcat(startPath, token[1]);
    }
    if(endPath[0] == '/'){
        strcpy(endPath,".");
        strcat(endPath, token[2]);
    }

    //splitting file name from directory path.
    if (strstr(startPath,"./") != NULL || startPath[1] == ':'){
        del = &startPath[strlen(startPath)];
        i = *del;
        while (del > startPath && *del != '/') {
            del--;
            if (*del != '/')
                fileName[i] = *del;
            i++;
        }
        if (*del == '/')
            *del = '\0';
        strrev(fileName);
        strcat(startPath,"/");
    }
    else {
        strcpy(fileName, token[1]);
        strcpy(startPath, currentWorkingDirectory);
    }
    chdir(startPath);
    currentDirectory();
    strcpy(startPath, currentWorkingDirectory);

    chdir(initialDirectory);
    int dirExists=chdir(endPath);
    if (dirExists != 0){
        printf("Please type an existing path or directory to move the file to!\n");
        return;
    }

    currentDirectory();
    strcpy(endPath, currentWorkingDirectory);
    strcpy(endFile, endPath);
    strcat(endFile, fileName);

    if(access(endFile, F_OK ) != -1 ) {
        printf("File exists! Do you want to (R)eplace it or (C)ancel this action?\n ?> ");
        scanf("%c", option);
        (char) tolower(option[0]);
        getchar();
        if (strcmp(option, "c") == 0) {
            printf("Procedure canceled.");
            return;
        }
    }

    //moving the file.
    chdir(startPath);
    d = opendir(startPath);
    if (d) {
        while ((dir = readdir(d)) != NULL) {

            if (strcmp(dir->d_name, fileName) == 0) {
                fileRead = fopen(dir->d_name, "rb");
                fileWrite = fopen(endFile, "wb");
                while ((dir = readdir(d)) != NULL) {
                    c = (char) fgetc(fileRead);
                    while (c != EOF)
                    {
                        fputc(c, fileWrite);
                        c = (char) fgetc(fileRead);
                    }
                }
                fclose(fileRead);
                fclose(fileWrite);
            }
        }
    }
    closedir(d);

    //deleting the original file.
    chdir(startPath);
    if(access(startPath, F_OK ) != -1 ) {
        removeCheck = remove(fileName);
        if (removeCheck == 0)
            printf("File moved successfully!\n");
        else
            printf("Error moving file!\n");
    }
    chdir(initialDirectory);
}

void echo() {

    char option[1] = {0};

    if(access(token[3], F_OK ) != -1 ) {
        printf("\tFile %s already exists!\nDo you want to append it? (Y/N) :", token[3]);
        scanf("%s", option);
        option[0] = (char) tolower(option[0]);
        getchar();
        if (strcmp(option, "n") == 0){
            printf("Procedure canceled.\n");
            return;
        }
        printf("File was modified.\n");
    }
    fileWrite = fopen(token[3], "a");
    fprintf(fileWrite, "%s", token[1]);
    fclose(fileWrite);

}

void type() {

    /*
     * Copies all the text files of the current directory to the target file.
     * If it exists it asks for permission to overwrite/append or cancel.
     */

    char filePath[350] = {0};
    char option[1] = {0};
    char c;

    currentDirectory();
    strcpy(filePath, currentWorkingDirectory);
    strcat(filePath, token[4]);

    if(access(filePath, F_OK ) != -1 ) {
        printf("\tFile \"%s\" already exists!\nWhat do you want to do with it?\n(R)eplace / (A)ppend / (C)ancel?\nI want to: ", token[4]);
        scanf("%s",&option[0]);
        option[0] = (char) tolower(option[0]);
        getchar();
        if (strcmp(option, "r") == 0) {
            printf("\nFile will be Replaced!");
            fileWrite = fopen(token[4], "w");
        }
        else if (strcmp(option, "a") == 0) {
            printf("\nFile will be Appended!");
            fileWrite = fopen(token[4], "a");
        }
        else if (strcmp(option, "c") == 0) {
            printf("\nProcedure is canceled!");
            return;
        }
        else {
            printf("\nWrong command please try again!");
            return;
        }
    }
    else {
        printf("\nFile does not exist..\n..Creating a new file!\n");
        fileWrite = fopen(token[4], "w");
    }
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name,".txt") != 0 && strcmp(dir->d_name, token[4]) != 0) {
                fileRead = fopen(dir->d_name, "r");
                c = (char) fgetc(fileRead);
                while (c != EOF)
                {
                    fputc(c, fileWrite);
                    c = (char) fgetc(fileRead);
                }
                fputc('\n',fileWrite);
                fclose(fileRead);
            }
        }
        fclose(fileWrite);
    }
    closedir(d);
    printf("\nProcedure completed!\n");
}

void sort(char *file) {

    /*
     * import the numbers from the binary file into an int array, sorting them.
     */

    char filePath[350] = {0};
    unsigned char c;
    int i, j, sortedArray[32] = {0}, number, value;

    currentDirectory();
    strcat(filePath, currentWorkingDirectory);
    strcat(filePath, file);

    fileRead = fopen(filePath, "rb");
    if (fileRead != NULL) {

        for (i = 0; i < 32; i++) {
            if (fscanf(fileRead, "%c", &c) == 1)
                value = c;
            else
                value = 0;
            sortedArray[i] = value;
        }

        for (j = 0; j < 32; ++j) {
            for (i = j + 1; i < 32; ++i) {
                if (sortedArray[j] > sortedArray[i]) {
                    number = sortedArray[j];
                    sortedArray[j] = sortedArray[i];
                    sortedArray[i] = number;
                }
            }
        }
        printf("Sorted Array (Ascended)\n");

        for (i = 0; i < 32; i++) {
            printf("Array[%d]-> Number:%d\n", i + 1, sortedArray[i]);
        }
        fclose(fileRead);
    }
    else
        printf("File: %s doesn't exist or can't open it!\n", token[1]);
}

void tree(char *path, int base) {

    /*
     * It shows the structure of the current directory and subdirectories.
     */

    char directory[350] = {0};
    int i;

    strcpy(directory, path);

    DIR *d2 = opendir(directory); // does not work if global

    if (!d2)
        return;

    while ((dir = readdir(d2)) != NULL)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            printf("  "); //two spaces on the left
            for (i=0; i < base; i++)
            {
                if (i%2 == 0 || i == 0)
                    printf("%c", 179);
                else
                    printf(" ");
            }

            printf("%c%c%s\n", 195, 196, dir->d_name);

            strcpy(directory, path);
            strcat(directory, "\\");
            strcat(directory, dir->d_name);
            tree(directory, base + 2);
        }
    }

    closedir(d2);
}

void clearScreen(HANDLE hConsole) {

    /*
     * Credits to microsoft and the original creator.
     * https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
     *
     * It clears the screen from any previous text or commands
     * and places the cursor at the start.
     */

    COORD coordScreen = {0, 0};    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.

    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR) ' ',     // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten))// Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    // Set the buffer's attributes accordingly.

    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.

    SetConsoleCursorPosition(hConsole, coordScreen);
}