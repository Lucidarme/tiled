/*
 * commandbutton.cpp
 * Copyright 2010, Jeff Bland <jksb@member.fsf.org>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "commandbutton.h"
#include "commanddatamodel.h"
#include "commanddialog.h"
#include "commandmanager.h"
#include "maintoolbar.h"
#include "mainwindow.h"
#include "utils.h"

#include <QEvent>
#include <QMenu>
#include <QMessageBox>

using namespace Tiled;
using namespace Tiled::Utils;
using namespace Tiled::Internal;

CommandButton::CommandButton(QWidget *parent)
    : QToolButton(parent)
    , mMenu(new QMenu(this))
{
    setIcon(QIcon(QLatin1String(":images/24x24/system-run.png")));
    setThemeIcon(this, "system-run");
    retranslateUi();

    setPopupMode(QToolButton::MenuButtonPopup);
    setMenu(mMenu);
    CommandManager::instance()->registerMenu(mMenu);

    connect(this, SIGNAL(clicked()), SLOT(runCommand()));
}

void CommandButton::runCommand()
{
    qDebug() << "openGame";

    cmd = new QProcess(this);
    connect( cmd, SIGNAL(readyReadStandardError()), this, SLOT(handleReadStandardError()) );
    connect( cmd, SIGNAL(readyReadStandardOutput()), this, SLOT(handleReadStandardOutput()) );

    QStringList arguments;
    arguments << QLatin1String("-jar") << QLatin1String("desktop-1.0.jar");
    cmd->start(QLatin1String("java"),  arguments);

    if ( cmd->state() == QProcess::NotRunning ) {
       qDebug() << "The process is not running.It exits";
    }
    else if ( cmd->state() == QProcess::Starting ) {
        qDebug() << "The process is started, but the program has not yet been invoked.";
    }else if ( cmd->state() == QProcess::Running ) {
        qDebug() << "The process is running and is ready for reading and writing.";
    }

    if (!cmd->waitForStarted())  {
       qDebug() << "Not yet started";
    }
}

void CommandButton::showDialog()
{
    CommandDialog dialog(window());
    dialog.exec();
}

void CommandButton::changeEvent(QEvent *event)
{
    QToolButton::changeEvent(event);

    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

void CommandButton::retranslateUi()
{
    setToolTip(tr("Execute Command"));
}

void CommandButton::handleReadStandardError()
{
    QByteArray data = cmd->readAllStandardError();
    qDebug() << data;
}

void CommandButton::handleReadStandardOutput()
{
   QByteArray data = cmd->readAllStandardOutput();
   qDebug() << "Data:\n" << data << endl;

}
