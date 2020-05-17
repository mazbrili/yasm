#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QShortcut>
#include <QKeySequence>
#include <QFileInfo>

#include "snippet.h"
#include "optionsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void firstTimeInitializeGUI();
    void setDefaultEditorConfiguration();
    void enableGUI();
    void disableGUI();
    void adjustGUIForSnippetCreation(QString);
    void logListWidgetSnippets();
    int createDbConnection();
    int getMaxIdFromDb();
    bool isCategoryAlreadyOnDb(QString);
    bool thereIsSomeCategoryOnDb();
    bool thereIsSomeSnippetOnDb(QString);
    void clearUi();
    void displaySnippets();
    void detectIfClipboardHasSomething();
    void retrieveDataFromDb();
    void openAnotherDb();
    QString getLastCategoryOnDb();
    void checkDefaultDbDirectory();
    void applySelectedTheme();
    void applySelectedFont();
    void applySelectedFontSize();
    void enableWordWrap(bool);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    int snippetId;
    OptionsDialog optionsDialog;
    QString dbFilePath;
    QString lastCategoryOnDb;
    bool isRetrievingDataFromDb;

private slots:
    void on_pushButtonNewCategory_clicked();
    void on_pushButtonRemoveCategory_clicked();
    void on_pushButtonNewSnippet_clicked();
    void on_pushButtonRemoveSnippet_clicked();
    void on_listWidgetCategories_itemSelectionChanged();
    void on_listWidgetSnippets_itemSelectionChanged();
    void on_lineEditSnippetTitle_textChanged();
    void on_textEditSnippetContent_textChanged();
    void on_actionExit_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionOptions_triggered();
    void on_actionAbout_Yasm_triggered();
    void on_lineEditSnippetTitle_selectionChanged();
    void on_textEditSnippetContent_selectionChanged();
    void on_clipboard_contentChanged();
    void on_shortcut_openDb_pressed();
};
#endif // MAINWINDOW_H
