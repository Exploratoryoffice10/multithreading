#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#include <sys/wait.h>


#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


int n,m,o,jls;
//n is number of pharma companies, m is number of vaccination zones, o is no. of students // a is number of students who have completed the process


long double pro[1500];


// initialising threads


pthread_t companythrd[1500];  
pthread_t vaczonethrd[1500];   
pthread_t studentthrd[1500];  
pthread_mutex_t lfj; // lfj means lock for jls both operation jls-- at a time not accepted

typedef struct company
{
    int nofbat,vacperbat;    // can get prob by using pro[comnum]
    pthread_mutex_t mutex;
}pcom;

pcom * companydata[1500];

typedef struct vaccinezone
{
    int status; // 1 means in action 0 means waiting
    int providernum;
    int noofslot;   // slots 
    int vacleft;    
    pthread_mutex_t mutex;
}vzone;

vzone * vaczonedata[1500];

typedef struct student{
    int testno;     // tells the test number its is going to go 
    int passed;     // 0 waiting for slot alloacation 1 got slot 2 student sent home 3 welcomed
    int vacz;       // going to vaccine zone
    pthread_mutex_t mutex;
}stdt;

stdt * studentdata[1500];

int min(int x,int y){
    if(x<y){
        return x;
    }
    return y;
}

void *pcom_fun(void * inp)
{
    int ind = *((int*)inp);
    int timforbat,flag1,flag2,batnum,vanum;
    printf(ANSI_COLOR_RED"Pharmaceutical company %d is ready to function now\n"ANSI_COLOR_RESET,ind+1);
    while(jls>0)
    {
        timforbat=rand()%4+2;
        batnum=rand()%5+1;
        vanum=rand()%11+10;
        printf(ANSI_COLOR_BLUE"Pharmaceutical Company %d is preparing %d batches of vaccines with %d in each which have success probability %Lf.\n"ANSI_COLOR_RESET,ind+1,companydata[ind]->nofbat,companydata[ind]->vacperbat,pro[ind]);
        sleep(timforbat);   // after this time the value should be updated so we can say it took this time to prepare medicine
        pthread_mutex_lock(&(companydata[ind]->mutex));
        companydata[ind]->nof
        bat=batnum;
        companydata[ind]->vacperbat=vanum;
        pthread_mutex_unlock(&(companydata[ind]->mutex));
        printf(ANSI_COLOR_BLUE"Pharmaceutical Company %d has prepared %d batches of vaccines with %d in each which have success probability %Lf.\n"ANSI_COLOR_RESET,ind+1,companydata[ind]->nofbat,companydata[ind]->vacperbat,pro[ind]);
        // implement a loop
        while(1)
        {
            if((companydata[ind]->nofbat)>0)
            {
                continue;
            }
            flag1=1;            // flag1 is to check when he has to start a production
            for(int i=0;i<m;i++)
            {
                if((vaczonedata[i]->providernum)==ind)      // when there is a vaccines of this provider
                {                                           // if i is not there in 
                    flag1=0;
                    break;
                }
            }
            if(flag1==1)
            {
                break;
            }
        }
        sleep(2);   // to start procuction again            
        printf(ANSI_COLOR_BLUE"All the vaccines prepared by Pharmaceutical Company %d are emptied. Resuming production now.\n"ANSI_COLOR_RESET,ind+1);
    }   // main loop 
    return NULL;
}


void *vacz_fun(void * inp)
{
    int y1,y2,y3;
    int ind = *((int*)inp);
    int x1,x2,x3,x4;
    printf(ANSI_COLOR_RED"Vaccination zone %d is ready to function now\n"ANSI_COLOR_RESET,ind+1);
    while(jls>0)
    {
        printf(ANSI_COLOR_MAGENTA"Vaccination zone %d is now searching for a vaccine provider\n"ANSI_COLOR_RESET,ind+1);
        vaczonedata[ind]->status=0;
        x1=1;   // x1 is a flag
        while(x1>0)
        {
            for(int i=0;i<n;i++)
            {
                pthread_mutex_lock(&(companydata[i]->mutex));
                if((companydata[i]->nofbat)>0)
                {
                    pthread_mutex_lock(&(vaczonedata[ind]->mutex));
                    (companydata[i]->nofbat)-=1;  
                    vaczonedata[ind]->providernum=i;
                    vaczonedata[ind]->vacleft=companydata[i]->vacperbat;    
                    pthread_mutex_unlock(&(vaczonedata[ind]->mutex));
                    pthread_mutex_unlock(&(companydata[i]->mutex)); // missed when used to break so unlocking
                    x1=0;
                    break;
                }  
                pthread_mutex_unlock(&(companydata[i]->mutex));
            }   //after break execution starts from here
        }
        printf(ANSI_COLOR_BLUE"Pharmaceutical Company %d has delivered vaccines to Vaccination zone %d, resuming vaccinations now\n"ANSI_COLOR_RESET,1+vaczonedata[ind]->providernum,ind+1);
        
        /*make the vaccines in it empty so prduction will start again loop for emptying below*/
        // then start // wait till k got finished or cannot give more//  make no.of vaccines to zero
        x1=0;   // flag for waiting till vaccination zone start
        while(1)
        {
            vaczonedata[ind]->status=0;
            x2=0;   // the number of students waiting
            for(int i=0;i<o;i++)
            {
                if((studentdata[i]->passed)==0) 
                    x2++;                       // x2 is no of waiting process;
            }  
            if(x2==0)   // if waiting process are there then x2 can go from 0 to higher values then he will get slots
            { 
                if(jls==0){
                    return NULL;
                }  
                continue;
            }
            if(x2!=0)   
            {
                x3=min(8,min(x2,vaczonedata[ind]->vacleft));
                x4=1+rand()%x3;	
            }
            pthread_mutex_lock(&(vaczonedata[ind]->mutex));
            vaczonedata[ind]->noofslot=x4; 
            pthread_mutex_unlock(&(vaczonedata[ind]->mutex));
            printf(ANSI_COLOR_RED"Vaccination Zone %d is ready to vaccinate with %d slots...//...\n​"ANSI_COLOR_RESET,ind+1,x4);   
            while(1)
            {
                x2=0;y3=0;
                for(int i=0;i<o;i++)
                {
                    //pthread_mutex_lock(&(studentdata[i]->mutex));
                    pthread_mutex_lock(&(studentdata[i]->mutex));
                    if((studentdata[i]->vacz)==ind)     // if i choses this as vacccination zone then increase x2 
                    {
                        x2++;
                    }
                    if(studentdata[i]->passed==0){
                        y3++;
                    }
                    pthread_mutex_unlock(&(studentdata[i]->mutex));
                }  
                if(x2==x4)  // when ind is not provider of any vaczone means all supplied by it are finished
                {
                    pthread_mutex_lock(&(vaczonedata[ind]->mutex));
                    vaczonedata[ind]->status=1;
                    vaczonedata[ind]->noofslot=0;
                    pthread_mutex_unlock(&(vaczonedata[ind]->mutex));
                    break;        
                }
                else if (x2<x4)
                {   
                    if(x2==0)   //cant move further if no slots are alloted
                    {
                        continue;
                    }
                    if(y3==0)
                    {
                        pthread_mutex_lock(&(vaczonedata[ind]->mutex));
                        vaczonedata[ind]->status=1;
                        vaczonedata[ind]->noofslot=0;   // no one should pick rest of the slots if 
                        pthread_mutex_unlock(&(vaczonedata[ind]->mutex));
                        break;
                    }
                }
            }

            printf(ANSI_COLOR_MAGENTA"VACCINATION ZONE %d entering Vaccination Phase\n"ANSI_COLOR_RESET,ind+1);
            y1=0;
            while(y1==0)
            {
                y1=1;
                for(int i=0;i<o;i++)
                {
                    if((studentdata[i]->vacz)==ind)
                    {
                        y1=0;           
                    }
                }
            }
            
            printf(ANSI_COLOR_MAGENTA"vaccination zone %d completed vaccination for slots alloted. Vaccines left are %d\n"ANSI_COLOR_RESET,ind+1,vaczonedata[ind]->vacleft);
            if((vaczonedata[ind]->vacleft)==0)
            {   
                pthread_mutex_lock(&(vaczonedata[ind]->mutex));
                vaczonedata[ind]->providernum=-1;
                pthread_mutex_unlock(&(vaczonedata[ind]->mutex));
                break;
            }
        }
        printf(ANSI_COLOR_CYAN"Vaccination Zone %d has run out of vaccines\n"ANSI_COLOR_RESET,ind+1);
    }
    return NULL;
}


void *stud_fun(void* inp){
    int ind = *((int *)inp); 
    studentdata[ind]->testno = 1;
    studentdata[ind]->passed = 0;
    int ccc=1,flag1=0;int fss=0;
    long double pab;
    while(ccc)
    {   
        studentdata[ind]->passed=0;
        if(studentdata[ind]->testno==1)
        {
            printf(ANSI_COLOR_CYAN"Student %d has arrived for his 1st round of Vaccination\n"ANSI_COLOR_RESET,ind+1);
        }
        else if(studentdata[ind]->testno==2)
        {
            printf(ANSI_COLOR_CYAN"Student %d has arrived for his 2nd round of Vaccination\n"ANSI_COLOR_RESET,ind+1);
        }
        else if(studentdata[ind]->testno==3)
        {
            printf(ANSI_COLOR_CYAN"Student %d has arrived for his 3rd round of Vaccination\n"ANSI_COLOR_RESET,ind+1);
        }
        printf(ANSI_COLOR_GREEN"Student %d is waiting for allocation of slot in a vaccination zone\n"ANSI_COLOR_RESET,ind+1);
        fss=1;  // flag for slot allocation
        while(fss)
        {
            studentdata[ind]->passed=0;
            for(int i=0;i<m;++i)
            {   
                pthread_mutex_lock(&(vaczonedata[i]->mutex));
                if((vaczonedata[i]->noofslot)>0)  // if slots are there
                {
                    (vaczonedata[i]->vacleft)-=1;
                    (vaczonedata[i]->noofslot)-=1;
                    studentdata[ind]->vacz=i;
                    studentdata[ind]->passed=1;     //found slot
                    printf(ANSI_COLOR_CYAN"Student %d assigned a slot on the Vaccination Zone %d and waiting to be vaccinated\n"ANSI_COLOR_RESET,ind+1,studentdata[ind]->vacz+1);
                    pthread_mutex_unlock(&(vaczonedata[i]->mutex));
                    fss=0;
                    break;
                }
                pthread_mutex_unlock(&(vaczonedata[i]->mutex));
            }    
        }
        // busy waiting
        while((vaczonedata[studentdata[ind]->vacz]->status)==0) // when in vaccination phase status is set to 1 
        sleep(2);
        pab = pro[vaczonedata[studentdata[ind]->vacz]->providernum];
        
        printf(ANSI_COLOR_GREEN"Student %d on Vaccination Zone %d has been vaccinated which has success probability %Lf.\n"ANSI_COLOR_RESET,ind+1,studentdata[ind]->vacz+1,pab);
        
        if(((long double)rand()/RAND_MAX)<=pab)
        {
            printf(ANSI_COLOR_YELLOW"Student %d has tested positive for antibodies.\n"ANSI_COLOR_RESET,ind+1);
            pthread_mutex_lock(&(studentdata[ind]->mutex));
            studentdata[ind]->passed=3; 
            studentdata[ind]->vacz=-1;
            pthread_mutex_unlock(&(studentdata[ind]->mutex));
        }
        else    // when failed 
        {
            printf(ANSI_COLOR_YELLOW"student %d has tested negative for antibodies.\n"ANSI_COLOR_RESET,ind+1);
            pthread_mutex_lock(&(studentdata[ind]->mutex));
            studentdata[ind]->testno+=1;    // he has to go another test
            studentdata[ind]->passed=0;     // he has to wait again
            studentdata[ind]->vacz=-1;      // no zone is alloted to him no he is out
            pthread_mutex_unlock(&(studentdata[ind]->mutex));
        }
        if((studentdata[ind]->passed)==3)   // tested possible so going to campus
        {       
            printf(ANSI_COLOR_MAGENTA"CONGRATULATIONS STUDENT %d....YOU ARE LUCKY ENOUGH TO CONTINUE THE SEMESTER IN COLLEGE\n"ANSI_COLOR_RESET,ind+1);   
            pthread_mutex_lock(&(lfj));
            jls--;  // student decision is given
            pthread_mutex_unlock(&(lfj));
            ccc=0;
            studentdata[ind]->vacz=-1;
        }
        if((studentdata[ind]->testno)>3)    // no more tests possible so going to home
        {
            printf(ANSI_COLOR_MAGENTA"UNLUCKY STUDENT %d. WE FAILED TO INJECT ANTIBODIES INTO YOUR BODY.\n"ANSI_COLOR_RESET,ind+1);
            printf(ANSI_COLOR_MAGENTA"SORRY TO SAY THIS STUDENT %d BUT PLEASE PACK YOUR LUGGAGE AND FIND YOUR WAY HOME.\n"ANSI_COLOR_RESET,ind+1);
            pthread_mutex_lock(&(studentdata[ind]->mutex));
            studentdata[ind]->passed=2; 
            pthread_mutex_unlock(&(studentdata[ind]->mutex));
            pthread_mutex_lock(&(lfj));
            jls--;
            pthread_mutex_unlock(&(lfj));
            ccc=0;
            studentdata[ind]->vacz=-1;
        }
    }

    if(studentdata[ind]->passed==2)
    {
        printf(ANSI_COLOR_RED".....student %d has an online sem.......\n"ANSI_COLOR_RESET,ind+1);
    }
    else if(studentdata[ind]->passed==3)
    {
        printf(ANSI_COLOR_RED".....student %d has an offline sem.......\n"ANSI_COLOR_RESET,ind+1);
    }
    return NULL;
}


int main()
{
    int or1;
    printf("ENTER THE NUMBER OF PHARMA COMPANIES,VACCINATION ZONES AND STUDENTS:-\n");
    scanf("%d %d %d",&n,&m,&o);
    jls=o;
    printf(ANSI_COLOR_BLUE);
    if(n<=0){
            printf("no companies to offer vaccines\n");
    }
    if(m<=0)
    {
        printf("no vaccination zones\n");
    }
    if(n<=0 || m<=0 || o<=0)
    {
        printf("simulation stopped due to impossible conditions\n");
        return 0;
    }
    int arr1[n+1],arr2[m+1],arr3[o+1];    // arr1-> pc ; arr2 -> vz ; arr3 -> stud    
    printf(ANSI_COLOR_RESET);
    printf("ENTER THE PROBABILITIES:-\n");
    
    for (int i = 0; i < n; i++)
    {
        scanf("%Lf",&pro[i]);
    }
    pthread_mutex_init(&(lfj),NULL);
    for(int i=0;i<n;i++)
    {
        arr1[i]=i;
        companydata[i]=(pcom*) malloc(sizeof(pcom));    // initialising the data used by thread
        pthread_mutex_init(&(companydata[i]->mutex),NULL);
        companydata[i]->nofbat=0;
        companydata[i]->vacperbat=0;
    }
    for(int i=0;i<m;i++){
        arr2[i]=i;
        vaczonedata[i]=(vzone*) malloc(sizeof(vzone));  // initialising the data used by thread
        vaczonedata[i]->noofslot=0;
        vaczonedata[i]->vacleft=0;
        vaczonedata[i]->status=0;
        vaczonedata[i]->providernum=-1;
        pthread_mutex_init(&(vaczonedata[i]->mutex),NULL);
    }
    
    for (int i = 0; i < o; i++)
    {
        arr3[i]=i;
        studentdata[i]=(stdt *) malloc(sizeof(stdt));   // initialising the data used by thread
        studentdata[i]->testno = 1; 
        studentdata[i]->passed = 0;
        studentdata[i]->vacz= -1;
        pthread_mutex_init(&(studentdata[i]->mutex),NULL);
    }  
    for(int i=0; i<n; i++)
    {
        pthread_create(&companythrd[i],NULL,pcom_fun,(void*)&(arr1[i]));
    }   
    for(int i=0; i<m ; i++)
    {
        pthread_create(&vaczonethrd[i],NULL,vacz_fun,(void*)&(arr2[i]));
    }
    for(int i=0; i<o ; i++)
    {
        pthread_create(&studentthrd[i],NULL,stud_fun,(void*)&(arr3[i]));
    }  
    // when thread completes automatic process is done
    for(int i=0;i<o;++i)
    {
        pthread_join(studentthrd[i],NULL);
    }
    sleep(1);
    printf("\n---------------------X SIMULATION FINISHED X--------------------\n");
    return 0;
}
