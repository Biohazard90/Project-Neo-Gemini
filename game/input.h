#ifndef INPUT_H
#define INPUT_H

#include <QKeyEvent>
#include <QHash>

enum Button_e
{
    BT_NONE = 0,
    BT_UP = (1 << 0),
    BT_DOWN = (1 << 1),
    BT_LEFT = (1 << 2),
    BT_RIGHT = (1 << 3),
    BT_SHOOT = (1 << 4)
};

class Input
{
    Input();
    static Input instance;
public:

    static Input *GetInstance();

    void Init();

    void AddMapping(Qt::Key key, Button_e button);
    void ClearMapping(Qt::Key key);

    Button_e GetBinding(Qt::Key key);

    bool OnKeyPressed(QKeyEvent *event);
    bool OnKeyReleased(QKeyEvent *event);

    bool IsButtonDown(Button_e button);

private:

    int buttonsDown;

    QHash<Qt::Key, Button_e> mapping;
};

#endif // INPUT_H
