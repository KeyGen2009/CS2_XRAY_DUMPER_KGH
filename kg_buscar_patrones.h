#pragma once

intptr_t BuscarPatrones(BYTE* buffer, SIZE_T tam, const char* patron, const char* mascara)
{
	SIZE_T patron_tam = strlen(mascara);

	for (SIZE_T i = 0; i <= tam - patron_tam; i++)
	{
		bool encontrado = true;

		for (SIZE_T j = 0; j < patron_tam; j++)
		{
			if (mascara[j] != '?' && (BYTE)patron[j] != buffer[i + j])
			{
				encontrado = false;
				break;
			}
		}

		if (encontrado)
		{
			return i;
		}
	}

}

