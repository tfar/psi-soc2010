/*
 * historydlg2.cpp - a dialog to show event history, using Qt4 API only
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

#include "historydlg2.h"

HistoryDlg2::HistoryDlg2(const Jid &j, PsiAccount *parent) : QDialog()
{
	m_ui.setupUi(this);
}

HistoryDlg2::~HistoryDlg2()
{

}
