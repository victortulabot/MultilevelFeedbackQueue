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
    int process[3][100][6]; 
    // [0][X][X] Actual details of process
    // [0][X][6] current queue in
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

    for(int i=0; i<3; i++){
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
        PB = details.input[2], // Priority Boost
        cCPUT = 0; // CPU Time
    float AWT = 0.0; // Average Waiting Time

    int flag[nProcess][2],
        PRQ[details.input[1]][details.input[0]];
    
    for(int i=0; i<nProcess; i++){
        flag[i][0] = 0; // preemp flag
        flag[i][1] = details.process[0][i][1]; // arrival time
    }
    
    int QP = details.queue[0][1];
    int lQP = 0, ilQP = 0;
    // look for highest queue priority
    for(int i=0; i<details.input[0]; i++){
        int temp = details.queue[i][1];
        if(temp < QP && temp > DONE){
            QP = details.queue[i][1];
            lQP = details.queue[i][0];
            ilQP = i;
        }
    }
    for(int i=0; i<nProcess; i++){
        for(int j=0; j<details.input[0]; j++){
            PRQ[i][j] = 0; // other queues marked to 0
            PRQ[i][ilQP] = 1; // highest priority to be used
        }
    }
    for(int row=0; row<details.input[1]; row++){
        details.process[0][row][5] = ilQP;
    }
    int key = 0;
    int keyPB = 0;
    int keyIO = 0;
    for(int i=0; i<nProcess; i++){
        // check arrival time
        int lcAT = i; // 
        while(flag[lcAT][1] <= DONE || lcAT >= details.input[1]){
            lcAT = (int)rand() % (int)details.input[1];
        }
        int cAT = details.process[0][lcAT][1]; // original arrival time
        for(int j=0; j<details.input[1]; j++){
            int temp = flag[j][1]; // changing arrival time
            if(temp < cAT && temp > DONE){
                cAT = temp;
                lcAT = j;
            }
        }
        int cQ = details.process[0][lcAT][5]; // current queue in of the process
        // look for the marked 1 in PRQ[lcAT][X]
        for(int k=0; k<details.input[0]; k++){
            if(PRQ[lcAT][k] == 1){
                cQ = k;
            }
        }
        // check burst time
        if(flag[lcAT][0] == 0 && details.process[0][lcAT][2] <= details.queue[cQ][2] && details.process[0][lcAT][2] >= 0){
            // iterate CPU Time
            for(int v=0; v<details.process[0][lcAT][2]; v++){
                cCPUT++;
                // printf("\nCPU: %d\n", cCPUT);
            }
           
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
                ET = ST;
            }
            else{
                ST = ET;
            }
            ET = ST + details.process[0][lcAT][2];
            printf("\nP[%d]", details.process[0][lcAT][0]);
            printf("\nQ[%d] Start time: %d End time: %d", details.queue[cQ][0], ST, ET);
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
        else if(details.process[0][lcAT][2] > details.queue[cQ][2] && flag[lcAT][1] > DONE){
            int temp2 = lcAT;
            flag[lcAT][0] = 1;
            nProcess++;
            // iterate CPU Time
            for(int v=0; v<details.process[0][lcAT][2]; v++){
                cCPUT++;
                // printf("\nCPU: %d\n", cCPUT);
                if(cCPUT % PB == 0){
                    keyPB = 1;
                }
            }
            

            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
                ET = ST;
            }
            else{
                ST = ET;
            }

            /*
                When IO is present, run a process from the lower queue = to the IO burst

            */
            // check for IO
            keyIO = 0;
            int tempST = ST;
            int tempET = ET;
            if(details.process[0][lcAT][3] > 0){
                
                for(int v=0; v<details.process[0][lcAT][2]; v++){
                    tempST++;
                    
                    if(tempST % details.process[0][lcAT][4] == 0){
                        keyIO = 1;
                        printf("\n tempST: %d\n", tempST);
                        // break;
                    }
                }
                
            }

            if(keyIO == 0){
                ET = ET + details.queue[cQ][2];
            }
            else{
                ET = ET + details.process[0][lcAT][3];
                tempST = ET;
                tempET = tempST + details.process[0][lcAT][3];
                details.process[2][i][0] = details.process[0][lcAT][0]; // for printint pID
                details.process[2][i][2] = tempST; // for printing start time
                details.process[2][i][3] = tempET; // for printing end time
                printf("\nIO Start time: %d End time: %d", tempST, tempET);
            }
    
            details.process[1][i][0] = details.process[0][lcAT][0]; // for printing pID
            details.process[1][i][1] = details.queue[cQ][0]; // for printing qID
            details.process[1][i][2] = ST; // for printing start time
            details.process[1][i][3] = ET; // for printing end time

            if(keyIO == 0){
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.queue[cQ][2];
            }
            else{
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.process[0][lcAT][3];
            }


            int ncAT = details.process[0][lcAT][1]; // original arrival time of the current process
            // look for next arrival time and check if within process time
            // get highest arrival timee
            for(int j=0; j<nProcess; j++){
                if(flag[j][1] > flag[temp2][1]){ // changing arrival time of the current process
                    temp2 = j;
                }
            }
            // get lowest arrival time but higher than current
            for(int j=0; j<nProcess; j++){
                if(flag[j][1] < flag[temp2][1] && flag[j][1] > ncAT){
                    temp2 = j;
                }
            }
            int newcAT = flag[temp2][1] - 1;
            // if within the process, do nothing
            if(flag[temp2][1] >= ST && flag[temp2][1] <= ET && flag[temp2][1] <= newcAT){
                ;
            }
            else{
                // look for highest arrival time
                for(int j=0; j<nProcess; j++){
                    if(flag[j][1] >= flag[temp2][1] && flag[j][1] <= ET){
                        temp2 = j;
                    }
                }
            }
            // change arrival time for queue
            flag[lcAT][1] = flag[temp2][1] + 1;
            PRQ[lcAT][cQ] = DONE;
            // printf("\nRBT: %d i: %d nProcess: %d lcAT: %d, pID: %d", details.process[0][lcAT][2], i, nProcess, lcAT, details.process[0][lcAT][0]);

            /*
                MAYBE CHANGE INTO THIS ONE?
                have array PRQ[input[1]][input[0]] and initialize to 0 where
                [X][0] = q0
                [X][1] = q1
                if marked 1, use
                loop into all input[0] and look for same and use it and break afterwards mark it DONE(-1)
                if not same, loop into all but iterating priority by 1 until it gets a match and use it and break afterwards mark it DONE
                if priority boost happens, mark everything to 0 and use very first highest priority queue
                current CPU time (cCPUT) has a loop to iterate by 1 for every burst time and check for priority boost
                priority boost happens when ( cCPUT % PT == 0)
            */
            // int keyQ = 0;
            QP = details.queue[cQ][1];
            // look for equal priority queue
            for(int k=0; k<details.input[0]; k++){
                int temp = details.queue[k][1];
                if(temp == QP && PRQ[lcAT][k] > DONE){ 
                    PRQ[lcAT][k] = 1;
                    break;
                }
                
            }
            
            // look for next priority queue
            int temp1 = QP+1;
            for(int k=0; k<details.input[0]; k++){
                int temp = details.queue[k][1];
                
                if(temp == temp1 && PRQ[lcAT][k] > DONE){
                    PRQ[lcAT][k] = 1;
                    break;
                }
                // temp1++;
            }

            /* -------- DO THIS NEXT
                When PB = 1, put all PRQ[X][X] into highest priority and reset remaining PRQ to 0
                // look for highest queue priority
                for(int i=0; i<details.input[0]; i++){
                    int temp = details.queue[i][1];
                    if(temp < QP && temp > DONE){
                        QP = details.queue[i][1];
                        lQP = details.queue[i][0];
                        ilQP = i;
                    }
                }
                for(int i=0; i<nProcess; i++){
                    for(int j=0; j<details.input[0]; j++){
                        PRQ[i][j] = 0; // other queues marked to 0
                        PRQ[i][ilQP] = 1; // highest priority to be used
                    }
                }
                for(int row=0; row<details.input[1]; row++){
                    details.process[0][row][5] = ilQP;
                }
            */


            // // look for next queue
            // int keyQ = 0;
            // QP = details.queue[cQ][1];
            // // get lowest queue priority
            // for(int k=0; k<details.input[0]; k++){
            //     int temp = details.queue[k][1];
            //     if(temp > QP && temp > DONE){
            //         QP = details.queue[k][1];
            //         lQP = details.queue[k][0];
            //         // ilQP = k;
            //         printf("\nQP is :%d", QP);
            //     }
            //     // if same priority, go RR
            //     else if(temp == details.queue[cQ][1] && details.queue[k][0] != details.queue[cQ][0]){
            //         keyQ = 1;
            //         details.process[0][lcAT][5] = k;
            //     }
            // }
            // if(keyQ == 0){
            //     // get next highest queue priority but lower than last queue
            //     for(int k=0; k<details.input[0]; k++){
            //         int temp = details.queue[k][1];
            //         if(temp < QP && temp > DONE && temp > details.queue[cQ][1]){
            //             QP = details.queue[k][1];
            //             lQP = details.queue[k][0];
            //             details.process[0][lcAT][5] = k;
            //             printf("\nchanged cQ: %d", details.process[0][lcAT][5]);
            //         }
            //         else if(temp == QP){
                        
            //         }
            //     }
            // }
        }
        // print preemp not done but remaining burst time is <= queue QT
        else if(flag[lcAT][0] == 1 && details.process[0][lcAT][2] <= details.queue[cQ][2]){
            // iterate CPU Time
            for(int v=0; v<details.process[0][lcAT][2]; v++){
                cCPUT++;
                // printf("\nCPU: %d\n", cCPUT);
                if(cCPUT % PB == 0){
                    keyPB = 1;
                }
            }
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
                ET = ST;
            }
            else{
                ST = ET;
            }

            // check for IO
            keyIO = 0;
            int tempST = ST;
            int tempET = ET;
            if(details.process[0][lcAT][3] > 0){
                
                for(int v=0; v<details.process[0][lcAT][2]; v++){
                    tempST++;
                    // printf("\nCPU: %d\n", cCPUT);
                    if(tempST % details.process[0][lcAT][4] == 0){
                        keyIO = 1;
                        break;
                    }
                }
                
            }

            if(keyIO == 0){
                ET = ET + details.queue[cQ][2];
            }
            else{
                ET = ET + details.process[0][lcAT][3];
                tempST = ET;
                tempET = tempST + details.process[0][lcAT][3];
                details.process[2][i][0] = details.process[0][lcAT][0]; // for printint pID
                details.process[2][i][2] = tempST; // for printing start time
                details.process[2][i][3] = tempET; // for printing end time
                printf("\nIO Start time: %d End time: %d", tempST, tempET);
            }

            // ET = ET + details.process[0][lcAT][2];
            details.process[1][i][0] = details.process[0][lcAT][0]; // for printing pID
            details.process[1][i][1] = details.queue[cQ][0]; // for printing qID
            details.process[1][i][2] = ST; // for printing start time
            details.process[1][i][3] = ET; // for printing end time

            if(keyIO == 0){
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.queue[cQ][2];
            }
            else{
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.process[0][lcAT][3];
            }

            // if(details.process[0][lcAT][3] > 0){
            //     ST = ET + 1;
            //     ET = ST + details.process[0][lcAT][3];
            //     printf("\nIO Start time: %d End time: %d", ST, ET);
            // }
            TT = ET - flag[lcAT][1];
            WT = TT - details.process[0][lcAT][2];
            key = 1;
            
            // printf("\nRBT: %d lcAT: %d pID: %d", details.process[0][lcAT][2], lcAT, details.process[0][lcAT][0]);
        }
        else{
            printf("\nwaz wrong? RBT: %d i: %d nProcess: %d lcAT: %d", details.process[0][lcAT][2], i, nProcess, lcAT);
        }
        // print preemp done
        if(flag[lcAT][0] == 1 && key == 1){
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
            }
            else{
                ST = ET;
            }
            ET = ST + details.process[0][lcAT][2];
            printf("\nP[%d]", details.process[0][lcAT][0]);
            for(int k=0; k<100; k++){
                if(details.process[1][k][3] != 0 && details.process[1][k][0] == details.process[0][lcAT][0]){
                    printf("\nQ[%d] Start time: %d End time: %d", details.process[1][k][1], details.process[1][k][2], details.process[1][k][3 ]);
                }
            }
            // printf("\nQ[%d] Start time: %d End time: %d", details.queue[cQ][0], ST, ET);
            TT = ET - flag[lcAT][1];
            WT = TT - details.process[0][lcAT][2];
            printf("\nWaiting time: %d", WT);
            printf("\nTurnaround time: %d", TT);
            printf("\n************************************");
            flag[lcAT][1] = DONE;
            key = 0;
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

   printf("\nIO");
   for(int k=0; k<100; k++){
        if(details.process[2][k][3] != 0 && details.process[2][k][0] > EMPTY){
            printf("\nP[%d] Start time: %d End time: %d", details.process[2][k][0], details.process[2][k][2], details.process[2][k][3]);
        }
    }

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