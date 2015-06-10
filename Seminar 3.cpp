/*
Napisati program koji otvara web stranice navedene u datoteci.
Program mora otvoriti neku tekstualnu datoteku na disku u kojoj se nalazi popis web stranica
Svaku web stranicu mora otvoriti u zasebnom prozoru preglednika. Svaki poziv CreateProcess funkcije
mora biti u novoj niti.
*/
#include <stdio.h>
#include <conio.h> 
#include <windows.h>
#include <string>

STARTUPINFO startInfo;	// Globalna varijabla za startup info

DWORD WINAPI StvoriProces(LPVOID);	// Prototip funkcije

void main()
{
	char siteData[20][256]; // Niz u koji se upisuju procitane stranice - moze procitati do 20 stranica
	FILE *fp;
	int i =0; // Brojac
	HANDLE hThread; 

	ZeroMemory(&startInfo, sizeof(startInfo));  // Popunjava se startup info
	startInfo.cb = sizeof(startInfo);

	if (( fp=fopen("stranice.txt", "r" )) == NULL ) // Pokusava otvoriti file, ako nije uspio onda izlazi iz programa
	{
		fprintf( stderr, "Error opening file %s!", "stranice.txt" );
		_getch();
		exit(1);
	} 

	while( (fscanf(fp,"%s",siteData[i] ) != EOF)&&(i<20) ) // Zapisuje stringove iz file-a u siteData dok ne dode do kraja
	{
		hThread=CreateThread(NULL,0, StvoriProces, siteData[i],0,0); // Pozivanje funkcije StvoriProces u novom thread-u i salje pokazivac na siteData[i]
		printf("Main thread read %s \n",siteData[i]); // Ispisuje sto je procitao iz file-a glavni thread
		i++; // Inkrement brojaca

	}

	printf("Read complete \n"); // Zatvaranje programa
	fclose(fp);
	CloseHandle(hThread);
	_getch();
	return; 
}

DWORD WINAPI StvoriProces(LPVOID lpParam) // Funkcija koja prima pointer na pocetak stringa koji se koristi
{
	PROCESS_INFORMATION processInfo; // Inicijaliziranje varijable za podatke u procesu koji ce se stvoriti
	char browserPath[512]= "c:\\Program Files\\Internet Explorer\\iexplore.exe"; // Lokacija IE
	char commandLine[256]="iexplore.exe "; // Inicijalizacija varijable za naredbu otvaranja stranice

	strcat(commandLine,(char*)lpParam); // Dodavanje stranice u naredbu

	printf("Thread parsing %s \n", commandLine); // Ispisuje koju stranicu je thread procitao 

	if(!CreateProcess(browserPath, commandLine, // Naredbe za stvaranje procesa, salje joj se lokacija IE-a (browserPath) i naredba (commandLine)
		NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &processInfo))
	{
		 printf("CreateProcess failed \n"); 
	}

	CloseHandle (processInfo.hThread); // Oslobadanje processInfo
	CloseHandle (processInfo.hProcess); 

	return 0;
}
