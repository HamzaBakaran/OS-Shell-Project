# Project 2: Memory

### How the mmap and munmap system calls work?

Since we already explained **mmap** in previous project we will briefly explain how **mmap** and **munmap** work.

-The **mmap** system call is used to map files or devices into memory. It can also be used to allocate memory from the operating system (OS) for anonymous mappings.   
-The **munmap** system call is used to unmap memory that was previously mapped with mmap.  

When using **mmap** to allocate memory, we need to specify the following arguments:  

-**addr**: The desired starting address for the mapping. Pass NULL to let the OS choose the address.  
-**length**: The length of the mapping.  
-**prot**: The protection flags, determining the memory's access permissions (e.g., read, write, execute).  
-**flags**: Additional flags to control the mapping behavior (e.g., anonymous, shared, private).  
-**fd**: The file descriptor of the file to be mapped (if mapping a file).  
-**offset**: The offset within the file to start the mapping (if mapping a file).  

### How to use mmap to obtain pages of memory from the OS, and allocate chunks from these pages dynamically when requested?

The **mmap** system call is commonly used for memory mapping, which can be used to allocate memory from the OS for various purposes. Here's the general process:  

1. Include the necessary header files:  

```bash
#include <sys/mman.h>
#include <fcntl.h> // Only if you're mapping a file
```
2. Determine the size of the memory you want to allocate. Typically, you would use the _**getpagesize()**_ function to get the system's page size:  

```bash
size_t page_size = getpagesize();
```

3. Call mmap to request a memory mapping:  

```bash
void* mapping = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
```

**NULL**: The desired starting address for the mapping. Passing NULL allows the OS to choose the address.    
**page_size**: The size of the memory to allocae, typically in multiples of the page size.    
**PROT_READ | PROT_WRITE**: The desired access permissions for the memory region. In this example, we enable both read and write access.   
**MAP_PRIVATE | MAP_ANONYMOUS**: Creating an anonymous mapping in private mode. Anonymous mappings are not backed by any file on disk, and private mode ensures that changes to the mapping are not visible to other processes.    
**-1 and 0**: Not applicable in this case.

4.Check the return value of **mmap** to handle any errors. If **mmap** fails, it returns **MAP_FAILED**:  

```bash
if (mapping == MAP_FAILED) {
    perror("mmap");
    // Handle the error
    return 1;
}
```
5.Use the allocated memory as needed. You can treat the **mapping** pointer as a regular memory buffer and perform read and write operations on it.    

6.When you're done using the allocated memory, unmap it using **munmap**:
```bash
munmap(mapping, page_size);
```

It's important to note that mmap and munmap work with memory pages, so the allocated memory size should typically be a multiple of the system's page size.

This approach allows you to obtain pages of memory from the OS using mmap and allocate chunks from these pages dynamically when needed. Remember to release the allocated memory using munmap when you're finished with it to free up system resources.

There are additional options and techniques you can use with mmap, such as mapping files, sharing memory between processes, and specifying additional flags and parameters based on your specific requirements.

### Here are the various arguments to the mmap system call and their descriptions:  

**addr (void *)**:  

The desired starting address for the mapping.  
Pass **NULL** to let the operating system choose the address automatically.  
If a specific address is specified, the operating system may or may not honor it.  
  
**length (size_t)**:  

The length of the mapping in bytes.  
Usually, this value is a multiple of the system's page size to ensure aligned mappings.  
    
**prot (int)**:    

The protection flags for the mapping, determining the memory's access permissions.  
The possible flags include:    
**PROT_NONE**: No access is allowed.  
**PROT_READ**: Read access is allowed.  
**PROT_WRITE**: Write access is allowed.  
**PROT_EXEC**: Execution access is allowed.  

**flags (int)**:

Additional flags to control the behavior of the mapping.    
Common flags include:    
**MAP_SHARED**: Creates a shared mapping, allowing multiple processes to share the same memory region.  
**MAP_PRIVATE**: Creates a private mapping, ensuring changes to the mapping are not visible to other processes.  
**MAP_FIXED**: Forces the mapping to use the exact address specified in **addr**  
**MAP_ANONYMOUS**: Creates an anonymous mapping that is not backed by any file.  
**MAP_FILE**: Maps a file specified by the file descriptor **fd** instead of using anonymous memory.  
  
**fd (int)**:  

The file descriptor of the file to be mapped (if mapping a file).      
This argument is used when **MAP_FILE** is set in the **flags** parameter.     
The file must be opened with appropriate permissions before calling **mmap.**  
  
**offset (off_t)**:  

The offset within the file to start the mapping (if mapping a file).  
This argument is used when **MAP_FILE** is set in the **flags** parameter.  
It specifies the offset from the beginning of the file where the mapping should start.  
  
The mmap system call returns the starting address of the allocated memory region upon success or **MAP_FAILED** if the mapping fails. In case of an error, you can use the **perror** function to print an error message.      

###  Write a simple C program that runs for a long duration say, by pausing for user input or by sleeping. While the process is active, use the ps or any other similar command with suitable options, to measure the memory usage of the process. Specifically, measure the virtual memory size (VSZ) of the process, and the resident set size (RSS) of the process (which includes only the physical RAM pages allocated to the process).  

```bash
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Process ID: %d\n", getpid());
    printf("Pausing for user input. Press Enter to continue...\n");
    getchar(); // Wait for user input
    sleep(3600); // Sleep for 1 hour

    return 0;
}
```
By running this program we obtained following:

![image](https://github.com/HamzaBakaran/OS-Shell-Project/assets/101096551/a11773fe-c11f-485a-8413-d9fc800b5004)  

**PID**:Process ID. It uniquely identifies a running process on the system.  
**VSZ**:VSZ (Virtual Memory Size): It represents the total virtual memory used by the process. Virtual memory includes both physical RAM and swap space (disk space used as an extension of RAM). The value is reported in kilobytes (KB). In this case, the process with PID 10219 has a virtual memory size of 2772 KB.   
**RSS**:RSS (Resident Set Size): It represents the physical RAM pages allocated to the process. It indicates the actual memory used by the process that is present in physical RAM. The value is reported in kilobytes (KB). In this case, the process with PID 10219 has a resident set size of 944 KB.

### Add code to your simple program to memory map an empty page from the OS  
```bash
  #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    printf("Process ID: %d\n", getpid());
    printf("Pausing for user input. Press Enter to continue...\n");
    getchar(); // Wait for user input

    void* mapping = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Memory mapping created.\n");
    printf("Pausing again. Press Enter to exit...\n");
    getchar(); // Wait for user input

    munmap(mapping, getpagesize());

    return 0;
}
```
This code adds memory mapping using mmap with the following arguments:

**NULL**: Let the OS choose the starting address for the mapping.
**getpagesize()**: Allocate one page of memory.
**PROT_READ | PROT_WRITE**: Enable read and write access to the memory.
**MAP_PRIVATE | MAP_ANONYMOUS:** Create an anonymous mapping in private mode.
**-1 and 0**: Not applicable in this case.


![image](https://github.com/HamzaBakaran/OS-Shell-Project/assets/101096551/73b180f9-3bc4-483e-83ac-4dbc2762a615)
  
**PID**: Process ID. It uniquely identifies a running process on the system. In this case, the process ID is 10696.  

**VSZ** (Virtual Memory Size): It represents the total virtual memory used by the process. Virtual memory includes both physical RAM and swap space. The value is reported in kilobytes (KB). Here, the process has a virtual memory size of 2776 KB.  

**RSS** (Resident Set Size): It represents the physical RAM pages allocated to the process. The RSS indicates the actual memory used by the process that is present in physical RAM. The value is reported in kilobytes (KB). In this case, the process has a resident set size of 944 KB.  

After creating the memory mapping in our program and pausing before any data is written into it, the **ps** command reports an increase in virtual memory usage (**VSZ**). This is because the virtual memory space has been reserved for the memory mapping, adding to the overall virtual memory footprint of the process. The size reported in **VSZ** will depend on the page size used by the system and the size specified in **mmap**.  

However, the physical memory usage (**RSS**) does not change at this point. This is because the memory mapping created by **mmap** is initially empty and not backed by any physical memory pages. The operating system only allocates physical memory pages on demand when the mapped memory is accessed or modified.  

So, when we pause the program after creating the memory mapping but before any data is written into it, the **RSS** remains the same as before because no physical memory pages have been allocated for the mapping yet.

In summary, the increase in virtual memory usage (**VSZ**) indicates the reserved virtual address space for the memory mapping, while the unchanged physical memory usage (**RSS**) reflects the fact that no physical memory pages have been allocated for the mapping until it is accessed or modified.


###  Write some data into your memory mapped page and measure the virtual and physicalmemory usage again

```bash
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    printf("Process ID: %d\n", getpid());
    printf("Pausing for user input. Press Enter to continue...\n");
    getchar(); // Wait for user input

    void* mapping = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapping == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Memory mapping created.\n");
    printf("Writing data into the memory mapped page...\n");

    const char* message = "Hello, Memory!";
    size_t message_len = strlen(message) + 1;
    memcpy(mapping, message, message_len);

    printf("Data written into the memory mapped page.\n");
    printf("Pausing again. Press Enter to exit...\n");
    getchar(); // Wait for user input

    munmap(mapping, getpagesize());

    return 0;
}
```

  


  
This code writes a string message into the memory mapped page using **memcpy**. The message is then printed to the console.    
![image](https://github.com/HamzaBakaran/OS-Shell-Project/assets/101096551/16eb71f2-f288-4e36-bc64-eb3ffcfb33fd)  
There is no significant change in either virtual memory usage (**VSZ**) or physical memory usage (**RSS**).  

Explanation:  

Initially, before memory mapping, the program consumes a certain amount of virtual and physical memory based on its code, stack, and heap usage.  
When memory mapping is created, virtual memory usage (**VSZ**) increases because the mapping reserves a portion of the virtual address space for the process.
However, the physical memory usage (**RSS**) does not change because the memory mapping does not immediately allocate physical memory pages.
When data is written into the memory mapped page, the virtual memory usage remains the same, as the process already reserved that space. The physical memory usage also remains the same because the OS uses a technique called demand paging, which brings the required pages into physical memory only when they are accessed.





