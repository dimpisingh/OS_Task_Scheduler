# OS_Task_Scheduler
Implemented a custom system call that effectively isolates a CPU core. Further implemented the Rotating Staircase Deadline Scheduler (RSDL) algorithm to schedule processes exclusively on the isolated core using C, Kernel Programming

<h2>Introduction </h2>
<p>The RSDL Test Program is a C program designed to demonstrate and test CPU isolation, CPU affinity, and process priority management on a Linux system. It uses various system calls and libraries to achieve its functionality. This README provides an overview of the program and instructions on how to build and run it.</p>

<h2>Usage</h2>
<p>To use the program, follow these steps:</p>
<ol>
  <li>Build the program using the provided Makefile or compile it manually.</li>
  <li>Run the program with the desired command-line arguments.</li>
</ol>


    ./rsdl_test <num_procs> <num_iterations>


<ul>
  <li> '&lt;num_procs &gt;' : The number of child processes to spawn.</li>
  <li> '&lt;num_iterations&gt;' : The number of iterations for each child process to perform computations.</li>
</ul>
<h2>Building the Program</h2>
<p>To build the program, you can use the provided Makefile. Simply run the following command in the terminal:</p>

    make

<p>This will compile the program and generate an executable named 'rsdl_test' .</p>

<h2>Program Overview</h2>
<p>The program performs the following tasks:</p>
<ul>
  <li>CPU isolation using a custom system call (SYS_ISOLATE_CPU).</li>
  <li>CPU affinity setup.</li>
  <li>Process priority adjustment.</li>
  <li>Spawning multiple child processes to perform computations.</li>
</ul>
<h2>System Calls</h2>
<p>The program uses a custom system call called SYS_ISOLATE_CPU to isolate CPU 0. This system call is implemented within the program.</p>

<h2>CPU Isolation and Affinity</h2>
<p>The program isolates CPU 0 and assigns the main process to it. It also sets up CPU affinity for processes to ensure they run on specific CPUs. By default, CPU 0 is isolated, and the main process is assigned to it.</p>
<h2>Process Priority</h2>
<p>The program adjusts the priority of the main process to give it higher priority than other processes. It also randomly assigns priorities to child processes within a specified range.</p>
<h2>Running the Program</h2>
<p>To run the program, use the following command:</p>

    ./rsdl_test <num_procs> <num_iterations>

<p>Replace <num_procs> with the desired number of child processes to spawn and <num_iterations> with the number of iterations for computations.</p>
