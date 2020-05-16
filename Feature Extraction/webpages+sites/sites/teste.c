#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

int main()
{
	CURL *curl;
	CURLcode res;
	
	int contador;
	char tab[100];
	float z[1000];
	char lol[256]="";
	char site[1024];
	char aux2[256]="";
	char limpasite[]="http://www.";
	int aux;
	int i=0;
	int verifica=0;
	FILE* ficheiro;
	FILE* jm;
	////////////////////////////////////////////////////////
	curl = curl_easy_init();
	
	////////////////////////////////////////////////////////

	//Inicializa site
	strcpy(site,"http://www.");

	// abrir o ficheiro 
	ficheiro = fopen("dias.txt","r");   /* Arquivo ASCII, para leitura */
	if(!ficheiro)
	{
		printf( "Erro na abertura do arquivo");
		
	}

	while(fgets(tab,100,ficheiro))
	{
		contador=0;

		while(tab[contador]!=10)
		{
			while(tab[contador]!=44 && verifica==0)
			{	
				append(aux2,tab[contador]);
				contador++;
			}
			if(verifica==0)
				contador++;
				
			verifica=1;
					
			if(tab[contador+1]==10)
			{
				
				append(lol,tab[contador]);//para copiar ultima letra
			
				
				strcat(site,lol); // junta o site em si, ao http://www.
			
				// CODIGO NOVO ///////////////////////
				strcat(aux2,".txt");
				curl = curl_easy_init();
				if(curl) {
							printf("%s\n",site);
						jm=fopen(aux2,"a+");
						strcpy(aux2,"");
						curl_easy_setopt(curl, CURLOPT_URL, site);
						curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
						curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
						curl_easy_setopt(curl, CURLOPT_WRITEDATA, jm);
						res = curl_easy_perform(curl);
						if(res != CURLE_OK)
							fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
						curl_easy_cleanup(curl);
					fclose(jm);
				}
				
				//FIM CODIGO NOVO ///////////////////////////////
				
				
				for(aux=0;aux<50;aux++)
					lol[aux]=0;
					i++;	
				strcpy(site,limpasite);
				
				verifica=0;
			}
			else if(tab[contador]!=34)
				append(lol,tab[contador]);
				contador++;
			
		}

	}

	fclose(ficheiro);

 return(0);
}
	system("./CutyCapt --url=http://www.record.xl.pt --out=re.png");



}
