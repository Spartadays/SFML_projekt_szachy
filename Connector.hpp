#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

STARTUPINFO sti = { 0 };
SECURITY_ATTRIBUTES sats = { 0 };
PROCESS_INFORMATION pi = { 0 };
HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
BYTE buffer[19984];
DWORD writ, excode, read, available;

bool koniec = false;

void ConnectToEngine(char* path)
{
	pipin_w = pipin_r = pipout_w = pipout_r = NULL;
	sats.nLength = sizeof(sats);
	sats.bInheritHandle = TRUE;
	sats.lpSecurityDescriptor = NULL;

	CreatePipe(&pipout_r, &pipout_w, &sats, 0);
	CreatePipe(&pipin_r, &pipin_w, &sats, 0);

	sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	sti.wShowWindow = SW_HIDE;
	sti.hStdInput = pipin_r;
	sti.hStdOutput = pipout_w;
	sti.hStdError = pipout_w;

	CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &sti, &pi);
	//std::cout << "Powinno polaczyc xD" << std::endl;
}

std::string getNextMove(std::string position)
{
	std::string str;
	position = "position startpos moves " + position + "\ngo\n";
	//std::cout << " POZYCJA OD STOCKFISHA: " << position << std::endl;
	WriteFile(pipin_w, position.c_str(), position.length(), &writ, NULL);
	Sleep(1000);

	PeekNamedPipe(pipout_r, buffer, sizeof(buffer), &read, &available, NULL);
	do
	{
		ZeroMemory(buffer, sizeof(buffer));
		if (!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read) break;
		//buffer[read] = 0;
		//std::cout << buffer;
		str += (char*)buffer;
	} while (read >= sizeof(buffer));
	//std::cout << str << std::endl;
	int n = str.find("bestmove");
	int m = str.find("mate");

	if (m != -1)
	{
		if (str[m + 5] == '1') { koniec = true; }

		return str.substr(str.find(" pv") + 4, 5);
	}
	else if (n != -1)
	{
		return str.substr(n + 9, 5);
	}
	else
		return "error";
}

void CloseConnection()
{
	WriteFile(pipin_w, "quit\n", 5, &writ, NULL);
	if (pipin_w != NULL) CloseHandle(pipin_w);
	if (pipin_r != NULL) CloseHandle(pipin_r);
	if (pipout_w != NULL) CloseHandle(pipout_w);
	if (pipout_r != NULL) CloseHandle(pipout_r);
	if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
	if (pi.hThread != NULL) CloseHandle(pi.hThread);
}

#endif CONNECTOR_H