#include "sss.h"
#include "randombytes.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//argv[1] -> pw
//argv[2] -> ntrustees

int main(int argc, char **argv){
	uint8_t data[sss_MLEN];//, restored[sss_MLEN];
	int ntrustees=atoi(argv[2]);
	sss_Share shares[ntrustees];
	int i;
	char *c;
	c = (char *)malloc(ntrustees);
	if(c==NULL){
		printf("Unable to allocate memory for data\n");
		exit(1);
	}
	strcpy(c,argv[1]);
	strncpy(data, c, sizeof(data)+1);
	sss_create_shares(shares, data, ntrustees, ntrustees);
	FILE *fp;
	char t[20];
	char temp[20];
	char* q="trustees";
	for(i=0;i<ntrustees;i++){
		strcpy(t,q);
		sprintf(temp,"%d",i);
		strcat(t,temp);
		strcat(t,".bin");
		fp=fopen(t,"wb");
		fwrite(shares[i],sizeof(shares),1,fp);
		fclose(fp);
		//printf("%s",shares[i]);
		fp=NULL;
	}
	// Split the secret into 5 shares (with a recombination theshold of 4)
	return 0;
}

/*#include "sss.h"
#include "randombytes.h"
#include <assert.h>
#include <string.h>

int main()
{
	uint8_t data[sss_MLEN], restored[sss_MLEN];
	sss_Share shares[5];
	size_t idx;
	int tmp;
	int ntrustees=10;
	int i;
	// Read a message to be shared
	strncpy(data, "Tyler Durden isn't real.", sizeof(data));
	// Split the secret into 5 shares (with a recombination theshold of 4)
	sss_create_shares(shares, data, ntrustees, ntrustees);
	FILE *fp;
	char t[20];
	char temp[20];
	for(i=0;i<ntrustees;i++){
		t="trustees";
		sprintf(temp,"%d",i);
		strcat(t,temp);
		strcat(t,".txt");
		printf("%s\n", t);
		fp=fopen(t,"w");
		fputs(shares[i],fp);
		fclose(fp);
		fp=NULL;
	}

	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(restored, shares, ntrustees);
	assert(tmp == 0);
	assert(memcmp(restored, data, sss_MLEN) == 0);
	return 0;
}
*/
