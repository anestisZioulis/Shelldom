<a name="readme-top"></a>

<pre align="center">
****************************************************
**     __        ___            __   __   v1.0    **
**    /__` |__| |__  |    |    |  \ /  \  |\/|    **
**    .__/ |  | |___ |___ |___ |__/ \__/  |  |    **
**                                                **
**                by Anestis Zioulis              **
**                 www.azioulis.com               **
****************************************************
</pre>

<!-- TABLE OF CONTENTS -->
<h2>
  <details align="center">
    <summary>Menu</summary>
    <a href="#about">About</a><br>
    <a href="#demo">Demo</a><br> 
    <a href="#installation">Installation</a><br> 
    <a href="#main-commands">Main Commands</a><br> 
    <a href="#extra-commands">Extra Commands</a><br> 
    <a href="#tools">Tools</a><br>
    <a href="#sources">Sources</a><br>
  </details>
</h2>

<!-- ABOUT -->
# <pre align="center">About</pre>
The `Shelldom` CLI (_Command Line Interface_) is a program for **Windows**, written in **C**.
It tries to be an **MS-DOS** like operating system, that supports specific commands and options.

This program was created as an assignment for the lesson "Operating Systems I" at the **IHU**
(_International Hellenic University - Serres Campus_) by Anestis Zioulis.

> **My assignment**:<br>
Develop a **shell** in the programming language of your choice. If the `C` language is used,
avoid using the `system()` command and create your own `functions`.<br>Your CLI must support the following commands:
`dir`, `move`, `echo`, `type`, `sort`. <br><br>

> **Additional commands**:<br>
I did implement some additional commands, that were assigned to other students in order
for the CLI to feel more usable and complete, than just an assignment.<br>
I implemented these extra commands: `list`, `help`, `cd`, `create`, `read`, `del`, `tree`, `cls`.

> **Notice**:<br>
This program is `NOT` case-sensitive.

<br>

<!-- DEMO -->
# <pre align="center">Demo</pre>

<div align="center">
  
  | ![](/Files/Images/demo.gif) |
  :--:
  
</div>

<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- INSTALLATION -->
# <pre align="center">Installation</pre>
To use this program you can download the latest <a href="https://github.com/anestisZioulis/Shelldom/releases/">release</a> and use the executable inside.
Also, you can compile the source code manually or through an IDE with a bundled compiler:

<br>

<ul>
  <li>[1] Download the latest <a href="https://github.com/anestisZioulis/Shelldom/releases/">release</a> and run the executable.</li>
  <h4 align="center">or</h4>
  <li>[2] Download JetBrains CLion or another IDE that includes the compiler.</li>
  <h4 align="center">or</h4>
  <li>[3] Download a tool chain, like MinGW, through the web and install it.</li>
</ul>


<br>

- [3] If you opted to compile the source code yourself.
- You downloaded the tool chain that has the compiler and installed it.
- Open `cmd`, change directory to the folder the source file is and check if `gcc`
  is available with the following command:
  ```c
    gcc -v
  ```

<br>

- If you see something like this you are good to compile:
  ```c
    Using built-in specs.
    COLLECT_GCC=gcc
    COLLECT_LTO_WRAPPER=c:/mingw/bin/../libexec/gcc/mingw32/9.2.0/lto-wrapper.exe
    Target: mingw32
    Configured with: ../src/gcc-9.2.0/configure --build=x86_64-pc-linux-gnu --host=mingw32 --target=mingw32 --disable-win32-registry --with-arch=i586 --with-tune=generic --enable-static --enable-shared --enable-threads --enable-languages=c,c++,objc,obj-c++,fortran,ada --with-dwarf2 --disable-sjlj-exceptions --enable-version-specific-runtime-libs --enable-libgomp --disable-libvtv --with-libiconv-prefix=/mingw --with-libintl-prefix=/mingw --enable-libstdcxx-debug --disable-build-format-warnings --prefix=/mingw --with-gmp=/mingw --with-mpfr=/mingw --with-mpc=/mingw --with-isl=/mingw --enable-nls --with-pkgversion='MinGW.org GCC Build-20200227-1'
    Thread model: win32
    gcc version 9.2.0 (MinGW.org GCC Build-20200227-1)
  ```

<br>

- Type:
  ```c
    gcc -o shelldom.exe main.c
  ```
  and your executable should have been compiled.

<br>

- You can run it in `cmd` by just typing it:
  ```c
    shelldom.exe
  ```

<br>

>Notice:<br>
This program have `libraries` that work in `Windows OS` only!
<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MAIN COMMANDS -->
# <pre align="center">Main Commands</pre>
<p align="center">These commands were required by the lesson assignment.</p>
<hr>
<br>

- [x] **DIR** / **LS**
    - Display the contents of a directory in a column(default) or a list(`-l`).
        - `dir` `<option>` `<path>`
        - `ls` `<option>` `<path>`
      ```c
          >> dir
          >> ls
          >> dir -l c:
          >> ls c:/deep/
          >> dir ../
          >> ls -l ../linux/impostor
      ```

<hr>

- [x] **MOVE**
    - Move a file from one folder to another
        - `move` `<path/file>` `<path>`
      ```c
          >> move c:/mario.txt d:/adventures/
          >> move princess.jpg f:/img/
          >> move ../monition.pre E:/nd/is/near/
      ```
<hr>

- [x] **ECHO**
    - Append a typed string in the provided file
        - `echo` `<alphanumeric_value>` `>` `<file>`
      ```c
          >> echo john55 > about.txt
          >> echo AbigSTRING_withText99 > sandiego.txt
      ```
<hr>

- [x] **TYPE**
    - Merge all the text files of the current folder into a new one.
    - If it exists, ask for permission to replace, append or cancel.
        - `type` `/a` `*.*` `>` `<file>`
      ```c
          >> type /a *.* > about.txt
          >> type /a *.* > YotaFive.txt
      ```
<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- EXTRA COMMANDS -->
# <pre align="center">Extra Commands</pre>
<p align="center">These extra commands were added by me, not included in the assignment,
so the CLI is more usable.</p>
<hr>
<br>

- [x] **SORT**
    - Read 32 numbers (byte size) from a binary file, sort them and display them.
        - `sort` `<file>`
      ```c
          >> sort numbers.bin
      ```
<hr>

- [x] **LIST**
    - Show all the available commands.
        - `list`
      ```c
          >> list
      ```
<hr>

- [x] **HELP**
    - Show details about a specific command.
        - `help` `<command>`
      ```c
          >> help sort
          >> help dir
      ```
<hr>

- [x] **CD**
    - Change the current working directory.
        - `cd` `<path>`
      ```c
          >> cd
          >> cd f:/
          >> cd ..
          >> cd ../backToTheFuture/
      ```
<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>
<hr>

- [x] **CREATE**
    - Save user inserted integer numbers from 1-255 in a text or binary file, exit with 0.
        - If you want to create a binary file without `.bin` you have to use the option `-b`!
        - `create` `<option>` `<file>`
      ```c
          >> create binaryFile.bin
          >> create -b binaryFileAsWell
          >> create textFile.txt
          >> create textFileWithoutExtension
      ```
<hr>

- [x] **READ**
    - Reads text or binary files.
        - In order to read binary files without the `.bin` extension, use the option `-b`.
        - `read` `<file>`
      ```c
          >> read data.bin
          >> read -b alsoBinary
          >> read numbers.txt
          >> read readme.md
      ```
<hr>

- [x] **DEL**
    - Delete a file `<file>` or files like `<file*>` or all files `*.*` on the current folder.
        - `del` `<file>`
      ```c
          >> del data.bin
          >> del dat*
          >> del *.*
      ```
<hr>

- [x] **TREE**
    - Displays the structure of the current folder, sub-folders and files.
        - `tree` `<path>`
      ```c
          >> tree
          >> tree f:/
          >> tree ..
          >> tree ../backToTheFuture/
      ```
<hr>

- [x] **CLS**
    - Clear the screen from text and move the cursor at the top left.
        - `cls`
      ```c
          >> cls
      ```
      >A microsoft function was used for this in order to bypass the `system` command,
      due to the assignment limitation. Will not work in the CLion console window!
      More details for the function: https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
<hr>

- [x] **EXIT**
    - Quits the program.
        - `exit`
      ```c
          >> exit
      ```
<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- TOOLS -->
# <pre align="center">Tools</pre>
<p align="center">The tools I used to create this assignment.</p>
<hr>

- MingGW_GCC 9.2.0 compiler from [MinGW-w64](https://www.mingw-w64.org/)
- Jetbrains CLion (Student/full version)
- C Standard 99
<hr>

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- SOURCES -->
# <pre align="center">Sources</pre>
<p align="center">Sources I used material to create the project and readme.</p>
<hr>

* [Stack Overflow](https://stackoverflow.com/)
* [Text to ASCII generator](https://patorjk.com/software/taag/)
* [Lorem Ipsum generator](https://www.lipsum.com/)
* [Best-README-Template](https://github.com/othneildrew/Best-README-Template/)
* [Microsoft CLS command](https://docs.microsoft.com/en-us/windows/console/clearing-the-screen)

<hr>
<p align="right">(<a href="#readme-top">back to top</a>)</p>
