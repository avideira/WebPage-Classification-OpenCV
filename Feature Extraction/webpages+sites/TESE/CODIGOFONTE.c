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

	char reniciasite[1024];
	char site[1024];
	char categoria[1024];
	char aux[10];
	char tab[1000];
	char exp[1024];
	char palavraacomparar[256]="";
	char limp[256]="";
	char sitepgravar[256]="";
	int x=0;
	int i=0;
	int numerodepaginas=0;
	int contador=0;
	int verifica=0;
	int passalinha=0;
	FILE* ficheiro;
	FILE* listas;
	
	
	CURL *curl;
	CURLcode res;

	//Inicializa site
	strcpy(site,"http://www.alexa.com/topsites/category;");
	strcpy(reniciasite,"http://www.alexa.com/topsites/category;");
	strcpy(categoria,"/Top/Sports");
	
	strcpy(exp,"site-");
	

	
	
	
	while(numerodepaginas<20)
	{
		sprintf(aux,"%d",numerodepaginas);
		strcat(site,aux);
		strcat(site,categoria);
		strcpy(aux,limp);
		
		
		// sacar o codigo fonte que contem os sites
		curl = curl_easy_init();
		if(curl) 
		{
			printf("%s\n",site);	
						
			ficheiro = fopen("testesite.txt","w");   /* Arquivo ASCII, para leitura */
			if(!ficheiro)
			{
				printf( "Erro na abertura do arquivo");
			}
			curl_easy_setopt(curl, CURLOPT_URL, site);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, ficheiro);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			fclose(ficheiro); 
		}
	
		ficheiro = fopen("testesite.txt","r");   /* Arquivo ASCII, para leitura */
		if(!ficheiro)
		{
			printf( "Erro na abertura do arquivo");
		}
	
		// ir ao codigo fonte , encontrar a "site-listing" e depois retirar os sites para um ficheiro txt
		
		
		while(fgets(tab,500,ficheiro)) // retira 500 caracteres da linha (faz isto linha a linha, ate chegar ao final do ficheiro)
		{
			contador=0;
	
			while(tab[contador]!=10 && verifica==0) // por enquanto que ele nao chega ao fim da linha e nao chegamos a listagem dos sites
			{
				if(tab[contador]==34) // se o caracter for igual a "
				{	
					contador++;
					i=0;
					while(i<5) // guarda 5 caracteres para comparar com o "site-" . 
					{
					
						if(tab[contador]!=10)
							append(palavraacomparar,tab[contador]);
						contador++;
						i++;
					}
			
					if(strcmp(palavraacomparar,exp)==0) //(caso seja igual a "site-" é pq ja chegamos a linha da listagem dos sites)
						verifica=1;
					strcpy(palavraacomparar,limp);
				}
				contador++;
			}
		
			if(verifica==1) //depois de encontrar o site-listing, passar 2 linhas a frente
			{
				passalinha++;
			}
		
			if(verifica==1 && passalinha==4)
			{
			
				i=0;
				while(tab[contador]!=10) // por enquanto que ele nao chega ao fim da linha
				{
					if(tab[contador]==62)
					{
						i++;
						if(i==2)
						{
							contador++; //para passar para o caracter a seguir ao >
							while(tab[contador]!=60) // por enquanto que nao chega ao <
							{
								append(sitepgravar,tab[contador]);
								contador++;
							}
							listas = fopen("listasites.txt","a+");
					
							fprintf(listas,"%s\n",sitepgravar);
							strcpy(sitepgravar,limp);
							fclose(listas);
						}
					}
				
					contador++;
								
				}
			
				verifica=0;
				passalinha=0;
			
			
			}
		
		}
	
	strcpy(site,reniciasite);
	numerodepaginas++;
	}
	
	
	
	
}
