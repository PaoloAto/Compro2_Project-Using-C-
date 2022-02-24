#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CART 100

typedef char string15[16];
typedef char string20[21];
typedef char string50[51];
typedef char string8[9];

typedef struct {
    string20 first,
	         middle,
			 last;
} nameType;

typedef struct {
    nameType name;
	string50 address;
} userInfoType;

typedef struct {
    string8 code;
	int     quantity;
} prodBoughtType;
typedef prodBoughtType arrBought[MAX_CART];


typedef struct{
    string15     username,
	             password;
	userInfoType info;
	char         type;
	float        creditLimit,
	             outstanding;
	arrBought    cart;
	int          nItems;	
}dataType;

struct userTag {
    dataType        sData;
	struct userTag *pNext;
};
typedef struct userTag userType;
typedef userType * ptrUser;

void
displayAll(ptrUser pUsers)
{
	while (pUsers != NULL) {
	  printf("%s\n", pUsers->sData.username);
	  pUsers = pUsers->pNext;
	}
}

void
freeAll(ptrUser *pFirst)
{   ptrUser pDel;

	while (*pFirst != NULL) {
        pDel = *pFirst;
		*pFirst = (*pFirst)->pNext;
    	free(pDel);
	}
}

void
getUserInfo(userInfoType *pInfo)
{
	printf("Enter name\n");
	printf("First name: ");
	fgets((*pInfo).name.first, 21, stdin);
	(*pInfo).name.first[strlen((*pInfo).name.first)-1] = '\0';
	printf("Middle name: ");
	fgets(pInfo->name.middle, 21, stdin);
	pInfo->name.middle[strlen(pInfo->name.middle)-1] = '\0';
	printf("Last name: ");
	fgets(pInfo->name.last, 21, stdin);
	pInfo->name.last[strlen(pInfo->name.last)-1] = '\0';
	
	printf("Enter address: ");
	fgets(pInfo->address, 21, stdin);
	pInfo->address[strlen(pInfo->address)-1] = '\0';	
}

void 
signUp(userType *pUser)
{
	char cDump;
	
	do {
       printf("Enter username : ");
       scanf("%s%c", (*pUser).sData.username, &cDump);	   
	} while (strlen((*pUser).sData.username) < 3 ||
	         strlen((*pUser).sData.username) > 15);

	do {
       printf("Enter password : ");
       scanf("%s%c", (*pUser).sData.password, &cDump);	   
	} while (strlen((*pUser).sData.password) < 6 ||
	         strlen((*pUser).sData.password) > 15);
			 
	getUserInfo(&(*pUser).sData.info);
	
    //get input for type of user and initialize as necessary	
}

void
displayAllRecur(ptrUser pUsers)
{
	if (pUsers != NULL){
	    printf("%s\n", pUsers->sData.username);
		displayAllRecur(pUsers->pNext);
	}
}

ptrUser
search(ptrUser pFirst, string15 username)
{   ptrUser pRun;

    pRun = pFirst;
	
	while(pRun != NULL &&
	      strcmp(pRun->sData.username, username) != 0)
	   pRun = pRun->pNext;
	 
	return pRun;
}

void 
deleteNode(ptrUser *pFirst, string15 username)
{
	ptrUser pFind, pRun;
	
	if (*pFirst == NULL)
		printf("List is empty\n");
	else {
	    pFind = search(*pFirst, username);	
		
		if (pFind == NULL)
			printf("%s is not in the list\n", username);
		else { //found node to delete
		    if (*pFirst == pFind) {
				//delete first node
				*pFirst = (*pFirst)->pNext;
			}
            else { //delete middle or end
               pRun = *pFirst;
               while (pRun->pNext != pFind)
				   pRun = pRun->pNext;
			   
			   pRun->pNext = pFind->pNext;
			}			
			free(pFind);
			pFind = NULL;
		}
	}
}

void
saveToText(ptrUser pFirst)
{
	string20 strName;
	FILE *   pText;
	
	if (pFirst == NULL)
		printf("Nothing to save \n");
	else {
  	  printf("Enter filename : ");
	  scanf("%s", strName);
	  
	  pText = fopen(strName, "wt");
	  if (pText != NULL){
		  while (pFirst != NULL){
			  fprintf(pText,"%s %s\n", 
			          pFirst->sData.username, pFirst->sData.password);
			  fprintf(pText, "%s\n", pFirst->sData.info.name.first);
			  fprintf(pText, "%c\n%.2f %.2f\n", pFirst->sData.type,
              		  pFirst->sData.creditLimit, pFirst->sData.outstanding);
			  fprintf(pText, "\n");
			  pFirst = pFirst->pNext;
		  }
		  fclose(pText);
	  }
	  else printf("Error writing to file\n");
	}	
}

void 
loadFromText(string20 strFile, ptrUser * pFirst)
{
	FILE * pFile;
	string20 user, pass;
	ptrUser pNew;
    char ch;	
	
	if ((pFile = fopen(strFile, "r")) != NULL){
		
	  while (fscanf(pFile,"%s%s", user, pass) == 2) {
		printf("user = %s; pass =%s**\n", user, pass );
		
		/* create linked list */
		pNew = malloc(sizeof(userType));
		strcpy(pNew->sData.username, user);
		strcpy(pNew->sData.password, pass);
		
		fgets(pNew->sData.info.name.first, 21, pFile);
		pNew->sData.info.name.first[strlen(pNew->sData.info.name.first)- 1] = '\0';
		
		ch = fgetc(pFile);
		pNew->sData.type = ch;
		fgetc(pFile);
		
		fscanf(pFile, "%f%f", &pNew->sData.creditLimit, &pNew->sData.outstanding);
		fgetc(pFile);  // extra new line
		
		//connect nodes at the head of the list
		pNew->pNext = *pFirst;
		*pFirst = pNew;
	  }
	  fclose(pFile);
	}
	else printf("Error opening file\n");
}

void 
saveToBin(ptrUser pFirst)
{	string20 strName;
	FILE *   pBin;
	
	if (pFirst == NULL)
		printf("Nothing to save \n");
	else {
  	  printf("Enter filename : ");
	  scanf("%s", strName);
	  
	  if ((pBin = fopen(strName, "wb"))!= NULL){
		  
/*		  fwrite(pFirst->sData.username, 21, 1, pBin);
		  fwrite(pFirst->sData.password, sizeof(string20), 1, pBin);
		  fwrite(&pFirst->sData.info, sizeof(userInfoType), 1, pBin);
		  fwrite(&pFirst->sData.type, sizof(char), 1, pBin);
		  fwrite(&pFirst->sData.creditLimit, sizeof(float),1, pBin);
		  fwrite(&pFirst->sData.outstanding, sizeof(float),1, pBin);
		  fwrite(pFirst->sData.cart, sizeof(arrBought), 1, pBin);
		//fwrite(pFirst->cart, sizeof(prodBoughtType), MAX_CART, pBin);  
		  fwrite(&pFirst->sData.nItems, sizeof(int), 1, pBin);
*/
          while (pFirst != NULL) {
		      fwrite(&pFirst->sData, sizeof(dataType), 1, pBin);
		      pFirst = pFirst -> pNext;
		  }
			  
		  fclose(pBin);
	  }
	  else printf("Error opening file for writing\n");
	  
	}
	
}

void
loadFromBin(string20 strFile, ptrUser * pFirst)
{
	FILE * pFile;
	dataType sTemp;
	ptrUser pNew;
	
	
	if ((pFile = fopen(strFile, "rb"))!= NULL) {
       while (fread(&sTemp, sizeof(dataType), 1, pFile) == 1) {
		   pNew = malloc(sizeof(userType));
		   pNew->sData = sTemp;
		   
		   
		   pNew->pNext = *pFirst;
		   *pFirst = pNew;
	   }
	   fclose(pFile);
	}
	else printf("Error opening file for reading\n");
}

void
modifyFNFromBinary(string20 username, string20 strNewFN, string20 strFile)
{
	FILE *pFile;
	dataType sTemp;
	int found = 0;
	long int pos;
	
	if ((pFile = fopen(strFile, "r+b")) != NULL) {
		while (!found && fread(&sTemp, sizeof(dataType), 1, pFile) == 1) {
		  if (strcmp(sTemp.username, username) == 0) {
			found = 1;
			strcpy(sTemp.info.name.first, strNewFN);
			
/*			pos = ftell(pFile);
			fseek(pFile, pos- sizeof(dataType), SEEK_SET); 
*/
            fseek(pFile, -sizeof(dataType) ,SEEK_CUR);
			
			fwrite(&sTemp, sizeof(dataType), 1, pFile);
		  }
		}
		fclose(pFile);
	}
	else printf("Error opening the file\n");
}

int main()
{
    // userType user;
	
/*	userType * pUsers = NULL,
	         * pNew;
*/
    ptrUser pUsers = NULL,
	        pNew, pLast;
	int opt;
	char cDump;
	
	ptrUser pRun, pTrail;
	string20 strFile;

/* Adds new data at the end of the list 	
	    pNew = malloc(sizeof(userType));
        signUp(pNew);
	    (*pNew).pNext = NULL;  // pNew->pNext = NULL;
		
	    if (pUsers == NULL)  //nothing yet in the list
		   pUsers = pNew;   //pNew is the first node
		else // connect the node
		   pLast->pNext = pNew;
		
		pLast = pNew;
*/	
	
	do {
	
	    pNew = malloc(sizeof(userType));
		signUp(pNew);
		pNew->pNext = NULL;
		
		if (pUsers == NULL)  //list is empty
		   pUsers = pNew;
		else if (strcmp(pUsers->sData.username, pNew->sData.username) > 0){
			//inserting at head of list
			pNew->pNext = pUsers;
			pUsers = pNew;
		}
		else {//insert at middle or end
			pRun = pUsers;
			while (pRun != NULL &&
			       strcmp(pNew->sData.username, pRun->sData.username) > 0){
                pTrail = pRun;				
				pRun = pRun->pNext;
			}
			
			pTrail->pNext = pNew;
			pNew->pNext = pRun;
		}
		
	
	    printf("Another sign-up?");
	    scanf("%d%c", &opt, &cDump);
	}while (opt == 1);
	
	displayAllRecur(pUsers);
	
	saveToText(pUsers);
	
	//saveToBin(pUsers);
	
	//deleteNode(&pUsers, "natslim");
	
//	displayAll(pUsers);
//	freeAll(&pUsers);
	
	printf("Enter filename: ");
	scanf("%s", strFile);
	loadFromText(strFile, &pUsers);
	// pUsers = load2(strFile);

    displayAll(pUsers);
    //freeAll(&pUsers);
	
	pUsers = NULL;
	
//	printf("Enter filename: ");
//	scanf("%s", strFile);
//	loadFromBin(strFile, &pUsers);
	
    displayAll(pUsers);
    freeAll(&pUsers);	
	
	return 0;
}


