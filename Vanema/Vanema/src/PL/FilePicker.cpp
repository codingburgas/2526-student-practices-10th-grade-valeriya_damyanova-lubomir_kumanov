#define WIN32_LEAN_AND_MEAN 
#include <string>           
#include <windows.h>        
#include <commdlg.h>

std::string OpenPosterFileDialog() {
    char filename[MAX_PATH] = { 0 };
    OPENFILENAMEA ofn;
    SecureZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Image Files (*.png;*.jpg;*.jpeg;*.webp)\0*.png;*.jpg;*.jpeg;*.webp\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = ".\\assets\\posters\\";
    ofn.lpstrTitle = "Select Movie Poster";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        return std::string(filename);
    }
    return "";
}