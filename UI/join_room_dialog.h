#ifndef JOIN_ROOM_DIALOG_H
#define JOIN_ROOM_DIALOG_H

#include <QDialog>

namespace Ui {
class JoinRoomDialog;
}

class JoinRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoinRoomDialog(QWidget *parent = nullptr);

    ~JoinRoomDialog();

    std::string roomId();

    std::string displayName();

    std::string pin();

private:
    Ui::JoinRoomDialog *ui;
};

#endif // JOIN_ROOM_DIALOG_H
