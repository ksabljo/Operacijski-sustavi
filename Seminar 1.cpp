/*algoritam	za	raèunanje	kamata	za	dobivene	iznose	glavnica*/
#define _CRT_SECURE_NO_WARNINGS
#define threadNumber 4
#include <stdio.h>
#include <Windows.h>
//globalne varijable
double suma_kamata;
int brojac;
HANDLE hMutex;

//struktura za slanje vise parametara
typedef struct
{
	FILE* fp_out; 
	float glavnica;
	int kamata;
	double izracun_kamate;
	int brojac;
}
argumenti, *arg_point;

DWORD WINAPI _izracun(LPVOID);

int main(int argc, char** argv)
{
	hMutex = CreateMutex(NULL, FALSE, NULL);
	FILE* fp_i;
	FILE* fp_o;
	argumenti arg[threadNumber];
	//otvaranje input datoteke
	fp_i =fopen("input.csv", "r");
	if (fp_i == NULL)
	{
		perror("Greska pri otvaranju ulazne datoteke\n");
		exit(EXIT_FAILURE);
	}
	//otvaranje izlazne datoteke
	fp_o = fopen("output.csv", "w");
	int i = 0;
	//ocitavanje podataka iz datoteke
	while ((fscanf(fp_i, "%f %*c %i %*c %*c", &arg[i].glavnica, &arg[i].kamata)) != EOF){
		arg[i].fp_out = fp_o;
		i++;
	}
	fclose(fp_i);
	HANDLE tHandle[threadNumber];
	//kreiranje vise threadove koji imaju istu funkciju
	for (int i = 0; i < threadNumber; i++)
	{
		brojac++;
		arg[i].brojac = brojac;
		tHandle[i] = CreateThread(0, 0, _izracun, &arg[i], 0, 0);
		if (tHandle[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}

	WaitForMultipleObjects(threadNumber, tHandle, TRUE, INFINITE);

	fprintf(fp_o,"Ukupan_izracunati_iznos_kamate: %lf\n", suma_kamata);
	for (int i = 0; i < threadNumber; i++)
		CloseHandle(tHandle[i]);

	CloseHandle(hMutex);
	fclose(fp_o);
	
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);

	char lpCommandLine[] ="notepad.exe output.csv";
	//char* u LPWSRT
	wchar_t wtext[25];
	mbstowcs(wtext, lpCommandLine, strlen(lpCommandLine) + 1);
	LPWSTR CommandLine = wtext;
	//kreiranje procesa
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &processInfo))
	{
		fprintf(stderr, "CreateProcess failed on error %d \n", GetLastError());
		ExitProcess(1);
	}

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	return 0;
}
//funkcija threada
DWORD WINAPI _izracun(LPVOID arg)
{
	arg_point P = (arg_point)arg;
	P->izracun_kamate = P->glavnica*P->kamata / 100;
	fprintf(P->fp_out,"izracunati_iznos_kamate_%d = %lf\n",P->brojac, P->izracun_kamate);

	WaitForSingleObject(hMutex, INFINITE);//mutex zauzet
	suma_kamata += P->izracun_kamate;
	ReleaseMutex(hMutex); //mutex slobodan
	return 0;
}