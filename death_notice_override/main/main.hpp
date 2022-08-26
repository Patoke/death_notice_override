#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <exception>
#include <intrin.h>
#include <Psapi.h>
#include "../libraries/MinHook/MinHook.h"
#include "../libraries/xorstr.hpp"

// class

// default function buffer size
#define DEFAULT_BUF_SIZE 256

struct CFmtStrN {
	CFmtStrN() {
		m_bQuietTruncation = false;
		m_szBuf[0] = 0;
		m_nLength = 0;
	}

	void* vftable;
	bool m_bQuietTruncation;
	char m_szBuf[DEFAULT_BUF_SIZE];
	int m_nLength;
};

// hooks
typedef CFmtStrN*(__cdecl* sprintf_fn)(CFmtStrN*, const char*, va_list args);
extern sprintf_fn sprintf_original;
extern CFmtStrN* __cdecl sprintf_hook(CFmtStrN* thisptr, const char* unk2, ...);

extern void hooks_init();