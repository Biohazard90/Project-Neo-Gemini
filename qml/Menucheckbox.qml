import QtQuick 1.1

Menubutton {

    property bool checked: false
    signal checkChanged( bool enabled )

    leftAlign: true
    Checkbox {
        state: checked ? 'Checked':''
        id: check
        x: parent.width - width - (parent.height - width) * 0.5
        y:  (parent.height - width) * 0.5
    }

    onClick: {
        check.state = (check.state=="") ? "Checked" : ""
        checkChanged((check.state=="Checked")?true:false)
    }

    onHover: {
        check.focusset = !check.focusset
    }
}
