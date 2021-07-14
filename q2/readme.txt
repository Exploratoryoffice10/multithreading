					Q2 REPORT:-

Each of pharmaceutical company , vaccination zone and student functions are in a thread. 
each follows a protocol to ensure the vaccination of students in the vaccination zone.

first jls is the number of remaining student thread that executing i.e., its the number of
students whose result regarding their stay in campus or home is not decided yet. when all
students are done. jls is zero. we are waiting for all students thread to complete their
execution(by using pthread_join we wait for all student threads to complete).the thread is 
done when the judjement regarding his stay is finalised.

struct company:-
for pharmaceutical company there is a struct holding its important information. it contains the 
number of batches left and the vaccines per batch.that is struct company renamed as pcom. the 
name of the pointer array holding this information is companydata[]. it also has a mutex lock 
to safegaurd it from parellel modification of same data.

struct student:-
for student there is a struct holding its required information.it contains the index of zone 
in which the student is going to get vaccinated in a variable named vacz , a variable named 
passed which will be 0 when waiting 1 when slot is alloted 2 when the student has to go home 
and 3 when student can stay in campus and a variable telling the number of the test it is 
going to participate in when it exceeds 3 the student thread is done as we can say the final 
report of student. when  no zone is not alloted to student to student or when there is no need
for student to go to a vaccination zone it is initialised to -1. it also has a mutex lock 

struct vaccinezone:-
for vaccination zone there is a struct holding its required information.it contains the index
of the company from which it got its medicines from in a variable named providernum, the number 
of vaccines left in vacleft and no.of slots given in noofslot and it contains a variable named 
status which tells whether is waiting for students to come or it entered its vaccination and a 
mutex lock.

all the arrays are filled with valid data at the beginning in the main function before initialising
the threads. as there wont be any batches in company then it is set to zero. the vaccineperbatch is 
also set to zero. for vaccination zone no of slot and vaccines left is set to zero and vaccine 
zone variable is set to -1 as there is no index -1.so there wont be a company because no such
company exists. it must get one from one.all locks are initialised while initialising variables.
for student its zone index is set to -1 . and passed is set to zero because it should wait for 
allocation. testno is set to 1 because he is going to have his first test.testno is only for student 
to know the number of test he is going to get.

NOTE:- IF ANY OF N,M,O IS ZERO THEN THERE IS NO POINT IN CONTINUING THE SIMULATION THEN THE 
SIMULATION IS STOPPED
it is useless if there are no students to come up with vaccination.
it is impossible to vaccinate if there are no providers (companies that produce vaccine)
it is impossible to vaccinate if there are no zones because student goes to zones not to the company.

HOW THEY WORK TOGETHER:

for pharmaceutical company there is a while loop with condition jls>0 as initially jls is o(number of students)
then we produce vaccines.we wait till all the vaccines are exhausted by checking the 
providernum of each zone because when vaccines left is zero it is asking for new 
vaccines so providernum is set to -1 from the previous provider index to -1. as it 
will be checking every zones provider num when it is -1 that implies there is no
vaccines left which are prepared in this unit.

for vaccination zone it has to do two jobs one is getting the vaccines from company and 
then exhausting the vaccines by assigning slots and conducting vaccinations. it searches 
for batches in zone and puts a lock and gets vaccines and decreases the number of batches 
in stock of company by one. now it assigns slots and the student will select one of the 
slot.this contains while for 2 jobs and a while loop for second job which breaks when 
zone runs out of vaccines.

for student it checks all vaccination zone for slot when it finds it has one then it 
decreases the number of slot by one and no.of vaccines by one. now student has to wait 
till all the slots are alloted or till no slots can be alloted (because all of them 
has passed 1 or 2 or 3). Suppose two different zones are waiting for slots to fill 
and there are no student then we have to move on because there wont be any slots. 
there is no point in waiting because we have to wait eternally so we move. and finally 
lets say some zones are waiting for no of waiting process > 0 then we can allot slots. 
but there are no waiting process but jls will be zero so it will not wait for completion 
of other threads.

when vaccination zone decides to start vaccination zone then we set status 1 and student 
will be (busy waiting) waiting for this condition. when this is condition is satisfied 
we wait for 2 seconds and then continue by giving the results of the test.

once vaccination phase starts in zone noof slots is made zero so no other thread can pick 
this vaccination zone or it leads to cases when student has his slot under a zone before
even the zone allots slots. by using mutex lock we make sure that either slot is picked 
first then it is made to zero or it enters vaccination zone he has to go to other zone 
for vaccination as there are no slots in the zone(because starting vaccination for the 
slots alloted)we have to take care of this because while one zone is waiting other zone 
may have finished its vaccination phase which may lead to decrement in no of waiting 
process which is dynamic and it changes continuously.

at the start of each thread of company and vaccination zone printing "that entity is ready" 
to state that the company and vaccination zone are ready to do its part and i am printing 
the final position of student whether he has a online semester or offline semester.

when the students select slot anyone can get anyone because threads are very random and by the
it is searching for a slot anyone may be alloted because the order is very unpredictable.

INPUT:
when either of n,m,o is zero then simulation is stopped. if valid we continue by taking the 
the probablity.

then first all company threads are called,followed by vaccination zone and students. there is 
no chance of deadlock because no two process are not locking two same mutex in which we wait 
for other to unlock it (this results in deadlock).

i am passing the index of the thread so that it can locate its struct data by referencing using
the index. the struct array we created is a global variable so it doesnt matter even if you pass
it by using its address.(if we pass by address we have to store additional index in it so it can
locate itself). by using the index the thread changes the values of struct which is used to 
achieve the simulation..
