#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxPatients 10000
#define maxLength 80
#define maxFileName 30
#define maxImages 10
#define personalNumberLength 11

struct patient{
    long personalNumber;
    char name[maxLength];
    int picRef[maxImages];
    int numberOfPicRefs; 
};
typedef struct patient Patient;

Patient createPatient(long personalNumber, char name[], int picRef[], int numberOfPicsRefs){
    Patient p;
    p.personalNumber = personalNumber;
    strcpy(p.name,name);
    p.numberOfPicRefs = numberOfPicsRefs;

    for(int i = 0; i < numberOfPicsRefs; i++){
        p.picRef[i] = picRef[i];
    }
    return p;
}

//Base functions
void databaseManagement(Patient patients[], int *nrOfPatients);
void registerPatient(Patient patients[], int *nrOfPatients);
void searchPatientsMenu(Patient patients[], int nrOfPatients);
void deletePatientsMenu(Patient patients[], int *nrOfPatients);
void sortPatientMenu(Patient patients[], int nrOfPatients);

//Print menus
void printMenu();
void printPatients(Patient patients[], int nrOfPatients);
void printSearchMenu();
void printSortMenu();
void printOneHitMenu();
void printDeleteMenu();

//Search functions
int getNrOfSearchHitsByPersonalNumber(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex);
int getNrOfSearchHitsByPatientName(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex);
int getNrOfSearchHitsByPicRef(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex);

//Sort functions
void sortByPersonalNumber(Patient patients[], int nrOfPatients);
void sortByPatientName(Patient patients[], int nrOfPatients);

//File functions
void saveFile(Patient patients[], int nrOfPatients);
void readFromFile(Patient patients[], int *nrOfPatients);

//Helping methods
void concatFileName(char nameOfFile[], char fileType[]);
int getLastIndex(char name[]);
void removeNewLineFromName(char name[]);
void updatePicRef(Patient patients[], int index);
void removePicRefFromPatient(Patient patients[], int index);

bool hasRemovedPatient(Patient patients[], int nrOfPatients, int index);
bool isPersonalNumberValid(long personalNumber);
bool doesPersonalNumberExist(Patient patients[], int nrOfPatients, long personalNumber);

int main(){
    int nrOfPatients = 0;
    Patient patients[maxPatients];
    readFromFile(patients, &nrOfPatients);
    databaseManagement(patients, &nrOfPatients);
    return 0;
}

void databaseManagement(Patient patients[], int *nrOfPatients){
    int choice;
    while(choice != 7){
        printMenu();
        scanf("%d", &choice);

        switch(choice){
            
            case 1:
                registerPatient(patients, nrOfPatients);
            break;

            case 2:
                printPatients(patients, *nrOfPatients);
            break;
            
            case 3:
                searchPatientsMenu(patients, *nrOfPatients);
            break;

            case 4:
                printf("Change\n");
            break;

            case 5:
                sortPatientMenu(patients, *nrOfPatients);
            break;

            case 6:
                //searchPatientsMenu(patients, *nrOfPatients);
                deletePatientsMenu(patients, nrOfPatients);
            break;

            case 7:
                saveFile(patients, *nrOfPatients);
                printf("Avslutar programmet! Bye!\n");
            break;

            default:
                printf("Välj ett alternativ från menyn!\n");
        }
    }
}

void printMenu(){
    printf("1. Registrera nya patienter\n");
    printf("2. Skriva ut alla patienter\n");
    printf("3. Söka efter patienter\n");
    printf("4. Ändra data för en patient\n");
    printf("5. Sortera patienter\n");
    printf("6. Avregistrera patient\n");
    printf("7. Avsluta programmet\n");
    printf(": ");
}

void printSearchMenu(){
    printf("\n");
    printf("1. Söka via personnummer\n");
    printf("2. Söka via patientens namn\n");
    printf("3. Söka via bildreferens\n");
    printf("4. Gå tillbaka till huvudmenyn\n");
    printf(": ");
}

void printSortMenu(){
    printf("\n");
    printf("1. Sortera via personnummer\n");
    printf("2. Sortera via patientens namn\n");
    printf("3. Gå tillbaka till huvudmenyn\n");
    printf(": ");
}

void printDeleteMenu(){
    printf("\n");
    printf("1. Ta bort bildreferenser\n");
    printf("2. Ta bort patienten\n");
    printf("3. Gå tillbaka till huvudmenyn\n");
    printf(": ");
}

void registerPatient(Patient patients[], int *nrOfPatients){
    printf("------------------\n");
    printf("Välkommen till registreringen!\n");
    char choice;
    while(choice != 'n'){
        long personalNumber;
        char name[maxLength];
        int picRef[maxImages];
        int numberOfPicRefs = 0; 

        printf("Personnummer: ");
        scanf("%ld", &personalNumber);
        if(!(doesPersonalNumberExist(patients, *nrOfPatients, personalNumber))){
            
            if(isPersonalNumberValid(personalNumber)){
                getchar();
                printf("\nPatientens namn: ");
                gets(name);
                printf("\nPatientens bildreferenser (Avsluta med -1): \n");
                while(numberOfPicRefs != maxImages){
                    scanf("%d", &picRef[numberOfPicRefs]);
                    if(picRef[numberOfPicRefs] == -1){
                        break;
                    }
                    numberOfPicRefs++;
                }

                patients[(*nrOfPatients)] = createPatient(personalNumber, name, picRef, numberOfPicRefs);
                (*nrOfPatients)++;
                printf("\nVill du fortsätta? (j/n):");
                scanf(" %c", &choice);
            }else{
                printf("\nDu har angivet ett ogiltigt personnummer. Skriv igen! \n");
            }
        }else{
            printf("\nDu har angivet ett existerande personnummer. Skriv igen!\n");
        }
    }
    printf("------------------\n");
}

void printPatients(Patient patients[], int nrOfPatients){
    printf("\n");
    printf("Personnummber\t\tNamn\t\tBildreferenser\n");
    printf("------------------------------------------------------\n");
    for(int i = 0; i < nrOfPatients; i++){
        int theFirstDigitOfPersonalNumber = patients[i].personalNumber / 10000;
        int lastFourDigitOfPersonalNumber = patients[i].personalNumber % 10000;
        printf("|%d-%d|\t", theFirstDigitOfPersonalNumber, lastFourDigitOfPersonalNumber);
        printf("|%s|\t", patients[i].name);
        printf("[");
        for(int j = 0; j < patients[i].numberOfPicRefs; j++){
            if(j < patients[i].numberOfPicRefs-1){
                printf("%d, ", patients[i].picRef[j]);
            }else{
                printf("%d", patients[i].picRef[j]);
            }
        }
        printf("] \n");
    }
    printf("\n");
}


void searchPatientsMenu(Patient patients[], int nrOfPatients){
    int choice = 0, nrOfSearchHits = 0, patientIndex = 0;
    Patient p;
    Patient tmpArr[maxPatients];
    
    while(choice != 4){
        memset(tmpArr, 0, sizeof(tmpArr)); //for clearing the array
        printSearchMenu();
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                nrOfSearchHits = getNrOfSearchHitsByPersonalNumber(patients, nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;

            case 2:
                nrOfSearchHits = getNrOfSearchHitsByPatientName(patients, nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;
            
            case 3:
                nrOfSearchHits = getNrOfSearchHitsByPicRef(patients, nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;

            case 4:
                printf("\nÅtergår till huvudmenyn \n\n");
            break;

            default:
                printf("Välj ett alternativ från menyn!\n");
        }
        if(nrOfSearchHits == 1 && choice != 4){
            updatePicRef(patients, patientIndex);
        }
    }
}

void deletePatientsMenu(Patient patients[], int *nrOfPatients){
    int choice = 0, nrOfSearchHits = 0, patientIndex = 0;
    Patient p;
    Patient tmpArr[maxPatients];
    
    while(choice != 4){
        memset(tmpArr, 0, sizeof(tmpArr)); //for clearing the array
        printSearchMenu();
        scanf("%d", &choice);

        switch(choice){
            case 1:
                nrOfSearchHits = getNrOfSearchHitsByPersonalNumber(patients, *nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;

            case 2:
                nrOfSearchHits = getNrOfSearchHitsByPatientName(patients, *nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;
            
            case 3:
                nrOfSearchHits = getNrOfSearchHitsByPicRef(patients, *nrOfPatients, tmpArr, &patientIndex);
                printPatients(tmpArr, nrOfSearchHits);
            break;

            case 4:
                printf("\nÅtergår till huvudmenyn \n\n");
            break;

            default:
                printf("Välj ett alternativ från menyn!\n");
        }
        if(nrOfSearchHits == 1 && choice != 4){
            printDeleteMenu();
            int tmp = 0;
            scanf("%d", &tmp);

            switch(tmp){
                case 1:
                    removePicRefFromPatient(patients, patientIndex);
                break;

                case 2:
                    if(hasRemovedPatient(patients, *nrOfPatients, patientIndex)){
                        (*nrOfPatients)--;
                    }
                break;

                case 3:
                    choice = 4;
                break;
            }
        }
    }
}

void sortPatientMenu(Patient patients[], int nrOfPatients){
    int choice = 0;
    printSortMenu();
    scanf("%d", &choice);

    switch(choice){
        case 1:
            sortByPersonalNumber(patients, nrOfPatients);
        break;

        case 2:
            sortByPatientName(patients, nrOfPatients);
        break;

        case 3:
            printf("\nÅtergår till huvudmenyn \n\n");
        break;

        default:
            printf("Välj ett alternativ från menyn!\n");
    }
    
}

void sortByPersonalNumber(Patient patients[], int nrOfPatients){
    Patient tmp;
    for(int i = 0; i < nrOfPatients; i++){
        for(int j = i + 1; j < nrOfPatients; j++){
            if((patients[i].personalNumber) < (patients[j].personalNumber)){
                tmp = patients[i];
                patients[i] = patients[j];
                patients[j] = tmp;
            }
        }
    }
}

void sortByPatientName(Patient patients[], int nrOfPatients){
    Patient tmp;
    for(int i = 0; i < nrOfPatients; i++){
        for(int j = i+1; j < nrOfPatients; j++){
            if(strcmp(patients[i].name,patients[j].name)>0){
                tmp = patients[i];
                patients[i] = patients[j];
                patients[j] = tmp;
            }
        }
    }
}

int getNrOfSearchHitsByPersonalNumber(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex){
    long personalNumber;
    int nrOfSearchHits = 0;
    char a[personalNumberLength], b[personalNumberLength];
    Patient p;
    printf("Skriv in patientens personnummer:");
    scanf("%ld", &personalNumber);
    sprintf(a, "%ld", personalNumber); //for converting from long to string
    
    for(int i = 0; i < nrOfPatients; i++){
        sprintf(b, "%ld", patients[i].personalNumber);
        if(strstr(b,a) > 0){
            p = patients[i];
            tmpArr[nrOfSearchHits] = p;
            (nrOfSearchHits)++;
            (*patientIndex) = i;
        }
    }
    return nrOfSearchHits;
}

int getNrOfSearchHitsByPatientName(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex){
    char name[maxLength];
    int nrOfSearchHits = 0;
    Patient p;
    printf("Skriv in patientens namn:");
    scanf("%s", name);
    for(int i = 0; i < nrOfPatients; i++){
        if(strcasestr(patients[i].name, name)){
            p = patients[i];
            tmpArr[(nrOfSearchHits)] = p;
            (nrOfSearchHits)++;
            (*patientIndex) = i;
        }
    }
    return nrOfSearchHits;
}

int getNrOfSearchHitsByPicRef(Patient patients[], int nrOfPatients, Patient tmpArr[], int *patientIndex){
    int nrOfSearchHits = 0, tmpVal;
    Patient p;
    printf("Skriv in en bildreferens:");
    scanf("%d", &tmpVal);

    for(int i = 0; i < nrOfPatients; i++){
        for(int j = 0; j < patients[i].numberOfPicRefs; j++){
            if(tmpVal == patients[i].picRef[j]){
                p = patients[i];
                tmpArr[(nrOfSearchHits)] = p;
                (nrOfSearchHits)++;
                (*patientIndex) = i;
            }
        }
    }
    return nrOfSearchHits;
}

void updatePicRef(Patient patients[], int index){
    printf("\nPatientens bildreferenser (Avsluta med -1): \n");
    while(patients[index].numberOfPicRefs != maxImages){
        scanf("%d", &patients[index].picRef[patients[index].numberOfPicRefs]);
            if(patients[index].picRef[patients[index].numberOfPicRefs] == -1){
                break;
            }
        patients[index].numberOfPicRefs++;
    }
}

void removePicRefFromPatient(Patient patients[], int index){
    printf("\nVilka bildreferenser vill du ta bort? (Avsluta med -1)\n");
    int val = 0;
    int foundIndex = 0;
    bool canRemoveElement = false;
    while(patients[index].numberOfPicRefs > 0){
        scanf("%d", &val);

        if(val != -1){
            for(int i = 0; i < patients[index].numberOfPicRefs; i++){
                if(val == patients[index].picRef[i]){
                    foundIndex = i;
                    canRemoveElement = true;
                }
            }

            if(canRemoveElement){
                printf("Lyckades ta bort elementet: %d\n", patients[index].picRef[foundIndex]);
                for(int i = foundIndex; i < patients[index].numberOfPicRefs; i++){
                    patients[index].picRef[i] = patients[index].picRef[i + 1];
                }
                patients[index].numberOfPicRefs--;
                canRemoveElement = false;
            }else{
                printf("Element existerar ej! Skriv igen\n");
            }
        }else{
            break;
        } 
    }    
    printf("\nÅtergår till sökmenyn\n");
}

bool hasRemovedPatient(Patient patients[], int nrOfPatients, int index){
    char choice;
    Patient tmp;
    getchar();
    printf("\nÄr du säker på att du vill ta bort denna patient? (j/n): ");
    scanf(" %c", &choice);
    choice = tolower(choice);
    switch(choice){

        case 'j':
            
            for(int i = index; i < nrOfPatients; i++){
                tmp = patients[i];
                patients[i] = patients[i + 1];
                patients[i + 1] = tmp;
            }
            return true;
        break;

        case 'n':
            printf("\nÅtergår till sökmenyn\n");
        break;
    }
    return false;
}

int getLastIndex(char name[]){
    int counter = 0;

    while(name[counter] != '\n'){
        counter++;
    }
    return counter;
}

void removeNewLineFromName(char name[]){
    int lastIndex = getLastIndex(name);
    name[lastIndex] = '\0';
}

void concatFileName(char nameOfFile[], char fileType[]){
    bool isInConcatFileName = true;
    while(isInConcatFileName){
        printf("Namnet på filen: ");
        scanf("%s", nameOfFile);
        getchar();
        printf("Typen på filen: ");
        scanf("%s", fileType);
        if(strstr(fileType, ".") != NULL){
            strcat(nameOfFile, fileType);
            isInConcatFileName = false;
        }else{
            printf("You forgot to add '.' in filetype!\n");
        }
    }
}

bool isPersonalNumberValid(long personalNumber){
    int sum = 0, addedDigit = 0, counter = 1, oldAddedDigitValue = 0, controlDigit = 0;
    int lastDigitOfPNumber = personalNumber % 10;

    for(int i = 0; i < personalNumberLength-1; i++){
        if(i % 2 == 0){
            sum += (personalNumber/10) % 10 * 2;
        }else{
            sum +=  (personalNumber/10) % 10 * 1;
        }
        personalNumber /= 10;
        addedDigit += sum % 10;
        sum /= 10;
    }

    oldAddedDigitValue = addedDigit;

    while(addedDigit % 10 != 0){
        addedDigit += counter;
        counter++;
    }

    controlDigit = (addedDigit - oldAddedDigitValue);

    if(lastDigitOfPNumber == controlDigit){
        return true;
    }
    return false;
}

bool doesPersonalNumberExist(Patient patients[], int nrOfPatients, long personalNumber){
    
    for(int i = 0; i < nrOfPatients; i++){
        if(patients[i].personalNumber == personalNumber){
            return true;
        }
    }
    return false;
}

void saveFile(Patient patients[], int nrOfPatients){
    FILE *fp;
    char nameOfFile[maxFileName], fileType[maxFileName];
    concatFileName(nameOfFile, fileType);
    fp = fopen(nameOfFile,"w");
    if(fp != NULL){
        fprintf(fp, "%d\n", nrOfPatients);
        for(int i = 0; i < nrOfPatients; i++){
            fprintf(fp, "%ld\n", patients[i].personalNumber);

            fprintf(fp, "%s", patients[i].name);
            fprintf(fp, "\n");
            fprintf(fp, "%d\n", patients[i].numberOfPicRefs);
            
            for(int j = 0; j < patients[i].numberOfPicRefs; j++){
                fprintf(fp, "%d ", patients[i].picRef[j]);
            }
        }
        fclose(fp);
    }else{
        printf("Kunde inte öppna filen!\n");
    }
}

void readFromFile(Patient patients[], int *nrOfPatients){
    FILE *fp;
    long personalNumber;
    char name[maxLength];
    int picRef[maxImages];
    int numberOfPicRefs; 
    char choice;
    char nameOfFile[maxFileName], fileType[maxFileName];

    printf("Välkommen till patientregister programmet!\n");
    printf("Har du en fil redan? (j/n): ");
    scanf(" %c", &choice);
    printf("\n");
    while(choice != 'n'){
        concatFileName(nameOfFile, fileType);
        fp = fopen(nameOfFile,"r");
        
        if(fp != NULL){
            fscanf(fp, "%d\n", nrOfPatients);

            for(int i = 0; i < (*nrOfPatients); i++){
                fscanf(fp, "%ld\n", &personalNumber);
                fgets(name, sizeof(name) ,fp);
                
                removeNewLineFromName(name);

                fscanf(fp, "%d\n", &numberOfPicRefs);

                for(int j = 0; j < numberOfPicRefs; j++){
                    fscanf(fp, "%d\n", &picRef[j]);
                }
                patients[i] = createPatient(personalNumber, name, picRef, numberOfPicRefs);
            }
            break;
        }else{
            printf("\nKunde inte öppna filen eller så existerar den ej!\n");
            printf("Försök igen \n");
            printf("\n");
            printf("Har du en fil redan? (j/n): ");
            scanf(" %c", &choice);
        } 
    }
    fclose(fp);
}