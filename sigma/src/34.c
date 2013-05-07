/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         34
Model Description:  
Output File:        untitled.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           SINGLE_STEP
Trace Vars:         INV[0;0],INV[0;1],INV[0;2],INV[1;0],INV[1;1],INV[1;2],INV[2;0],INV[2;1],INV[2;2],REJECT
Random Number Seed: 12345
Initial Values:     3,3,5,5
Ending Condition:   STOP_ON_TIME
Ending Time:        100.000
Trace Events:       ALL EVENTS TRACED
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

#include <math.h>

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   RUN(void);	/*** Starting the Simulation ***/
void   FOR(void);	/*** Starting the simulation for each location ***/
void   ENTER(void);	/*** Customers arrive for each location, no need for state changes since we do not have a queue in this problem ***/
void   RENT(void);	/*** People Rent Cars from Location I ***/
void   RETURN(void);	/*** Customers are returning Cars to I ***/
void   REJECTI(void);	/*** # of customers rejected at each location ***/
void   DATA0(void);	/*** Acquiring data that have N * Type Array Information ***/
void   UPGRADE(void);	/*** Checking to see if the person can be upgraded to a better car model ***/
void   DATA1(void);	/*** Loading data which has N * N Array Information ***/
void   CDF(void);	/*** Generating K which is equal to the city number that the car is going to ***/
void   THRESH(void);	/*** Checking if Inventory from nearest cities can be transferred to current city ***/
void   INVINC(void);	/*** Increasing inventory of current location ***/
void   INVDEC(void);	/*** Decreasing Inventory of nearest location ***/
void   SORT(void);	/*** Sorting cities in terms of proximity to city I ***/
void   SEASONA(void);	/*** Stores the Seasonal Factors ***/

/*   STATE VARIABLE DELARATIONS   */
long   I;	/***  Location Index  ***/
long   INV[50][50];	/***  Inventory at Location i  ***/
long   N;	/***  Number Of Locations  ***/
long   REJECT;	/***  Number Of Rejected Customers  ***/
long   REJ[3];	/***  Rejections Per Location  ***/
long   J;	/***  Car Type Index  ***/
double POS[50][50];	/***  Distance Array  ***/
double RATE[50][50];	/***  Distribution Rate for Each Node  ***/
double P[50][50];	/***  Probability of returning the car from i to j  ***/
double LOWER[50][50];	/***  Lower Value  ***/
long   DESTIN;	/***  Index of Ending Destination of Car  ***/
double TEMP;	/***  Temporary Variable  ***/
long   K;	/***  Return Location Index  ***/
long   R[50][50];	/***  Rejection of Each Car Type at Each Location  ***/
long   TYPE;	/***  Number of Types of Cars at every location  ***/
double TOTAL;	/***  Stores the value of the cumulative distribution  ***/
long   BATCH;	/***  Cars transferred if inv drops below threshold  ***/
double UPPER[50][50];	/***  Upper Value  ***/
double TEMP2;	/***  Temporary Variable  ***/
double TEMP3;	/***  Temporary Variable  ***/
long   THRESHOL[50][50];	/***  Threshold values for each type at each location  ***/
long   TEMP4;	/***  Temporary Variable  ***/
long   INDI[50][50];	/***  Indicator Variable  ***/
double MIN;	/***  Temporary Variable to Interchange Positions  ***/
long   SORTED[50][50];	/***  Stores cities in ascending order of distance  ***/
double ADD;	/***  Adding the cities in the row  ***/
long   TEMP5;	/***  Temporary Variable to get closest city number  ***/
double A[50][50];	/***  Alpha Value  ***/
double B[50][50];	/***  Beta Value  ***/
long   TEMP6;	/***  Stores Year Number  ***/
double GROWTH;	/***  Stores Annual Growth Rate  ***/
double SEASON[12];	/***  Stores Seasonal Data Values  ***/
long   TEMP7;	/***  Temporary Variable to store month number  ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   RUN_event,
   FOR_event,
   ENTER_event,
   RENT_event,
   RETURN_event,
   REJECTI_event,
   DATA0_event,
   UPGRADE_event,
   DATA1_event,
   CDF_event,
   THRESH_event,
   INVINC_event,
   INVDEC_event,
   SORT_event,
   SEASONA_event,
   };

/*    MAIN PROGRAM     */
int main(int argc, char** argv)
{
  int  next_event;
  char keytoclose = 'p';

  if(!startup_check(0))
    return -1;

  /* Initialize csiglib and simulation */
  while (initialize(argc, (const char * *)argv)) {;

  /* Schedule beginning of simulation */
  event_time = current_time;
  event_type = RUN_event;
  schedule_event();

  /* Schedule end of simulation */
  event_time = stop_time;
  event_type = run_end_event;
  event_priority = 9999;
  schedule_event();

/*  EVENT EXECUTION CONTROL LOOP */
  while (!run_error && !done) {
    /* Pull next event from event list */
    next_event = c_timing();

    /* increment the event count for this event */
    event_count[next_event]++;

    /* Call appropriate event routine */
    switch ( next_event ) {
      case run_end_event:  run_end();
               break;

      case RUN_event:  RUN();
               event_trace("RUN",event_count[next_event]);
               break;

      case FOR_event:  FOR();
               event_trace("FOR",event_count[next_event]);
               break;

      case ENTER_event:  ENTER();
               event_trace("ENTER",event_count[next_event]);
               break;

      case RENT_event:  RENT();
               event_trace("RENT",event_count[next_event]);
               break;

      case RETURN_event:  RETURN();
               event_trace("RETURN",event_count[next_event]);
               break;

      case REJECTI_event:  REJECTI();
               event_trace("REJECTI",event_count[next_event]);
               break;

      case DATA0_event:  DATA0();
               event_trace("DATA0",event_count[next_event]);
               break;

      case UPGRADE_event:  UPGRADE();
               event_trace("UPGRADE",event_count[next_event]);
               break;

      case DATA1_event:  DATA1();
               event_trace("DATA1",event_count[next_event]);
               break;

      case CDF_event:  CDF();
               event_trace("CDF",event_count[next_event]);
               break;

      case THRESH_event:  THRESH();
               event_trace("THRESH",event_count[next_event]);
               break;

      case INVINC_event:  INVINC();
               event_trace("INVINC",event_count[next_event]);
               break;

      case INVDEC_event:  INVDEC();
               event_trace("INVDEC",event_count[next_event]);
               break;

      case SORT_event:  SORT();
               event_trace("SORT",event_count[next_event]);
               break;

      case SEASONA_event:  SEASONA();
               event_trace("SEASONA",event_count[next_event]);
               break;

      }
    }
  }
// experiments terminated
printf("Experiments ended! If runs end early: \n\r1. check fields in *.exp file. \n\r2. check if output file was already open. \n\r");
return 0;
}

void
event_trace(const char * name_of_event,const long count)
{
  c_timest(INV[0][0], 1, 0);
  c_sampst(INV[0][0], 1, 0);
  c_timest(INV[0][1], 2, 0);
  c_sampst(INV[0][1], 2, 0);
  c_timest(INV[0][2], 3, 0);
  c_sampst(INV[0][2], 3, 0);
  c_timest(INV[1][0], 4, 0);
  c_sampst(INV[1][0], 4, 0);
  c_timest(INV[1][1], 5, 0);
  c_sampst(INV[1][1], 5, 0);
  c_timest(INV[1][2], 6, 0);
  c_sampst(INV[1][2], 6, 0);
  c_timest(INV[2][0], 7, 0);
  c_sampst(INV[2][0], 7, 0);
  c_timest(INV[2][1], 8, 0);
  c_sampst(INV[2][1], 8, 0);
  c_timest(INV[2][2], 9, 0);
  c_sampst(INV[2][2], 9, 0);
  c_timest(REJECT, 10, 0);
  c_sampst(REJECT, 10, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g \n"
,(double)INV[0][0], (double)INV[0][1], (double)INV[0][2], (double)INV[1][0], (double)INV[1][1], (double)INV[1][2], (double)INV[2][0], (double)INV[2][1], (double)INV[2][2], (double)REJECT);
}



int
initialize(int argc, const char** argv)
{
static int first_time = 1;     /* First time in initialize? */
static FILE *input_fp;     /* For reading from the input file */
char *exp_file_name;       /* For constructing input file name */
char y_n = 'p';            /* yes/no for file overwrite*/

       char dir[256];
       char fname[256];
       char ext[256];
       char simulation[1024];
       char experient_name[1024];
        _splitpath( argv[0], NULL, dir, fname, ext );
       strcpy(simulation, fname);
       strcat(simulation, ext);
       strcpy(experient_name, fname);
       strcat(experient_name, ".exp");
     printf("Running the simulation: %s\n", simulation);
    if(strlen(dir) !=0)
       printf("In Path: %s\n",dir);
  if (first_time) {
    exp_file_name = _strdup(argv[0]);
    exp_file_name[strlen(exp_file_name)-1] = 'p';
    printf("\nLooking for experiment file: %s\n",experient_name);
    }

  if ((first_time && (input_fp=fopen(exp_file_name,"r"))!=NULL)
                                           || input_fp!=NULL) {
  if (first_time) {
     first_time = 0; /* Reset for next time into initialize */
     printf("Found. Use [Control]-C to abort replications.\n");
     }

  /* We have run control file of type *.exp          */
  /* Read next set of data from run control file.    */
  if (fscanf(input_fp,"%s %1s %ld %lf %d", output_file_name, &y_n, &rndsd, &stop_time, &trace_flag)<4
     || fscanf(input_fp,"%ld", &N)<1
     || fscanf(input_fp,"%ld", &TYPE)<1
     || fscanf(input_fp,"%ld", &BATCH)<1
     || fscanf(input_fp,"%lf", &GROWTH)<1
     ) {
     /* End of run control file */
     fclose(input_fp);
     return 0;
     }

  if (y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N') { 
  fprintf(stderr,"INPUT ERROR: Invalid append file flag in *.exp file: (y=append, n=overwrite old file)\n"); 
  return 0; 
  }

  if (y_n == 'y' || y_n == 'Y') {
     if ((output_fp = fopen(output_file_name,"a"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
  return 0; 
  }
  }
if (y_n == 'n' || y_n == 'N') {
     if ((output_fp = fopen(output_file_name,"w"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
     return 0;
     }
     }

  if (rndsd < 1 || rndsd > 65534) {
     fprintf(stderr,"\nINPUT ERROR: Random seed %ld is not between 0 and 65534\n",rndsd);
     return 0;
     }

  if (stop_time <= 0.0) {
     fprintf(stderr,"\nINPUT ERROR: Stopping time %lf is negative!\n",stop_time);
     return 0;
     }

  if (trace_flag != 0 && trace_flag != 1) {
     fprintf(stderr,"\nINPUT ERROR: Invalid trace_flag=%d: (1=full trace, 0=summary only)\n",trace_flag);
     return 0;
     }

  done = 0;
     }

 else if (first_time) { /* And open failed, implies data from stdin */
    first_time = 0; /* Reset for next time into initialize */
    printf("Not found, input data at the keyboard.\n");
     /* Give output file name */
     while(y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N')
         {
         printf("\nOUTPUT FILE (Enter File Name with Path):\n");
         scanf("%s", output_file_name);
         fflush(stdin);
         sprintf(filename,"%.20s", output_file_name);
         printf("WARNING:File %.20s must not be open!!\n If file does not exist it will be created.\n",filename);
         printf("Do you want the new output appended to this file? (yes/[no])\n");
         scanf("%1s",&y_n);
         fflush(stdin);
         }
     if(y_n == 'y' || y_n == 'Y') output_fp = fopen(filename,"a");
     if(y_n == 'n' || y_n == 'N') output_fp = fopen(filename,"w");

     /* Read in random number seed */
     printf("\n\nRANDOM NUMBER SEED (Enter Integer Between 0 and 65534):\n");
     scanf("%ld", &rndsd);
     fflush(stdin);

     /* Read in run stopping time */
     printf("\nSTOPPING TIME (Enter number of time units until run termination):\n");
     scanf("%lf", &stop_time);
     fflush(stdin);

     /* Read in trace_flag */
     printf("\n\nTRACE (1 = Event Trace, 0 = Summary Only):\n");
     scanf("%d", &trace_flag);
     fflush(stdin);

     /* Parameters for the initial event */;
     printf ( "\nEnter initial value for N: \n");
     scanf  ( "%ld", &N);
     printf ( "\nEnter initial value for TYPE: \n");
     scanf  ( "%ld", &TYPE);
     printf ( "\nEnter initial value for BATCH: \n");
     scanf  ( "%ld", &BATCH);
     printf ( "\nEnter initial value for GROWTH: \n");
     scanf  ( "%lf", &GROWTH);
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	        INV[0;0]");
   fprintf(output_fp,"	        INV[0;1]");
   fprintf(output_fp,"	        INV[0;2]");
   fprintf(output_fp,"	        INV[1;0]");
   fprintf(output_fp,"	        INV[1;1]");
   fprintf(output_fp,"	        INV[1;2]");
   fprintf(output_fp,"	        INV[2;0]");
   fprintf(output_fp,"	        INV[2;1]");
   fprintf(output_fp,"	        INV[2;2]");
   fprintf(output_fp,"	            REJECT ");
   fprintf(output_fp,"\n");
   }
  /* Initialize CSIGLIB variables and files */
  c_initlk(rndsd);
  c_initfiles();

  return(1);
}



void
run_end()
{
  printf("\r\nNormal completion after %f time units\n",current_time);
  printf("The Next Seed In the Random Input Stream is %ld\n",rndsd);

  ///  Summary statistics ///
  fprintf(output_fp,"SUMMARY STATISTICS\n");
  printf("SUMMARY STATISTICS\n");
  c_timest(INV[0][0], 1, 1);
   fprintf(output_fp, "INV[0][0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[0][0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[0][0], 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[0][1], 2, 1);
   fprintf(output_fp, "INV[0][1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[0][1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[0][1], 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[0][2], 3, 1);
   fprintf(output_fp, "INV[0][2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[0][2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[0][2], 3, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[1][0], 4, 1);
   fprintf(output_fp, "INV[1][0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[1][0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[1][0], 4, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[1][1], 5, 1);
   fprintf(output_fp, "INV[1][1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[1][1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[1][1], 5, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[1][2], 6, 1);
   fprintf(output_fp, "INV[1][2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[1][2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[1][2], 6, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[2][0], 7, 1);
   fprintf(output_fp, "INV[2][0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[2][0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[2][0], 7, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[2][1], 8, 1);
   fprintf(output_fp, "INV[2][1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[2][1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[2][1], 8, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(INV[2][2], 9, 1);
   fprintf(output_fp, "INV[2][2]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("INV[2][2]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(INV[2][2], 9, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(REJECT, 10, 1);
   fprintf(output_fp, "REJECT:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("REJECT:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(REJECT, 10, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  printf("Output written to, %s\n",output_file_name);
  fclose(output_fp);
  c_closedisk();
  done = 1;
}



/****************************/
/*     EVENT FUNCTIONS      */
/****************************/

/*** Starting the Simulation ***/
void
RUN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */

  /* state changes */
  RNK[1]=2;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Starting Loading of Different Data  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 0;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA0_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Starting the simulation for each location ***/
void
FOR()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( (J<(TYPE-1))&&(I<=(N-1)) );
  _edge_condition[2] = ( (J==(TYPE-1))&&(I<(N-1)) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Starting entry simulation for each location  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + RATE[I][J]*ERL(1);
    event_type = ENTER_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Incremental increase of car type at a location  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = FOR_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Increase in Location Index  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I+1;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = FOR_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Customers arrive for each location, no need for state changes since we do not have a queue in this problem ***/
void
ENTER()
{
int  _edge_condition[4];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  TEMP=RND;
  TEMP6=(CLK/8640);
  TEMP7=MOD(CLK,8640)/720;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( INV[I][J]>0 );
  _edge_condition[2] = ( INV[I][J]==0&&J==(TYPE-1) );
  _edge_condition[3] = ( INV[I][J]==0&&J<(TYPE-1) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Time Delay (hrs) between arrivals, for now independent of location or class  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + SEASON[TEMP7]*RATE[I][J]* pow((1+(GROWTH/100)), TEMP6) *ERL(1);
    event_type = ENTER_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checking to see if car types at a location are available  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = TEMP;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = RENT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Checking to see if the cars at location I is equal to 0  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = REJECTI_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[3])
    /***  Checking to see if inventory of the car type is equal to 0  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    transfer[5] = J;
    transfer[6] = TEMP;
    for ( t_index=7; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = UPGRADE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** People Rent Cars from Location I ***/
void
RENT()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  TEMP = transfer[5];

  /* state changes */
  INV[I][J]=INV[I][J]-1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( INV[I][J]<THRESHOL[I][J]&&INDI[I][J]==0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking to see where the car is going  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = 0;
    transfer[6] = TEMP;
    transfer[7] = 0;
    for ( t_index=8; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = CDF_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checks if threshold value has been breached  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = 1;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = THRESH_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Customers are returning Cars to I ***/
void
RETURN()
{
  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  K = (long) transfer[5];

  /* state changes */
  INV[K][J]=INV[K][J]+1;
}


/*** # of customers rejected at each location ***/
void
REJECTI()
{
  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  REJ[I]=REJ[I]+1;
  REJECT=REJECT+1;
  R[I][J]=R[I][J]+1;
}


/*** Acquiring data that have N * Type Array Information ***/
void
DATA0()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  INV[I][J]=DISK(INVENTORY.DAT,(I*TYPE)+J+1);
  RATE[I][J]=DISK(ARRIVAL.DAT,(I*TYPE)+J+1);
  THRESHOL[I][J]=DISK(THRESHOLD.DAT,(I*TYPE)+J+1);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( (J<(TYPE-1))&&(I<=(N-1)) );
  _edge_condition[1] = ( (J==(TYPE-1))&&(I<(N-1)) );
  _edge_condition[2] = ( (I==N-1)&&(J==TYPE-1) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking to see if all the data in a row has been added  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA0_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checking to see if it is ready to load the next row of data  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I+1;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA0_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Loading some more data  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 0;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA1_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Checking to see if the person can be upgraded to a better car model ***/
void
UPGRADE()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  K = (long) transfer[5];
  TEMP = transfer[6];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( INV[I][J]>0 );
  _edge_condition[1] = ( INV[I][J]==0&&J==(TYPE-1) );
  _edge_condition[2] = ( INV[I][J]==0&&J<(TYPE-1) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking if upgrade can happen, and if it can then the customer is provided with the upgrade  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = TEMP;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = RENT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checking to see if upgrade cannot take place  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = K;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = REJECTI_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Checking to see if upgrade is an option  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    transfer[5] = K;
    transfer[6] = TEMP;
    for ( t_index=7; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = UPGRADE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Loading data which has N * N Array Information ***/
void
DATA1()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  POS[I][J]=DISK(DIST.DAT,(I*N)+J+1);
  P[I][J]=DISK(PROBABILITY.DAT,(I*N)+J+1);
  LOWER[I][J]=DISK(USETIME.DAT,(I*TYPE*4)+(4*J)+1);
  UPPER[I][J]=DISK(USETIME.DAT,(I*TYPE*4)+(4*J)+2);
  A[I][J]=DISK(USETIME.DAT,(I*TYPE*4)+(4*J)+3);
  B[I][J]=DISK(USETIME.DAT,(I*TYPE*4)+(4*J)+4);
  ENT[1]=J;
  ENT[2]=POS[I][J];
  ADD=ADD+PUT(INC,1);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( (J<(N-1))&&(I<=(N-1)) );
  _edge_condition[1] = ( (J==(N-1))&&(I<(N-1)) );
  _edge_condition[2] = ( (J==N-1) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking to see if there is more data in the row  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA1_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checking to see if we are ready to load the next row of data  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I+1;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = DATA1_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Sorting an Array to compute city numbers with respect to distance from each city in ascending order  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SORT_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Generating K which is equal to the city number that the car is going to ***/
void
CDF()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  K = (long) transfer[5];
  TEMP = transfer[6];
  TOTAL = transfer[7];

  /* state changes */
  TOTAL=TOTAL+P[I][K];
  TEMP2=A[I][K];
  TEMP3=B[I][K];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( TOTAL>=TEMP );
  _edge_condition[1] = ( TOTAL<TEMP );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Modelling Usage Time of a Car  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = K;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + (POS[I][K]/60)+LOWER[I][K]+(UPPER[I][K]-LOWER[I][K])*BET(TEMP2,TEMP3);
    event_type = RETURN_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Looping till exit condition is met  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = K+1;
    transfer[6] = TEMP;
    transfer[7] = TOTAL;
    for ( t_index=8; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = CDF_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Checking if Inventory from nearest cities can be transferred to current city ***/
void
THRESH()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  TEMP4 = (long) transfer[5];

  /* state changes */
  TEMP5=SORTED[I][TEMP4];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( INV[TEMP5][J]<THRESHOL[TEMP5][J]+BATCH&&TEMP4<N-1 );
  _edge_condition[1] = ( INV[TEMP5][J]>=THRESHOL[TEMP5][J]+BATCH );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking if inventory of closest city is not greater than its threshold value plus batch size  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = TEMP4+1;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = THRESH_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Checking if Inventory of city is greater than its threshold value plus batch size  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    transfer[5] = TEMP5;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = INVDEC_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Increasing inventory of current location ***/
void
INVINC()
{
  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  INV[I][J]=INV[I][J]+BATCH;
  INDI[I][J]=0;
}


/*** Decreasing Inventory of nearest location ***/
void
INVDEC()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];
  TEMP5 = (long) transfer[5];

  /* state changes */
  INV[TEMP5][J]=INV[TEMP5][J]-BATCH;
  INDI[I][J]=1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Modeling Delay Time to transfer cars from city to city  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + POS[TEMP5][I]/60;
    event_type = INVINC_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Sorting cities in terms of proximity to city I ***/
void
SORT()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];
  J = (long) transfer[4];

  /* state changes */
  ADD=ADD-GET(FST,1);
  SORTED[I][J]=ENT[1];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( J<N-1 );
  _edge_condition[1] = ( I==(N-1)&&J==(N-1) );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Checking to see if there are more elements in the row  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    transfer[4] = J+1;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SORT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Storing value of seasonal factors for arrivals  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 1;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SEASONA_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Stores the Seasonal Factors ***/
void
SEASONA()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];

  /* state changes */
  SEASON[I-1]=DISK(SEASONALITY.DAT,I);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( I==12 );
  _edge_condition[1] = ( I<12 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Starting to model entries for each location and each car type  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 0;
    transfer[4] = 0;
    for ( t_index=5; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = FOR_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Making sure all months seasonal factors are stored  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I+1;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SEASONA_event;
    event_priority = 5;
    schedule_event();
    }

}

