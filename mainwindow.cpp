#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "publicite.h"
#include <QMessageBox>
#include <QDate>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_rechercher, SIGNAL(clicked()), this, SLOT(on_pushButton_rechercher_clicked()));
    connect(ui->pushButton_supprimer, SIGNAL(clicked()), this, SLOT(on_pushButton_supprimer_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

void MainWindow::on_pushButton_3_pressed()
{

}
void MainWindow::on_pushButton_2_pressed()
{
    close();

}
void MainWindow::on_pushButton_ajouter_clicked()
{
    publicite p;
    QString TS = ui->combobox->currentText();
    QString firstChoice = ui->combobox->itemText(0);

    if (ui->lineedit_compagnie->text().isEmpty() ||

        ui->lineedit_budget->text().isEmpty() ||
        (!ui->dateedit_db->date().isValid() || !ui->dateedit_fn->date().isValid()) ||
       TS == firstChoice ||
        ui->lineedit_DUREE->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Tous les champs sont obligatoires!", QMessageBox::Ok);
        return; // Stop further execution
    }

    p.setCOMPAGNIE(ui->lineedit_compagnie->text());
    p.setBUDGET(ui->lineedit_budget->text().toDouble());
    p.setDate_DB(ui->dateedit_db->date());
    p.setDATE_FN(ui->dateedit_fn->date());
    p.setTYPE(TS);
    p.setDUREE(ui->lineedit_DUREE->text().toDouble());
    p.setID_PUBLICITE(ui->lineedit_ID_PUBLICITE->text().toDouble());

    if (p.ajouter()) {
        QMessageBox::information(this, "Message", "Publicité ajoutée", QMessageBox::Ok);
    }
}

publicite *MainWindow::rechercherParId(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM PUBLICITE WHERE ID_PUBLICITE = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        int ID_PUBLICITE=query.value("ID_PUBLICITE").toInt();
        QString compagnie = query.value("COMPAGNIE").toString();
        double budget = query.value("BUDGET").toDouble();
        QDate date_db = query.value("DATE_DB").toDate();
        QDate date_fn = query.value("DATE_FN").toDate();
        QString type = query.value("TYPE").toString();
        double duree = query.value("DUREE").toDouble();
        QString frequence = query.value("FREQUENCE").toString();
        // Vous pouvez récupérer et initialiser FREQUENCE ici si nécessaire.

        // Créer un nouvel objet publicite avec les informations récupérées
        publicite *p = new publicite(ID_PUBLICITE,compagnie, budget, date_db, date_fn, type, duree, frequence);
        return p;
    } else {
        // En cas d'erreur ou si la publicité n'est pas trouvée, retourner nullptr ou gérer l'erreur selon vos besoins
        return nullptr;
    }
}

void MainWindow::rechercherPubliciteParId(int id)
{
    publicite *p = rechercherParId(id);

    if (p != nullptr) {
        // Afficher les informations de la publicité dans les lineEdits
        ui->lineedit_compagnie->setText(p->get_COMPAGNIE());
        ui->lineedit_budget->setText(QString::number(p->get_BUDGET()));
        ui->dateedit_db->setDate(p->get_DATE_DB());
        ui->dateedit_fn->setDate(p->get_DATE_FN());
        ui->combobox->setCurrentText(p->get_TYPE());
        ui->lineedit_DUREE_2->setText(QString::number(p->get_DUREE()));
        ui->lineedit_FREQUENCE->setText(p->get_FREQUENCE());

        delete p; // Assurez-vous de libérer la mémoire une fois que vous avez fini d'utiliser l'objet
    } else {
        // Gérer le cas où la publicité n'est pas trouvée
        QMessageBox::warning(this, "Avertissement", "Publicité non trouvée!", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_rechercher_clicked()
{
    int id = ui->lineedit_rechid->text().toInt();
    rechercherPubliciteParId(id);
}

bool MainWindow::supprimerPubliciteParId(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PUBLICITE WHERE ID_PUBLICITE = :id");
    query.bindValue(":id", id);
    return query.exec();
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineedit_25->text().toInt();
    if (supprimerPubliciteParId(id)) {
        QMessageBox::information(this, "Information", "Publicité supprimée avec succès!", QMessageBox::Ok);
        ui->lineedit_25->clear();

    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de la publicité!", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_afficher_clicked()
{
    publicite p;
    QSqlQueryModel *model = p.afficher();

    if (model != nullptr) {
        ui->tableView_afficher->setModel(model);
        ui->tableView_afficher->resizeColumnsToContents();
    }
}
bool MainWindow::modifierPublicite(int id)
{
    publicite *p = rechercherParId(id);

    if (p!= nullptr) {
        p->setID_PUBLICITE(ui->lineedit_rechid->text().toInt());
        p->setCOMPAGNIE(ui->lineedit_COMPAGNIE_2->text());
        p->setBUDGET(ui->lineedit_BUDGET_2->text().toInt());
        ui->combobox_2->setCurrentText(p->get_TYPE());
        p->setDate_DB(ui->dateedit_db->date());
        p->setDATE_FN(ui->dateedit_fn->date());
        p->setFREQUENCE(ui->lineedit_FREQUENCE_2->text());
        p->setDUREE(ui->lineedit_DUREE_2->text().toInt());


        bool success = p->modifier(id);

        if (success) {
            QMessageBox::information(this, "Information", "Informations de l'Equipement modifiées avec succès!", QMessageBox::Ok);
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification des informations de l'Equipement!", QMessageBox::Ok);
        }

        delete p; // Assurez-vous de libérer la mémoire une fois que vous avez fini d'utiliser l'objet
        return success;
    } else {
        // Gérer le cas où l'equipement n'est pas trouvé
        QMessageBox::warning(this, "Avertissement", "Equipement non trouvé!", QMessageBox::Ok);
        return false;
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    int id = ui->lineedit_rechid->text().toInt();
    modifierPublicite(id);
}


