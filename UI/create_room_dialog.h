#ifndef CREATE_ROOM_DIALOG_H
#define CREATE_ROOM_DIALOG_H

#include <QDialog>

namespace Ui {
class CreateRoomDialog;
}

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(QWidget *parent = nullptr);

    ~CreateRoomDialog();

    std::string roomId();

    std::string description();

    std::string secret();

    std::string pin();

    bool permanent();

    bool isPrivate();

    std::string displayName();

private:
    Ui::CreateRoomDialog *ui;
};

#endif // CREATE_ROOM_DIALOG_H
