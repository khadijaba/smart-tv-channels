#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QSqlQueryModel>
#include "publicite.h"  // Inclure le fichier d'en-tête de la classe Publicite

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_pressed();
    void on_pushButton_2_clicked();
    void on_pushButton_3_pressed();
    void on_pushButton_rechercher_clicked();

    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_afficher_clicked();
    void on_pushButton_ajouter_clicked();

private:
    Ui::MainWindow *ui;

    publicite* rechercherParId(int id);
    void rechercherPubliciteParId(int id);
    void rechercherPubliciteParId_modifier(int id);
    bool supprimerPubliciteParId(int id);
    bool modifierPublicite(int id);

};

#endif // MAINWINDOW_H
