# thread-safe-counter
linux(Ubuntu20.24LTS)
***
### #Mutex Performance 
<img width="764" alt="mutex performance" src="https://user-images.githubusercontent.com/76603802/120200472-55142f80-c25f-11eb-8c5a-46fbc455c588.png">

### #Semaphore Performance
<img width="753" alt="semaphore performance" src="https://user-images.githubusercontent.com/76603802/120200754-a3293300-c25f-11eb-9380-0125c6a13648.png">

### #Result
Mutex performance is much better than Semaphore performance.

### #Analysis
Mutex can only be unlocked by someone who has locked it and no multiple enters. Mutex is has only two state lock or unlocked. So, it is very simple. 
Semaphore is no lock owner concept and multiple threads can enter critical section. Semaphore can has multiple variables that acting as a flag. Multiple lock variables increased code complexity. So, Semaphore performance is lower than mutex.
