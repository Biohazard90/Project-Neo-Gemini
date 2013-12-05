#include "input.h"

#include <QDebug>

Input Input::instance;
Input *Input::GetInstance()
{
    return &instance;
}

Input::Input()
{
    buttonsDown = 0;
}

void Input::Init()
{
    AddMapping(Qt::Key_W, BT_UP);
    AddMapping(Qt::Key_A, BT_LEFT);
    AddMapping(Qt::Key_S, BT_DOWN);
    AddMapping(Qt::Key_D, BT_RIGHT);

    AddMapping(Qt::Key_Up, BT_UP);
    AddMapping(Qt::Key_Left, BT_LEFT);
    AddMapping(Qt::Key_Down, BT_DOWN);
    AddMapping(Qt::Key_Right, BT_RIGHT);

    AddMapping(Qt::Key_Space, BT_SHOOT);
}

void Input::AddMapping(Qt::Key key, Button_e button)
{
    ClearMapping(key);

    mapping.insert(key, button);
}

void Input::ClearMapping(Qt::Key key)
{
    if (mapping.contains(key))
        mapping.remove(key);
}

Button_e Input::GetBinding(Qt::Key key)
{
    if (!mapping.contains(key))
        return BT_NONE;

    return mapping[key];
}

bool Input::OnKeyPressed(QKeyEvent *event)
{
    Button_e button = GetBinding((Qt::Key)event->key());

    if (button == BT_NONE)
        return false;

    buttonsDown |= button;
    return true;
}

bool Input::OnKeyReleased(QKeyEvent *event)
{
    Button_e button = GetBinding((Qt::Key)event->key());

    if (button == BT_NONE)
        return false;

    buttonsDown &= ~button;
    return true;
}

bool Input::IsButtonDown(Button_e button)
{
    return (buttonsDown & button) != 0;
}

void Input::ResetButtons()
{
    buttonsDown = 0;
}
