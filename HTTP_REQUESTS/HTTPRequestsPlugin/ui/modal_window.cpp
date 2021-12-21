#pragma once
#include "stdafx.h"
#include "modal_window.h"
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <thread>
#include <vector>
#include <string>

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;
static std::string notifyName;

ModalWindow::ModalWindow(WindowPtr ui) : DSDialog(ui, "ui/modal_window.xml")
{    
    closeButton = Button::Connect(ui, "closeButton");
    closeButton.OnClick += event(&ModalWindow::onCloseClicked);
    onInitiate();
}

void ModalWindow::onInitiate()
{

}

void ModalWindow::onCloseClicked(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    DialogResult result = DialogResult::None;
    if (btn.getObject() == closeButton.getObject())    
        result = DialogResult::OK;            
    else
        result = DialogResult::Cancel;

    window.setDialogResult(result);
    window.close();
}

ModalWindow::~ModalWindow()
{
}
