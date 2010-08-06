#ifndef JIDVALIDATOR_H
#define JIDVALIDATOR_H

#include <QValidator>

class JidValidator : public QValidator
{
    Q_OBJECT
public:
    explicit JidValidator(QObject *parent = 0);
	State validate(QString &input, int &pos) const;
signals:

public slots:

};

#endif // JIDVALIDATOR_H
