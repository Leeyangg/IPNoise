#ifndef TEXT_EDIT_WIDGET
#define TEXT_EDIT_WIDGET

#include <stdint.h>

#include <QtCore/QObject>
#include <QtWidgets/QTextEdit>
#include <QtGui/QKeyEvent>

#include <ipnoise-common/log_common.h>

class TextEditWidget
    :   public QTextEdit
{
    Q_OBJECT

    signals:
        void submit();

    public:
        TextEditWidget(QWidget *a_parent = 0);
        virtual ~TextEditWidget();

        virtual void keyPressEvent(QKeyEvent *e);
        virtual void keyReleaseEvent(QKeyEvent *e);

    private:
        int32_t     m_ctrl_pressed;
        int32_t     m_shift_pressed;
};

#endif

