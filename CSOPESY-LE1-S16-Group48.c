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
    int process[3][100][7]; 
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

    int flag[nProcess][2];
    int PRQ[details.input[1]][details.input[0]];

    int counter = 0;
    
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
        details.process[0][row][6] = 0;
    }
    int key = 0;
    int keyPB = 0;
    int keyIO = 0;
    for(int i=0; i<nProcess; i++){
        // check arrival time
        int lcAT = i; // 
        if(flag[lcAT][1] <= DONE || lcAT >= details.input[1]){
            lcAT = 0;
            while(flag[lcAT][1] <= DONE || lcAT >= details.input[1]){
                // lcAT = (int)rand() % (int)details.input[1];
                lcAT++;
            }
            int cAT = flag[lcAT][1]; // arrival time
            for(int j=0; j<details.input[1]; j++){
                int temp = flag[j][1]; // changing arrival time
                if(temp < cAT && temp > DONE){
                    cAT = temp;
                    lcAT = j;
                }
            }
        }
        else{
            int cAT = details.process[0][lcAT][1]; // original arrival time
            for(int j=0; j<details.input[1]; j++){
                int temp = flag[j][1]; // changing arrival time
                if(temp < cAT && temp > DONE){
                    cAT = temp;
                    lcAT = j;
                }
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
            AWT = AWT + WT;
        }
        // preemp not done
        else if(details.process[0][lcAT][2] > details.queue[cQ][2] && flag[lcAT][1] > DONE){
            int temp2 = lcAT;
            flag[lcAT][0] = 1;
            nProcess++;

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
                        // break;
                    }
                }
                
            }

            if(keyIO == 0){
                ET = ET + details.queue[cQ][2];
                // iterate CPU Time
                for(int v=0; v<details.queue[cQ][2]; v++){
                    cCPUT++;
                    if(cCPUT % PB == 0){
                        keyPB = 1;
                    }
                }
            }
            else {
                for(int v=0; v<details.process[0][lcAT][3]; v++){
                    cCPUT++;
                    if(cCPUT % PB == 0){
                        keyPB = 1;
                    }
                }
                ET = ET + details.process[0][lcAT][4];
                tempST = ET;
                tempET = tempST + details.process[0][lcAT][3];
                details.process[2][i][0] = details.process[0][lcAT][0]; // for printint pID
                details.process[2][i][2] = tempST; // for printing start time
                details.process[2][i][3] = tempET; // for printing end time
            }
    
            details.process[1][i][0] = details.process[0][lcAT][0]; // for printing pID
            details.process[1][i][1] = details.queue[cQ][0]; // for printing qID
            details.process[1][i][2] = ST; // for printing start time
            details.process[1][i][3] = ET; // for printing end time

            if(keyIO == 0){
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.queue[cQ][2];
            }
            else if(details.process[0][lcAT][6] == 0){
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.process[0][lcAT][3];
                details.process[0][lcAT][6] = details.process[0][lcAT][6] + details.process[0][lcAT][3];

                // dito mo icheck kung may kasunod pa or wala
                int holder = lcAT;
                // look for the process in the same queue
                for(int j=0; j<details.input[1]; j++){
                    if(PRQ[j][cQ] == 1 && j != lcAT && flag[j][1] > flag[holder][1]){
                        holder = j;
                    }
                }

                // kung may kasunod, gawin mo to tapos process mo
                if(holder != lcAT){
                    // change arrival time in the same queue
                    flag[lcAT][1] = flag[holder][1] + 1; // tama na to
                    // move remaining arrival time +1
                    for(int j=0; j<details.input[1]; j++){
                        if(j != lcAT && j != holder){
                            flag[j][1]++;
                        }
                    }
                }
                
                // kung walang kasunod, gawin mo to
                else if(holder == lcAT){
                    // look for next arrival time at same prio queue
                    // check arrival time
                    int aAT = 0;
                    int templcAT = lcAT; 
                    for(int j=0; j<details.input[1]; j++){
                        if(flag[j][1] > aAT){
                            aAT = flag[j][1];
                        }
                    }
                    int tempcAT = aAT; // arrival time
                    for(int j=0; j<details.input[1]; j++){
                        int atemp = flag[j][1]; // changing arrival time
                        if(atemp < tempcAT && atemp > DONE && j != lcAT){
                            tempcAT = atemp;
                            templcAT = j;
                        }
                    }
                    int tempcQ = cQ;
                    // look for current prio queue
                    // look for the marked 1 in PRQ[lcAT][X]
                    for(int k=0; k<details.input[0]; k++){
                        if(PRQ[templcAT][k] == 1){
                            tempcQ = k;
                        }
                    }

                    // execute process = IO burst
                    // iterate CPU time as you process
                    for(int v=0; v<details.process[0][lcAT][3]; v++){
                        cCPUT++;
                        if(cCPUT % PB == 0){
                            keyPB = 1;
                        }
                    }
                    nProcess++;
                    i++;
                    details.process[1][i][0] = details.process[0][templcAT][0]; // for printing pID
                    details.process[1][i][1] = details.queue[tempcQ][0]; // for printing qID
                    details.process[1][i][2] = tempST; // for printing start time
                    details.process[1][i][3] = tempET; // for printing end time
                    details.process[0][templcAT][2] = details.process[0][templcAT][2] - details.process[0][lcAT][3];
                    
                    // change arrival time of executed process to the last of same prio queue
                    int tempncAT = details.process[0][templcAT][1]; // original arrival time of the current process
                    int temp3 = templcAT;
                    // look for next arrival time and check if within process time
                    // get highest arrival timee
                    for(int j=0; j<nProcess; j++){
                        if(flag[j][1] > flag[temp3][1]){ // changing arrival time of the current process
                            temp3 = j;
                        }
                    }
                    // get lowest arrival time but higher than current
                    for(int j=0; j<nProcess; j++){
                        if(flag[j][1] < flag[temp3][1] && flag[j][1] > tempncAT && flag[j][1] > DONE){ // check this must be > DONE
                            temp3 = j;
                        }
                    }
                    int tempnewcAT = flag[temp3][1] - 1;
                    // if within the process, do nothing
                    if(flag[temp3][1] >= ST && flag[temp3][1] <= ET && flag[temp3][1] <= tempnewcAT){ // check ST and ET
                        ;
                    }
                    else{
                        // look for highest arrival time
                        for(int j=0; j<nProcess; j++){
                            if(flag[j][1] >= flag[temp3][1] && flag[j][1] <= ET){ // check ET
                                temp3 = j;
                            }
                        }
                    }
                    // change arrival time for queue
                    flag[templcAT][1] = flag[temp3][1] + 1;
                    ST = tempST;
                    ET = tempET;
                }
                
            }
            else{
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.process[0][lcAT][3];
                details.process[0][lcAT][6] = details.process[0][lcAT][6] + details.process[0][lcAT][3];
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
                if(flag[j][1] < flag[temp2][1] && flag[j][1] > ncAT && flag[j][1] > DONE){ // check this must be > DONE
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
            // change arrival time for queue                                        check
            if (keyIO == 1 && details.process[0][lcAT][6] < details.queue[cQ][2] && details.process[0][lcAT][6] > 0){
                // counter < queue burst
                nProcess++;
                i++;
            }
            else{
                flag[lcAT][1] = flag[temp2][1] + 1;
                PRQ[lcAT][cQ] = DONE;
                details.process[0][lcAT][6] = 0;
            //}

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
                int nextcQ = ilQP+1;
                int keyDONE = 0;
                // look for equal priority queue
                for(int k=0; k<details.input[0]; k++){
                    int temp = details.queue[k][1];
                    if(temp == QP && PRQ[lcAT][k] > DONE){ 
                        PRQ[lcAT][k] = 1;
                        nextcQ = k;
                        break;
                    }
                    else if(PRQ[lcAT][k] == DONE){
                        keyDONE = 1;
                    }
                    
                }
                
                // look for next priority queue
                int temp1 = QP+1;
                for(int k=0; k<details.input[0]; k++){
                    int temp = details.queue[k][1];
                    
                    if(temp == temp1 && PRQ[lcAT][k] > DONE){
                        PRQ[lcAT][k] = 1;
                        nextcQ = k;
                        keyDONE = 0;
                        break;
                    }
                    else if(PRQ[lcAT][k] == DONE){
                        keyDONE = 1;
                    }
                    // temp1++;
                }

                // if(keyDONE == 1){
                //     for(int i=0; i<details.input[1]; i++){
                //         for(int j=0; j<details.input[0]; j++){
                //             if(PRQ[i][j] <= DONE){
                //                 ;
                //             }
                //             else{
                //                 PRQ[i][nextcQ] = 1; // highest priority to be used
                //             }
                //         }
                //     }
                //     for(int row=0; row<details.input[1]; row++){
                //         details.process[0][row][5] = nextcQ;
                //     }
                //     keyDONE = 0;
                // }

                /* -------- DO THIS NEXT
                    When PB = 1, put all PRQ[X][X] into highest priority and reset remaining PRQ to 0
                */
                
            // look for highest queue priority
            if(keyPB == 1){
                    for(int k=0; k<details.input[1]; k++){
                        for(int l=0; l<details.input[0]; l++){
                                PRQ[k][l] = 0; // other queues marked to 0
                                PRQ[k][ilQP] = 1; // highest priority to be used
                        }
                    }
                    for(int row=0; row<details.input[1]; row++){
                        details.process[0][row][5] = ilQP;
                    }
                    keyPB = 0;
            }
           }
        }
        // print preemp not done but remaining burst time is <= queue QT
        else if(flag[lcAT][0] == 1 && details.process[0][lcAT][2] <= details.queue[cQ][2] && flag[lcAT][1] > DONE){
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
                    if(tempST % details.process[0][lcAT][4] == 0){
                        keyIO = 1;
                        // break;
                    }
                }
                
            }

            if(keyIO == 0){
                ET = ET + details.process[0][lcAT][2];
                for(int v=0; v<details.process[0][lcAT][2]; v++){
                    cCPUT++;
                    if(cCPUT % PB == 0){
                        keyPB = 1;
                    }
                }
            }
            else{
                for(int v=0; v<details.process[0][lcAT][3]; v++){
                    cCPUT++;
                    if(cCPUT % PB == 0){
                        keyPB = 1;
                    }
                }
                ET = ET + details.process[0][lcAT][4];
                tempST = ET;
                tempET = tempST + details.process[0][lcAT][3];
                details.process[2][i][0] = details.process[0][lcAT][0]; // for printint pID
                details.process[2][i][2] = tempST; // for printing start time
                details.process[2][i][3] = tempET; // for printing end time
            }

            // ET = ET + details.process[0][lcAT][2];
            details.process[1][i][0] = details.process[0][lcAT][0]; // for printing pID
            details.process[1][i][1] = details.queue[cQ][0]; // for printing qID
            details.process[1][i][2] = ST; // for printing start time
            details.process[1][i][3] = ET; // for printing end time                                              // dito yung 18
            if(keyIO == 0){
                details.process[0][lcAT][2] = details.process[0][lcAT][2] - details.process[0][lcAT][2];
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
            // look for highest queue priority
            if(keyPB == 1){
                    for(int k=0; k<details.input[1]; k++){
                        for(int l=0; l<details.input[0]; l++){
                                PRQ[k][l] = 0; // other queues marked to 0
                                PRQ[k][ilQP] = 1; // highest priority to be used
                        }
                    }
                    for(int row=0; row<details.input[1]; row++){
                        details.process[0][row][5] = ilQP;
                    }
                    keyPB = 0;
            }
            // AWT = AWT + WT;
        }
        // print preemp done
        if(flag[lcAT][0] == 1 && key == 1){
            if(ST < details.process[0][lcAT][1]){
                ST = details.process[0][lcAT][1];
            }
            else{
                ST = ET;
            }
            // ET = ST + details.process[0][lcAT][2];
            printf("\nP[%d]", details.process[0][lcAT][0]);
            for(int k=0; k<100; k++){
                if(details.process[1][k][3] != 0 && details.process[1][k][0] == details.process[0][lcAT][0]){
                    printf("\nQ[%d] Start time: %d End time: %d", details.process[1][k][1], details.process[1][k][2], details.process[1][k][3 ]);
                }
            }
            TT = ET - flag[lcAT][1];
            WT = TT - details.process[0][lcAT][2];
            printf("\nWaiting time: %d", WT);
            printf("\nTurnaround time: %d", TT);
            printf("\n************************************");
            flag[lcAT][1] = DONE;
            key = 0;
            AWT = AWT + WT;
        }
    }

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

    if(details.input[0] < 2 || details.input[0] > 5){
        printf("Input invalid");
        exit(1);
    }

    if(details.input[1] < 3 || details.input[1] > 100){
        printf("Input invalid");
        exit(1);
    }

    MLFQ(details);
    
    return 0;
}