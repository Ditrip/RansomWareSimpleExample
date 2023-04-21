#pragma once

#ifndef HEADER
#include <windows.h>
#include <string>
#endif 

#include <iostream>
#include <lmcons.h>
#include <wincrypt.h>
#include <fstream>
#include "MyStingsOp.h"


void setMyExDir();
void addMyFiles();
std::wstring getMyDir();
void myEncryption();
void myDecryption();

void setMyExDir() {
	std::wstring directoryPath = getMyDir();
	OutputDebugString(L"Dir is: ");
	OutputDebugString(directoryPath.c_str());
	CreateDirectory(directoryPath.c_str(), NULL);
}

void addMyFiles() {
	std::wstring directoryPath = getMyDir();
	directoryPath += L"\\testFile.txt";
	HANDLE myFile = CreateFile(directoryPath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,CREATE_ALWAYS,0,NULL);
	std::string myText = "Roses are red\nmy name is not Dave\nthis makes no sence\nmicrowave.";
	DWORD bytesWritten;
	WriteFile(myFile, myText.c_str(), myText.size(), &bytesWritten, NULL);
	CloseHandle(myFile);
	
}

std::wstring getMyDir() {
	wchar_t username[UNLEN + 1];
	wchar_t* usrP = username;
	DWORD userlenght = UNLEN + 1;
	GetUserNameW(username, &userlenght);
	std::wstring directoryPath = L"C:\\Users\\";
	directoryPath += username;
	directoryPath += L"\\ExampleDir";

	return directoryPath;
}

void myEncryption() {
	std::wstring path = getMyDir();
	std::fstream myFile, tempFile;

	myFile.open(path + L"\\testFile.txt", std::fstream::in);
	tempFile.open(path + L"\\tmp.txt", std::fstream::out);
	if (!myFile || !tempFile)
	{
		OutputDebugString(L"\nError occured (myEncryption (fstream.open))\n");
		return;
	}

	char ch;
	// This while loop reads every char
	// std::noskipws allows to not skip white spaces
	while (myFile >> std::noskipws >> ch) {
		ch += 100;
		tempFile << ch; // << is for append
	}
	myFile.close();
	tempFile.close();

	myFile.open(path + L"\\testFile.txt", std::fstream::out | std::fstream::trunc);
	tempFile.open(path + L"\\tmp.txt", std::fstream::in);

	if (!myFile  || !tempFile)
	{
		OutputDebugString(L"\nError occured (myEncryption)\n");
		return;
	}

	while (tempFile >> std::noskipws >> ch) {
		myFile<<ch;
	}
	myFile.close();
	tempFile.close();
	
	std::wstring tmpStr = path + L"\\tmp.txt";
	std::remove(WStringToString(tmpStr).c_str());
	
}

void myDecryption() {
	std::wstring path = getMyDir();
	std::fstream myFile, tempFile;

	myFile.open(path + L"\\testFile.txt", std::fstream::in);
	tempFile.open(path + L"\\tmp.txt", std::fstream::out);

	if (!myFile || !tempFile)
	{
		OutputDebugString(L"\nError occured (myDecryption)\n");
		return;

	}
	char ch;
	while (myFile >> std::noskipws >> ch) {
		ch -= 100;
		tempFile << ch; // << is for append
	}

	myFile.close();
	tempFile.close();
	
	myFile.open(path + L"\\testFile.txt", std::fstream::out);
	tempFile.open(path + L"\\tmp.txt", std::fstream::in);
	
	while (tempFile >> std::noskipws >> ch) {
		myFile<<ch;
	}

	myFile.close();
	tempFile.close();

	std::wstring tmpStr = path + L"\\tmp.txt";
	std::remove(WStringToString(tmpStr).c_str());
	
}