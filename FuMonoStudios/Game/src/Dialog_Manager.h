// dialog_manager.h
#ifndef DIALOG_MANAGER_H
#define DIALOG_MANAGER_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class DialogManager {
public:
    DialogManager(const std::string& filePath);

    // Obtener el texto del di�logo actual
    std::string GetCurrentDialog();

    // Cambiar al siguiente cuadro de di�logo
    void NextDialog();

private:
    std::vector<std::string> dialogs;
    size_t currentDialogIndex;
};

#endif // DIALOG_MANAGER_H
