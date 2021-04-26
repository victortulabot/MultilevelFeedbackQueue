/***************************************************************
Names:  Cuasi, Beaverly
        Tulabot, Victor
Group:   48
Section:  S16
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EMPTY -2
#define DONE -1

typedef char String20[21];
typedef char String3[4];

typedef struct{
    int input[3];
    int queue[5][3];
    int process[2][100][5]; 
    // [0][X][X] Actual details of process
    // [1][X][0] process ID
    // [1][X][1] queue ID
    // [1][X][2] Start time
    // [1][X][3] End time
}Feature;

void GetInputs(FILE *fp, Feature *details){
    for(int i=0; i<5; i++){
        for(int j=0; j<3; j++){
            details->queue[i][j] = EMPTY;
        }
    }

    for(int i=0; i<2; i++){
        for(int j=0; j<100; j++){
            for(int k=0; k<5; k++){
                details->process[i][j][k] = EMPTY;
            }
        }
    }

    for(int i=0; i<3; i++){
        fscanf(fp, "%d", &details->input[i]);
        // input[0] = X ; number of queues 2 <= X <= 5
        // input[1] = Y ; number of processes 3 <= Y  <= 100
        // input[2] = S ; time period for priority boost
    }

    // queue
    for(int j=0; j<details->input[0]; j++){
        for(int k=0; k<3; k++){
            fscanf(fp, "%d", &details->queue[j][k]);
            // printf("%d ", details->queue[j][k]);
        }
        // printf("\n");
    }
    // queue[X][0] = A ; queue ID
    // queue[X][1] = B ; priority
    // queue[X][2] = C ; time quantum
    
    // process
    for(int row=0; row<details->input[1]; row++){
        for(int col=0; col<5; col++){
            fscanf(fp, "%d", &details->process[0][row][col]);
            // printf("%d ", details->process[0][row][col]);
        }
        // printf("\n");
    }
        // process[0][X][0] = F ; process ID
        // process[0][X][1] = G ; arrival time
        // process[0][X][2] = H ; total execution time
        // process[0][X][3] = I ; length of IO burst time
        // process[0][X][4] = J ; how often IO burst happens
}

void MLFQ(Feature details){
    int WT = 0, // Waiting Time
        TT = 0, // Turnaround Time
        ST = 0, // Start Time
        ET = 0, // End Time
        nProcess = details.input[1], // # of processes
        AT = 0, // Arrival Time
        PB = details.input[2]; // Priority Boost
    float AWT = 0.0; // Average Waiting Time

    int flag[nProcess][2];
    
    for(int i=0; i<nProcess; i++){
        flag[i][0] = 0;
        flag[i][1] = details.process[0][i][1];
    }
    
    int QP = details.queue[0][1];
    int lQP = 0, ilQP = 0;
    for(int i=0; i<details.input[0]; i++){
        int temp = details.queue[i][1];
        if(temp < QP && temp > DONE){
            QP = details.queue[i][1];
            lQP = details.queue[i][0];
            ilQP = i;
        }
    }

    for(int i=0; i<nProcess; i++){
        // check arrival time
        int lcAT = i; // 
        while(flag[lcAT][1] <= DONE ){
            lcAT = (int)rand() % (int)nProcess;
        }
        int cAT = details.process[0][lcAT][1];
        for(int j=0; j<details.input[1]; j++){
            int temp = flag[j][1];
            if(temp < cAT && temp > DONE){
                cAT = temp;
                lcAT = j;
            }
        }
        // check burst time
        if(flag[lcAT][0] == 0 && details.process[0][lcAT][2] <= details.queue[ilQP][2] && details.process[0][lcAT][2] >= 0){
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
            }
            else{
                ST = ET;
            }
            ET = ST + details.process[0][lcAT][2];
            printf("\nP[%d]", details.process[0][lcAT][0]);
            printf("\nQ[%d] Start time: %d End time: %d", details.queue[ilQP][0], ST, ET);
            if(details.process[0][lcAT][3] > 0){
                ST = ET + 1;
                ET = ST + details.process[0][lcAT][3];
                printf("\nIO Start time: %d End time: %d", ST, ET);
            }
            TT = ET - flag[lcAT][1];
            WT = TT - details.process[0][lcAT][2];
            printf("\nWaiting time: %d", WT);
            printf("\nTurnaround time: %d", TT);
            printf("\n************************************");
            flag[lcAT][1] = DONE;
        }
        // preemp not done
        else if(flag[lcAT][0] == 1 && details.process[0][lcAT][2] > details.queue[ilQP][2] && flag[lcAT][1] > DONE){
            flag[lcAT][0] = 1;
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
            }
            else{
                ST = ET;
            }
            ET = ET + details.queue[ilQP][2];
            details.process[1][i][0] = details.process[0][lcAT][0];
            details.process[1][i][1] = details.queue[ilQP][0];
            details.process[1][i][2] = ST;
            details.process[1][i][3] = ET;

            int ncAT = details.process[0][lcAT][1];
            for(int j=0; j<nProcess; j++){
                if(flag[lcAT][1] > )
            }
        }
        // print preemp done
        else if(flag[lcAT][0] == 1 && flag[lcAT][1] == DONE){
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
            }
            else{
                ST = ET;
            }
            ET = ST + details.process[0][lcAT][2];
            printf("\nP[%d]", details.process[0][lcAT][0]);
            printf("\nQ[%d] Start time: %d End time: %d", details.queue[ilQP][0], ST, ET);
            if(details.process[0][lcAT][3] > 0){
                ST = ET + 1;
                ET = ST + details.process[0][lcAT][3];
                printf("\nIO Start time: %d End time: %d", ST, ET);
            }
            TT = ET - flag[lcAT][1];
            WT = TT - details.process[0][lcAT][2];
            printf("\nWaiting time: %d", WT);
            printf("\nTurnaround time: %d", TT);
            printf("\n************************************");
        }
    }

    /*
    Sample Print
        P[1]
        Q[1] Start time: 1 End time: 3
        Q[2] Start time: 6 End time: 7
        Q[1] Start time: 12 End time: 14
        Q[2] Start time: 18 End time: 21
        IO Start time: 1 End time: 1
        Waiting time: 2
        Turnaround time: 2

        P[3]
        Q[1] Start time: 7 End time: 8
        IO Start time: 6 End time: 7
        Q[1] Start time: 14 End time: 15
        IO Start time: 15 End time: 16
        Q[1] Start time: 19 End time: 20
        Q[1] Start time: 23 End time: 24
    */

    AWT = AWT / details.input[1];
    printf("\nAverage Waiting Time: %.2f", AWT);
}

int main(){
    srand((unsigned int)time(NULL));
    FILE *fp;
    String20 filename;
    Feature details;

    //ask user for input file
    printf("Input filename: ");
    scanf("%s", filename);

    //open input file
    if((fp = fopen(filename,"r")) == NULL){
            printf("%s not found.", filename);
            exit(1);
        }
    //get inputs
    GetInputs(fp,&details);
    fclose(fp);

    MLFQ(details);
    
    return 0;
}