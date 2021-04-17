/***************************************************************
Names:  Cuasi, Beaverly
        Tulabot, Victor
Group:   48
Section:  S16
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char String20[21];

typedef struct{
    int input[3];
    int process[100][3]; // dapat yung # of rows = details.input[1]
}Feature;

void GetInputs(FILE *fp, Feature *details){
    for(int i=0; i<3; i++){
        fscanf(fp, "%d", &details->input[i]);
        // input[0] = X
        // input[1] = Y
        // input[2] = Z
    }

    for(int row=0; row<details->input[1]; row++){
        for(int col=0; col<3; col++){
            fscanf(fp, "%d", &details->process[row][col]);
            // printf("%d ", details->process[row][col]);
        }
        // printf("\n");
    }
}

void RR(Feature details){
    /*
        1.) Iterate per process
        2.) Check Arrival Time
        3.) Check if cBT > QT
        4.) if cBT > QT, cBT - QT then next process. Flag cProcess
            Suggestion: cBT / QT = # of switches
                        put cPiD into array for queuing
        5.) if cBT <= QT, finish cProcess
    */

    int WT = 0, // Waiting Time
        TT = 0, // Turnaround Time
        ST = 0, // Start Time
        ET = 0, // End Time
        temp = 0, temp2 = 0,
        lET = 0, // Last Ending Time of finished process
        nProcess = details.input[1], // # of processes
        AT = 0, // Arrival Time
        QT = details.input[2]; // Quantum Time
    float AWT = 0.0; // Average Waiting Time
    int flag[details.input[1]][details.input[1]],
        data[50][4],
        BT[details.input[1]];

    for(int i=0; i<details.input[1]; i++){
        flag[i][0] = 0; // flag[i][0] = flag 
        flag[i][1] = details.process[i][1]; // flag[i][1] = AT
        BT[i] = details.process[i][2]; // burst time
    }
    for(int i=0; i<50; i++){
        for(int j=0; j<4; j++){
            data[i][j] = 0;
        }
    }

    for(int i=0; i<nProcess; i++){
        temp = i;
        // temp2 = temp;
        // for(int p=0; p<details.input[1]; p++){
        //     printf("\npID: %d  AT: %d BT: %d is flagged: %d", details.process[p][0], details.process[p][1], details.process[p][2], flag[p]);
        // }
        for (int j=0; j<details.input[1]; j++){
            // printf("\ntemp %d vs nProcess %d\n",temp,nProcess);
            // check if temp >= nProcess
            if(temp >= nProcess-1){
                if(details.process[j][1] >= 0){
                    temp = j;
                }
                // printf("\ntemp is: %d", temp);
            }
            

            // check arrival time
            if(details.process[j][1] < details.process[temp][1] && details.process[j][1] >= 0){
                temp = j;
                // printf("\ntemp is: %d AT", temp);
            }
        }
        
        // check burst time with quantum time
        if(details.process[temp][2] > QT){
            temp2 = temp;
            flag[temp][0] = 1;
            nProcess++;
            int newTime = details.process[temp][2] - QT;
            ST = ET;
            ET = ET + QT;
            data[i][0] = details.process[temp][0];
            data[i][1] = ST;
            data[i][2] = ET;
            data[i][3] = QT;
            details.process[temp][2] = newTime;
            //  for(int p=0; p<nProcess; p++){
            //     printf("\npID: %d  AT: %d ", details.process[p][0], details.process[p][1]);
            // }
            /*
                look for next arrival time
                check if within process time
            */
           int cAT = flag[temp][1];
           for(int p=0; p<nProcess; p++){
               if(details.process[p][1] > details.process[temp2][1]){
                   temp2 = p;
               }
           }
           for(int p=0; p<nProcess; p++){
               if(details.process[p][1] < details.process[temp2][1] && details.process[p][1] > cAT){ // check
                   temp2 = p;
               }
           }
           int newcAT = details.process[temp2][1] - 1;
        //    printf("\nbefore: %d >= nice newcAT: %d",details.process[temp2][1], newcAT);
           if(details.process[temp2][1] >= ST && details.process[temp2][1] <= ET && details.process[temp2][1] <= newcAT){
               printf("\nnice pID: %d", details.process[temp2][0]);
           }
           else{
               // look for highest arrival time
                for(int l=0; l<nProcess; l++){
                    // printf("\n%d vs %d",details.process[l][1],details.process[temp2][1]);
                    if(details.process[l][1] >= details.process[temp2][1] && details.process[l][1] <= ET){
                        // printf("\n%d > %d",details.process[l][1],details.process[temp2][1]);
                        temp2 = l;
                    }
                }
           }
            
            // printf("\nMax AT: %d", details.process[temp2][1]);

            // change arrival time for queue
            
            details.process[temp][1] = details.process[temp2][1] + 1;
            // if(details.process[temp][2] <= 4){
            //     // flag[temp] = 0;
            //     nProcess--;
            // }
            // printf("\nNew Arrival Time: %d for pID %d", details.process[temp][1], details.process[temp][0]);
            // printf("\nRemaining BT: %d",details.process[temp][2]);
        }
        else{
            if(flag[temp][0] == 1 && details.process[temp][2] <= QT && details.process[temp][2] > 0){
                nProcess++;
                // printf("\nlast iteration for pID %d", details.process[temp][0]);
                int newTime = details.process[temp][2] - details.process[temp][2];
                ST = ET;
                ET = ET + details.process[temp][2];
                data[i][0] = details.process[temp][0];
                data[i][1] = ST;
                data[i][2] = ET;
                data[i][3] = details.process[temp][2];
                details.process[temp][2] = newTime;
                // printf("\nRemaining BT: %d",details.process[temp][2]);
            }
        }
        // printf("\nflag temp %d", flag[temp]);
        if(flag[temp][0] == 0 && details.process[temp][2] > 0){
            ST = ET;
            ET = ET + details.process[temp][2];
            
            lET = ET;
            TT = ET - details.process[temp][1];
            WT = TT - BT[temp]; 
            AWT = AWT + WT;
            printf("\nP[%d]", details.process[temp][0]);
            printf("\nStart Time: %d  End Time: %d ", ST, ET);
            printf("\nWaiting Time: %d", WT);
            printf("\nTurnaround Time: %d", TT);
            printf("\n************************************");
            details.process[temp][1] = -1;
        }
        else if (flag[temp][0] == 1 && details.process[temp][2] == 0){ // == 0
            // printf("\nflagged %d",details.process[temp][0]);
            int cBT = 0;
            printf("\nP[%d]", details.process[temp][0]);
            for(int i=0; i<50; i++){
                if(data[i][2] != 0 && data[i][0] == details.process[temp][0]){
                    printf("\nStart Time: %d  End Time: %d ", data[i][1], data[i][2]);
                    cBT += data[i][3];
                    ST = data[i][1];
                    ET = data[i][2];
                    // printf("\ni: %d of data[%d] = Start Time: %d  End Time: %d ",i , data[i][0], data[i][1], data[i][2]);

                }
            }
            // ST = ET; //remove
            ET = ET + details.process[temp][2];
            
            lET = ET;
            TT = ET - flag[temp][1];
            WT = TT - BT[temp];
            AWT = AWT + WT;
            // printf("\nStart Time: %d  End Time: %d ", ST, ET); //remove
            printf("\nWaiting Time: %d", WT);
            printf("\nTurnaround Time: %d", TT);
            printf("\n************************************");
            details.process[temp][1] = -1;
            // for(int i=0; i<50; i++){
            //     printf("\ni: %d of data[%d] = Start Time: %d  End Time: %d ",i , data[i][0], data[i][1], data[i][2]);
            // }
        }
    }

    AWT = AWT / details.input[1];
    printf("\nAverage Waiting Time: %.2f", AWT);
}

int main(){
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

    // while( (X != 0 || X != 1 || X != 2 || X != 3) && (Y <= 3 || Y >= 100) ){
    //     printf("Invalid input, please try again: ");
    //     printf("\nInput values X Y Z: ");
    //     scanf("%d %d %d", &X, &Y, &Z);
    // }

    RR(details);
    
    return 0;
}