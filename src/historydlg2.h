/*
 * historydlg2.h - a dialog to show event history, using Qt4 API only
 * Copyright (C) 2010  Tobias Markmann
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef HISTORYDLG2_H
#define HISTORYDLG2_H

#include "psiaccount.h"

#include <QDialog>
#include <QObject>

#include "ui_historydlg2.h"

namespace XMPP {
	class Jid;
}

using namespace XMPP;

class HistoryDlg2 : public QDialog
{
	Q_OBJECT
	
	public:
		HistoryDlg2(const Jid &j, PsiAccount *);
		~HistoryDlg2();

	private:
		Ui::HistoryDlg2 m_ui;
};

#endif
