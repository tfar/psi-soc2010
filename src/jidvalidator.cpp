#include "jidvalidator.h"
#include "xmpp_jid.h"

JidValidator::JidValidator(QObject *parent) :
    QValidator(parent)
{

}

QValidator::State JidValidator::validate(QString &input, int &pos) const {
	XMPP::Jid input_jid(input);
	if (input_jid.isValid()) {
		return Acceptable;
	} else {
		return Intermediate;
	}
}
