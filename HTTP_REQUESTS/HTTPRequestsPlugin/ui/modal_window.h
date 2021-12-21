#pragma once
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <iostream>
#include <string>

class ModalWindow : public Dewesoft::MUI::DSDialog
{
public:
    explicit ModalWindow(Dewesoft::MUI::WindowPtr ui);
    ~ModalWindow();

private:
    Dewesoft::MUI::Button closeButton;
    void onInitiate();
    void onCloseClicked(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
};
