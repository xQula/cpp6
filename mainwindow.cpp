#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

   race1 = new Controller(&m);
   race2 = new Controller(&m);

   concurRace1 = new ExampleRace(&m);
   concurRace2 = new ExampleRace(&m);

    //Сигналы по завершению работы потоков
    connect(race1, &Controller::sig_WorkFinish, [&](){
        //отображаем только когда второй поток закончит работу
        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                  QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }

    });

    connect(race2, &Controller::sig_WorkFinish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }

    });


    connect(concurRace1, &ExampleRace::sig_Finish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }
    });

    connect(concurRace2, &ExampleRace::sig_Finish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Метод запускает два потока
void MainWindow::StartRace(void){


    if(ui->rb_qtConcur->isChecked()){

        //ui->te_debug->append("Выполни ДЗ!");
        //Тут должен быть код ДЗ
        // тут 2 варианта черз метод then или ждать цикла
        auto k = [this](){return this->concurRace1->DoWork(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());};
        auto v = [this](){return this->concurRace1->DoWork(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());};
        //auto k = [this](){return this->concurRace1->DoWork(&number, true, ui->sb_initNum->value());};
        //auto v = [this](){return this->concurRace1->DoWork(&number, true, ui->sb_initNum->value());};
        auto i =  QtConcurrent::run(k);
        //QtConcurrent::run(k).then(v);
        while(i.isFinished() == false){}
        auto a = QtConcurrent::run(v);

        // прерыл интенет, но так и не понял как связать в коде ниже
        // вылетает ошибка
        //  In template: type 'decay_t<ExampleRace **>' (aka 'ExampleRace **') cannot be used prior to '::' because it has no members
        // и второе что не понятно
        // как методы
        //  ui->rb_mutexOn->isChecked, ui->sb_initNum->value ипользовать без доп переменных
        //auto var1 = ui->rb_mutexOn->isChecked();
        //auto var2 = ui->sb_initNum->value();
        // QtConcurrent::run(concurRace1, &ExampleRace::DoWork, var1, var2);
        //QtConcurrent::run(&this->concurRace2, &ExampleRace::DoWork, &number, var1, var2);

    }
    else{
       emit race1->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
       emit race2->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
    }
}

//Обработка кнопки "Старт"
void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    countFinish = 0;
    number = 0;
    StartRace( );
}
