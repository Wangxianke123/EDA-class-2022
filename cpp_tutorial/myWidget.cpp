#include "myWidget.h"

MyWidget::MyWidget()
{
    label = new QLabel();
    QString str;
    str.append("This is a sub-class of QWidget.\n");
    str.append("The keyPressEvent() has been overridden.\n");
    str.append("Press any key and check the Text-Box below.\n");
    label->setText(str);
    
    console = new QTextEdit();
    console->setReadOnly(true);
    console->setFixedSize(100, 100);
    console->setAlignment(Qt::AlignCenter);
    console->setFontPointSize(56);

    layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(console);

    this->setLayout(layout);
}

MyWidget::~MyWidget()
{

}

/**
 * @brief please check the line 627 of "qwidget.h"
 * @brief we are overriding "keyPressEvent()" here
 */
void MyWidget::keyPressEvent(QKeyEvent* e)
{
    // print pressed key in a new window
    QString str = e->text();
    console->setText(str);
}
