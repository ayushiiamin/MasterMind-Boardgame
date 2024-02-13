//https://www.geeksforgeeks.org/generating-random-number-range-c/ ..random within range
//https://cboard.cprogramming.com/c-programming/112560-timers-c.html ..for time delay

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>       //for o_rdwr, o_sync, o_cloexec
#include<sys/mman.h>
#include<stdint.h>      //for uint32_t
#include<string.h>
#include<time.h>
#include<sys/time.h>


#define BLOCK_SIZE (4*1024)
#define GPIO_BASE 0x3f200000

#define GREEN 13        //green LED is connected to GPIO pin 13
#define RED 5           //red LED is connected to GPIO pin 5
#define BUTTON 19       //button LED is connected to GPIO pin 19



volatile unsigned int * gpio;       //declaration for pointer to GPIO registers in memory

int N,C,noOfTurns;      //declaration for N- length of sequence, C- no of colours and noOfTurns



void timeDelay(long time)       //function to cause delay for time seconds
{

    time_t start,end;
    double time1;

    start=clock();
    //https://cboard.cprogramming.com/c-programming/112560-timers-c.html
    while(time1=((clock()-start)/CLOCKS_PER_SEC)<time) //while loop to cause delay
    {

    }

    // printf("timeDelay() over\n");
}


void blinkLED(int pin, int noOfTimes)   //function for making the LED blink 
{
    
    int setOff,clrOff;
    if(pin>=32)     //GPSET1 and GPCLR1 if pin>=32
    {
        setOff=8;
        clrOff=11;
    }
    else        //GPSET0 and GPCLR0 if pin<32
    {
        setOff=7;
        clrOff=10;
    }

    // printf("setoff %d and clroff %d\n",setOff,clrOff);
    
    for(int i=0;i<noOfTimes;i++)
    {
        
        //we set to HIGH
        *(gpio+setOff) = 1<<(pin & 31);

        // printf("timeDelay1 for %d\n",i);

        timeDelay(1);       //time delay of 1 second

        //we set to LOW
        *(gpio+clrOff) = 1<<(pin & 31);

        // printf("timeDelay2 for %d\n",i);

        timeDelay(1);       //time delay of 1 second
    }
}



void setOutputMode(int pin)     //function to set pin to output mode- 001
{
    int fSel,shift;
    fSel = pin/10;
    shift = (pin%10)*3;
    *(gpio+fSel) = (*(gpio + fSel) & ~(7 << shift))| (1 << shift) ;     //set bits to one

}

void setInputMode(int pin)      //function to set pin to input mode- 000
{
    int fSel,shift;
    fSel= pin/10;
    shift = (pin%10)*3;
    *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift));      //set bits to zero
}

int getButtonValue(int pin)     //function to check when the button is pressed
{
    int gplev;
    if(pin>=32)     //GPLEV1 when pin>=32
    {
        gplev = 14;
    }
    else        //GPLEV0 when pin<32
    {
        gplev=13;
    }
    if(  ( *(gpio+gplev) &(1 << (pin & 31)))!=0)    //returns 1 when button is pressed
    {
        return 1;
    }
    else        //returns 0 when button not pressed
    {
        return 0; 
    }

}

int buttonClick()       //function to return the number of times the button is pressed
{
    int buttonClickNo=0;
    
    fprintf(stderr,"click the button within 7 seconds:\n");
    

    time_t start;
    double time1;

    start=clock();

    while(time1=((clock()-start)/CLOCKS_PER_SEC)<7) //while loop to count the number of times the button is clicked for 7 seconds
    {
        if(getButtonValue(BUTTON))      //when button is pressed
        {
            while(getButtonValue(BUTTON)!=0);       //loop till button is released
            buttonClickNo++;        //increment buttonClickNo
        }

    }
    fprintf(stderr,"7 secs over\n");
    blinkLED(RED,1);        //input accepted
    if((buttonClickNo<1)||(buttonClickNo>C)) //true if button is not clicked or button is clicked more number of times
    {                                           // than the number of colours
        fprintf(stderr,"WRONG NUMBER OF BUTTON CLICKS\n");
        blinkLED(RED,5);   
        buttonClick();      //calls the function again
    }
    blinkLED(GREEN,buttonClickNo);      //echo the input value
    return buttonClickNo;


}









void main(int argc, char* argv[])
{
    static volatile unsigned int gpiobase = 0x3F200000;
    int fd;

    int debugMode=0, unitTestMode=0;        //flag for debug or unit test mode

    


    if(argc>1)
    {
        if(strcmp(argv[1],"-d")==0)     //if ran in debug mode
        {
            debugMode=1;
        }
        else if(strcmp(argv[1],"-u")==0)      //if ran in unit test mode
        {
            unitTestMode=1;
        }
        else
        fprintf(stderr,"invalid argument");
    }

    if(unitTestMode==1)
    {
        char* secret = argv[2];
        char* guess = argv[3];
        int exact=0, approximate=0;
        int * guessFlag = (int*) malloc(sizeof(int)*N);     //an array of guess flags
        int * secretFlag=(int*) malloc(sizeof(int)*N);      //an array of secret flags
        for(int j=0;j<strlen(secret);j++)
        {
            guessFlag[j]= secretFlag[j]=0;
            if(secret[j]==guess[j])     //if correct colour in correct position
            {
                exact++;
                guessFlag[j]=secretFlag[j]=1;       //flag set to 1
            }
        }

        //check for approximate number of corrects
        for(int j=0;j<strlen(secret);j++)  //for guess
        {
            if(guessFlag[j]==0)
            {
                for(int k=0;k<strlen(secret);k++)  //for secret
                {
                    if((guess[j]==secret[k])&&(secretFlag[j]==0))  //if correct colour in wrong position
                    {
                        approximate++;
                    }
                }

            }
            
        }
        printf("%d exact\n",exact);
        printf("%d approximate\n",approximate);

        free(guessFlag);
        free(secretFlag);


        exit(0);

    }

    if((fd= open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC) )< 0)
    {
        fprintf(stderr,"cannot open /dev/gpiomem\n");
        exit(0);

    }

   

    gpio = (uint32_t *) mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE) ;
    if((int32_t)gpio==-1)
    { 
        fprintf(stderr,"can't mmap\n"); 
        exit(0);
    }

    

    //set green led in pin to output mode
    setOutputMode(GREEN);
    //set red led in pin to output mode
    setOutputMode(RED);
    //set button in pin to input mode
    setInputMode(BUTTON);

    printf("MASTERMIND\n");
    printf("__________\n");

    // C=3;
    // N=3;
    // noOfTurns=5;

    fprintf(stderr,"Enter the number of colours:");
    scanf("%d",&C);
    fprintf(stderr,"Enter the length of the sequence:");
    scanf("%d",&N);
    fprintf(stderr,"Enter the number of turns:");
    scanf("%d",&noOfTurns);
    fprintf(stderr,"\n\n");

    int * secret;
    secret = (int *) malloc(sizeof(int)*N);


    
    
    for(int i=0;i<N;i++)        //for loop to generate a random secret
    {
        int no = (rand()% C) +1;  //upper=C lower=1
        //https://www.geeksforgeeks.org/generating-random-number-range-c/
        secret[i]=no;
    }

    

    if(debugMode==1)
    {
        printf("Secret: ");
        for(int i=0;i<N;i++)
        {
            printf("%d ",secret[i]);
        }
        printf("\n____________\n");
    }

    int *guess;
    guess = (int*) malloc(sizeof(int)*N);

   

    blinkLED(GREEN,1);      //to indicate that the game starting

    printf("game started\n");

    // printf("noofTurns=%d\n",noOfTurns);
    // printf("N value\n",N);

    for(int i=0;i<noOfTurns;i++)
    {
        if(debugMode==1)
        {
            printf("\n\n\nGuess %d: ",(i+1));
        }
        for(int j=0;j<N;j++)  //to get the guess from the user
        {
            // printf("in j loop %d",j);

            guess[j]=buttonClick();     
            if(debugMode==1)
            {
                printf("%d ",guess[j]);
            }
        }

        //input completed
        blinkLED(RED,2);

        // printf("Secret: %d %d %d\n",secret[0],secret[1],secret[2]);
        // printf("Guess: %d %d %d\n",guess[0],guess[1],guess[2]);


        //check if guess is correct

        //check for exact number of corrects
        int exact=0, approximate=0;
        int * guessFlag = (int*) malloc(sizeof(int)*N);     //an array of guess flags
        int * secretFlag=(int*) malloc(sizeof(int)*N);      //an array of secret flags
        for(int j=0;j<N;j++)
        {
            guessFlag[j]= secretFlag[j]=0;
            if(secret[j]==guess[j])     //if correct colour in correct position
            {
                exact++;
                guessFlag[j]=secretFlag[j]=1;       //flag set to 1
            }
        }

        //check for approximate number of corrects
        for(int j=0;j<N;j++)  //for guess
        {
            if(guessFlag[j]==0)
            {
                for(int k=0;k<N;k++)  //for secret
                {
                    if((guess[j]==secret[k])&&(secretFlag[j]==0))     //if correct colour in wrong position
                    {
                        approximate++;
                    }
                }

            }
            
        }

        free(secretFlag);
        free(guessFlag);

        if(debugMode==1)
        {
            printf("\nAnsw %d: %d %d\n\n",(i+1),exact,approximate);
        }

        //blink the exact matches
        blinkLED(GREEN,exact);
        blinkLED(RED,1);
        //blink the approximate matches
        blinkLED(GREEN,approximate);

        if(exact==N)        //game won
        {
            int setOff,clrOff;
            
                setOff=7;       //GPSET0 and GPCLR0 as RED=13<32
                clrOff=10;
           
            *(gpio+setOff) = 1<<(RED & 31);     //RED set to HIGH
            blinkLED(GREEN,3);      //green LED blinks 3 times
            *(gpio+clrOff) = 1<<(RED & 31);     //RED set to LOW
            if(debugMode==1)
            {
                printf("Game finished in %d moves\n",(i+1));
            }
            break;
        }
        else    //if guess is wrong
        {
            blinkLED(RED,3);    //red blinks 3 times and next turn begins
        }

    }

    free(secret);
    free(guess);

    fprintf(stderr,"GAME OVER\n");

}