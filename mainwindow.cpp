#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snippet.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	createDBConnection();
	snippetId = getMaxIdFromDb();
	firstTimeInitializeGUI();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::firstTimeInitializeGUI()
{
	ui->pushButtonNewSnippet->setText("New Snippet");
	ui->pushButtonRemoveSnippet->setText("Remove Snippet");
	this->setWindowTitle("YASM0 v0.2 (Alpha)");

	ui->pushButtonRemoveSnippet->setEnabled(false);	
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
	
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("SELECT * FROM 'snippets';"))
	{
		qDebug() << "Reading all snippets from the db";
	}
	else
	{
		qWarning() << "Can't read the snippets from the db";
	}
}

void MainWindow::enableGUI()
{
	ui->pushButtonRemoveSnippet->setEnabled(true);
	ui->listWidgetSnippets->setEnabled(true);
	ui->lineEditSnippetTitle->setEnabled(true);
	ui->textEditSnippetContent->setEnabled(true);
}

void MainWindow::disableGUI()
{
	ui->pushButtonRemoveSnippet->setEnabled(false);
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
}

void MainWindow::on_pushButtonNewSnippet_clicked()
{
	snippetId++;
	Snippet *newItem = new Snippet(snippetId);
	newItem->setText(newItem->getTitle());
	
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("INSERT INTO 'snippets' ('id', 'title', 'snippet') VALUES (NULL, '" + newItem->getTitle() + "', '" + newItem->getContent() + "');"))
	{
		qDebug() << "New snippet inserted in table snippets";
	}
	else
	{
		qWarning() << "Can't insert a snippet in table snippets";
	}
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	
	enableGUI();
}

void MainWindow::on_pushButtonRemoveSnippet_clicked()
{
	if (ui->listWidgetSnippets->selectedItems().count() > 0 && ui->listWidgetSnippets->currentRow() >= 0)
	{
		Snippet *removedItem = (Snippet *) ui->listWidgetSnippets->takeItem(ui->listWidgetSnippets->currentRow());
		qDebug() << "removed item id=[" << removedItem->getId() << "]";
		
		if (ui->listWidgetSnippets->count() > 0)
		{
			QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
			Snippet *selectedSnippet = (Snippet *) selectedItem;
			ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
			ui->textEditSnippetContent->setText(selectedSnippet->getContent());
		}
		else if (ui->listWidgetSnippets->count() == 0)
		{
			ui->lineEditSnippetTitle->setText("");
			ui->textEditSnippetContent->setText("");
			disableGUI();
		}
	}
}

void MainWindow::on_listWidgetSnippets_clicked()
{
	QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
	Snippet *selectedSnippet = (Snippet *) selectedItem;
	
	ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
	ui->textEditSnippetContent->setText(selectedSnippet->getContent());
	qDebug() << "Snippet selected [id=" << selectedSnippet->getId() << "]";
}

void MainWindow::on_lineEditSnippetTitle_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 0)
	{
		Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
		selectedSnippetId = selectedSnippet->getId();
		
		selectedSnippet->setTitle(ui->lineEditSnippetTitle->text());
		ui->listWidgetSnippets->selectedItems().at(0)->setText(selectedSnippet->getTitle());	
	}
}

void MainWindow::on_textEditSnippetContent_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 0)
	{
		Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
		selectedSnippetId = selectedSnippet->getId();
		
		selectedSnippet->setContent(ui->textEditSnippetContent->toPlainText());
	}
}

void MainWindow::logListWidgetSnippets()
{
	qDebug() << "Elements on listWidgetSnippets=[" << ui->listWidgetSnippets->count() << "]";
}

void MainWindow::createDBConnection()
{
	qDebug() << "Connecting with the database";
	
	const QString DRIVER("QSQLITE");	
	
	if (QSqlDatabase::isDriverAvailable)
	{
		qDebug() << "QSQLITE driver is available";
	}
	
	QString dbFolder = "C:\\tmp\\";
	QDir dbDirectory(dbFolder);
	QString sqliteFileName = "snippets.db";
	QString dbURI = dbFolder + sqliteFileName;
	
	db = QSqlDatabase::addDatabase(DRIVER);
	db.setDatabaseName(dbURI);
	
	if (!dbDirectory.exists())
	{
		dbDirectory.mkdir(".");
		
		QMessageBox::StandardButton notification;
		notification = QMessageBox::information(this, "New Database Created", "A new directory and a new database\nhas beencreated");
	}
	
	if (db.open())
	{
		qDebug() << "The db has been opened";
	}
	else
	{
		qWarning() << "Can't open db";
	}
	
	QSqlQuery sqlQuery;
	if (sqlQuery.exec("CREATE TABLE IF NOT EXISTS 'snippets' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'title' TEXT, 'snippet' TEXT, 'datetime' DATETIME DEFAULT CURRENT_TIMESTAMP)"))
	{
		qDebug() << "Table snippets has been created";
	}
	else
	{
		qWarning() << "Can't create table snippets";
	}
}

int MainWindow::getMaxIdFromDb()
{
	int maxId = -1;
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("SELECT MAX(id) FROM 'snippets';"))
	{
		if(sqlQuery.next())
		{
			qDebug() << "Selecting the max id from db";
			maxId = sqlQuery.value(0).toInt();
		}
	}
	else
	{
		qWarning() << "Can't selecte the max id from db";
	}
	
	return maxId;
}
