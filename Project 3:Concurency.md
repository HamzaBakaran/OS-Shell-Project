## Project 3: Concurrency  
  
Task: Server-Client  
  
Here we are considering an application server which needs to handle requests of many clients.
The server can serve only one request (thread) at a time. The other threads (requests) that arrive
while the server is busy, must wait to be served using a synchronization primitive (semaphore or
condition variable).To make sure the waiting times are not too excessive, the server keeps a maximum of N
requests (threads) in the system, and you may suppose N > 2. The thread that arrives in the
system will have to first check if N other requests are already in the system: if yes, the thread
will exit without waiting and return an error value to the client, by calling the function
thread_exitFailure(). This function will terminate the thread and it does not return.
When a thread is ready to be served, it must call the function receiveService(). No more
than one thread should call this function at any point in time. This function blocks the thread
during the service time.
Note that, while the thread which is receiving the service is blocked, other arriving threads must
be free to join the queue, or exit if the system is overloaded.
After a thread returns from receiveService(), it must enable one of the waiting threads to
seek service (if any are waiting), and then terminate itself successfully by calling the function
thread_exitSuccess(). This function terminates the thread and does not return.
Your task is to write pseudocode of the function to be run by the request threads in this system.
For a solution, you can only use locks and condition variables, but you may use other variables if
your solution asks for it (clearly state all the variables used and their initial values at the start of
your solution).  

Solution:  


```
integer num_requests = 0  // Number of requests currently in the system
boolean server_busy = false  // Flag to indicate if the server is busy
condition variable cv  // Condition variable for synchronization
lock mutex  // Lock for mutual exclusion

lock(mutex)  // Acquire the lock to enter the critical section

if num_requests == N:  // Check if the maximum number of requests is reached
    unlock(mutex)  // Release the lock before exiting
    thread_exitFailure()  // Exit the thread with a failure

num_requests++  // Increment the number of requests in the system

if server_busy:  // Check if the server is busy
    wait(cv, mutex)  // Wait on the condition variable until server is available

server_busy = true  // Server is now busy serving a request
unlock(mutex)  // Release the lock

receiveService()  // Simulate the service time for the request

lock(mutex)  // Reacquire the lock before entering the critical section

num_requests--  // Decrement the number of requests as the request is served
server_busy = false  // Server is no longer busy

if num_requests > 0:  // Check if there are waiting requests
    signal(cv)  // Signal one waiting request to proceed

unlock(mutex)  // Release the lock

thread_exitSuccess()  // Terminate the thread successfully
```
