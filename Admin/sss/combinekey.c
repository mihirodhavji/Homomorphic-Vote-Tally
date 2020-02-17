#include "sss.h"
#include "randombytes.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//argv[1] -> ntrustees

int main(int argc, char **argv){
	uint8_t restored[sss_MLEN];
	int ntrustees=atoi(argv[1]);
	sss_Share shares[ntrustees];
	int tmp,i;
	FILE *fp;
	char t[20];
	char temp[20];
	char* q="trustees";
	for(i=0;i<ntrustees;i++){
		strcpy(t,q);
		sprintf(temp,"%d",i);
		strcat(t,temp);
		strcat(t,".bin");
		fp=fopen(t,"rb");
		fread(shares[i],sizeof(shares[i]),1,fp);
		fclose(fp);
		fp=NULL;
	}
	tmp = sss_combine_shares(restored, shares, ntrustees);
	assert(tmp == 0);
	fp=fopen("pw.txt","w");
	fputs((char*)restored,fp);
	fclose(fp);
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

	// Read a message to be shared
	strncpy(data, "Tyler Durden isn't real.", sizeof(data));

	// Split the secret into 5 shares (with a recombination theshold of 4)
	sss_create_shares(shares, data, 5, 4);

	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(restored, shares, 4);
	assert(tmp == 0);
	assert(memcmp(restored, data, sss_MLEN) == 0);
}*/
