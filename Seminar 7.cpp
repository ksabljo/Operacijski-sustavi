/*
Napisati program koji otvara slike iz pojedine mape. Korisnik pokreće program na način da upiše ime programa
i iza toga broj(npr. OtvoiSlike.exe 5). Broj označava koliko program slika treba dohvatiti iz trenutne mape.
Ako je broj veći od postojećih datoteka, program javlja grešku. Program dohvaća prvih n slika koje je korisnik
naveo i otvara u programu mspaint.exe. Svaka slika se otvara preko posebne niti.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;


STARTUPINFO startInfo;	//koristimo prilikom pokretanja procesa
PROCESS_INFORMATION processInfo;

/*Funkcija za konvertiranje stringa u LPTSTR koji se koristi kao drugi parametar pri pokretanju procesa*/
LPTSTR ConvertToLPTSTR(string stringToConvert)
{
	WCHAR*  wprocessName = new WCHAR[stringToConvert.size()+1];
	mbstowcs(wprocessName, stringToConvert.c_str(), stringToConvert.size()+1);

	return wprocessName;
}
int brojac=-1;	//-1 jer se pri prvom ulasku u funkciju poveća za 1 pa postaje 0

/*Funkcija koju pozivamo prilikom kreiranja svake niti*/
DWORD WINAPI otvori(LPVOID parametriNiti)
{
	brojac++;
	string slika[8]={"1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg", "6.jpg", "7.jpg", "8.jpg"};//niz stringova u koji su pohranjene slike
	/*Svaki put kad se kreira nova nit prikazuje se slijedeća slika*/
	string name="C:\\Windows\\System32\\mspaint.exe "+slika[brojac];
	LPTSTR lpCommandLine=ConvertToLPTSTR(name);//poziva se funkcija za konv.
	ZeroMemory(&startInfo, sizeof(startInfo)); 
	startInfo.cb = sizeof(startInfo); 
	/*kreira se proces koji otvara ono što piše u lpCommandLine, ukoliko dođe do 		greške ispisuje se error*/
	if ( !CreateProcess ( NULL,lpCommandLine , NULL, NULL, FALSE, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, 
	&startInfo, &processInfo)) 
	{ 
		fprintf(stderr,"CreateProcess failed on error %d \n");
		GetLastError (); 
		ExitProcess(1);
	}
	return 0;
}

/*funkcija koja se poziva ako smo krivo unijeli parametre*/
void display_usage(void) 
{ 
 fprintf(stderr, "\nProper Usage is: " ); 
 fprintf(stderr, "\n\nLIST_IT filename.ext\n" ); 
} 

int main(int argc, char *argv[] )
{
	/*Ako je broj argumenata manji od 2 pozovi funkciju display_usage()*/
	if( argc < 2 ) 
	{ 
		display_usage(); 
		getch(); 
		exit(1); 
	} 
	HANDLE hThread; //deklaracija niti
	/*Drugi argument je spremljen kao string pa ga treba konvertirati u integer*/
	int broj=atoi(argv[1]);
	/*Ako je korisnik upisao broj veći od 8 program javlja grešku jer ima 8 	slika*/
	if(broj>8) {
		fscanf(stderr, "Greska");
		exit(1);
	}
	//petlja se vrti onoliko puta koliki je broj koji je unio korisnik
	for(int i=0;i<broj;i++)
	{
		hThread=CreateThread(NULL,0,otvori,NULL,0,0);//kreira se nit
	}
	CloseHandle(hThread); 
	getch(); 

return 0;
}                               
