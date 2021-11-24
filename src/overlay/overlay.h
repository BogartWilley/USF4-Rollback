#pragma once

#include <windows.h>
#include <d3d9.h>

void InitializeOverlay(HWND hWnd, IDirect3DDevice9* lpDevice);
void DrawOverlay();
void FreeOverlay();

LRESULT WINAPI OverlayWindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);