# MUSICAL MAYHEM


## INPUT

<ol>
	<li> Number of performers </li>
	<li> Number of acoustic,electric stages</li>
	<li> Number of coordinates</li>
	<li> Minimum, maximum performance time</li>
	<li> Wait time for each performer </li>
	<li> Name, instrument type, the arrival time for each performer</li>
</ol>


## Explanation

<ul>

<li>
getsastage is the primary function. The instrument decides the type of performer. Type is 0 if the performer performs only on the acoustic stage or 1 if the performer performs only onthe electric stage else it is 2.
</li>

<li>
 Based on the type of performer corresponding timer threads (acoustic/electric) are created. Also, for a singer, timer thread is created which searches for a musician performing a solo (waits for formusician semaphore) andposts forsinger semaphores as a signal. forsinger semaphore is taken back if a musician is not available.
</li>
<li> 
Performers leave if they do not get corresponding stage or musician (for a singer) in time. 
</li>
<li> 
Initially, stage of a performer is –1. When timer threads are created, the thread which first received the semaphore sets the value of stage to 0 for acoustic, 1 for electric and 2 for musician. 
</li>
<li>
The timer threads for a stage also invokes performance function for the performer after getting a stage.
</li>
<li>
If a performer is performing, he/she creates a timer thread to search for a singer who is not performing. The performing musician sends a signal by posting formusician semaphore and waits for forsinger semaphore. If found, a singer performance time is increased by 2seconds else formusician semaphore is taken back.
</li>
<li>
After completion of performance, a thread is created to collect t-shirt from coordinators (for both singers and musicians who performed). 
</li>
</ul>