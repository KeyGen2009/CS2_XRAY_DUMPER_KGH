#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;


//Variables del proceso
DWORD pid;
HANDLE hp;


//En este header vas a tener funciones especiales para buscar modulos en un proceso externo
#include "KGModulos.h"
#include "kg_buscar_patrones.h"

//Modulos del juego client.dll
ModuloInfo CLIENT;

//Wallhack (tienen que completarlo con lo que ustedes encuentren o tengan)
uintptr_t WALLHACK;
unsigned char xray_off[] = { 0x32,0xC0 };
unsigned char xray_on[] = { 0xB0,0x01 };


int main()
{
	//Abrimos el CS2 para poder leer y escribir en memoria.
	hp = kg_abrir_proceso("cs2.exe", PROCESS_ALL_ACCESS);

	//Carga el modulo que quieras
	CLIENT = kg_obtener_modulo_info(pid, L"client.dll");

	//Crear los patrones y la mascara del xray
	const char* XRAY_PATRON =
		"\x00\xC0"
		"\x4C\x8B\xA4\x24\xC8\x00\x00\x00"
		"\x48\x8B\xB4\x24\xC0\x00\x00\x00"
		"\x48\x8B\x9C\x24\xD0\x00\x00\x00"; //0 -> 25 

	const char* XRAY_MASCARA = "?xxxxxxxxxxxxxxxxxxxxxxxx";

	//Crear un espacio en el cheat para la imagen virtual
	BYTE* buffer_client = new BYTE[CLIENT.size];
	SIZE_T client_bytes;

	//Cargar la imagen dentro del buffer
	ReadProcessMemory(hp, (LPBYTE)CLIENT.base, buffer_client, CLIENT.size, &client_bytes);
	if (client_bytes == CLIENT.size)
	{
		cout << "Se cargo correctamente la imagen de clietn.dll" << endl;
	}

	//Buscar de forma automatica la direccion de XRAY
	WALLHACK = BuscarPatrones(buffer_client, CLIENT.size, XRAY_PATRON, XRAY_MASCARA);
	cout << "TE RE KBIO VALVE la direccion de XRAY es client.dll + 0x" << hex << WALLHACK << endl;

	while (1)
	{

		if (GetKeyState(VK_F1))
		{
			WriteProcessMemory(hp, (LPBYTE)(CLIENT.base + WALLHACK), &xray_on, sizeof(xray_on), 0);
		}
		else {
			WriteProcessMemory(hp, (LPBYTE)(CLIENT.base + WALLHACK), &xray_off, sizeof(xray_off), 0);

		}

		Sleep(10);
	}

}