#include "widget.h"
#include "ui_widget.h"

#include <QTextStream>


Widget::Widget(QWidget *parent) : QWidget(parent) {

    widget = new QWidget(this);

    //RADIO BUTTONS
    radio_buttons_layout = new QVBoxLayout;
    rb_if_cur_file = new QRadioButton("Сhoose a file");
    rb_if_new_file = new QRadioButton("Create a new file");
    rb_if_cur_file->setChecked(true);
    radio_buttons_layout->addWidget(rb_if_cur_file);
    radio_buttons_layout->addWidget(rb_if_new_file);
    connect(rb_if_cur_file, &QRadioButton::clicked, this, &Widget::on_rb_if_cur_file_clicked);
    connect(rb_if_new_file, &QRadioButton::clicked, this, &Widget::on_rb_if_new_file_clicked);

    button_open_file = new QPushButton("&Open File");
    connect(button_open_file, &QPushButton::clicked, this, &Widget::on_button_open_file_clicked);
    line_input_file_name = new QLineEdit;

    choose_file_layout = new QVBoxLayout;
    choose_file_layout->addWidget(button_open_file);
    choose_file_layout->addWidget(line_input_file_name);
    line_input_file_name->hide();

    header_layout = new QHBoxLayout;

    header_layout->addLayout(radio_buttons_layout);
    header_layout->addLayout(choose_file_layout);


    //COMBO BOXES
    cb_encrypt_label = new QLabel("Encryption: ");
    cb_hash_label    = new QLabel("Hash: ");
    cb_encrypt       = new QComboBox;
    cb_hashes[0]     = new QComboBox;
    cb_hashes[0]->addItems({"-"});
    cb_hashes[1]     = new QComboBox;
    cb_hashes[1]->addItems({"-", "md_gost94"});
    cb_hashes[2]     = new QComboBox;
    cb_hashes[2]->addItems({"-", "md_gost12_256"});
    cb_hashes[3]     = new QComboBox;
    cb_hashes[3]->addItems({"-", "md_gost12_512"});
    cb_encrypt->addItems({"-", "gost2001", "gost2012_256", "gost2012_512"});
    connect(cb_encrypt, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cb_encrypt_activated(int)));

    cb_encrypt_layout = new QHBoxLayout;
    cb_hash_layout    = new QHBoxLayout;
    QVBoxLayout* hashes_layout = new QVBoxLayout;
    for(int i = 0; i < NUM_ENCRYPT_VARS; i++) {
        hashes_layout->addWidget(cb_hashes[i]);
        if(i)
            cb_hashes[i]->hide();
    }

    cb_encrypt_layout->addWidget(cb_encrypt_label); cb_encrypt_layout->addWidget(cb_encrypt);
    cb_hash_layout->addWidget(cb_hash_label);
    for(int i = 0; i < NUM_ENCRYPT_VARS; i++)
        cb_hash_layout->addWidget(cb_hashes[i]);


    //PERSONAL DATA
    labels[0] = new QLabel("Country Name:");
    labels[1] = new QLabel("State or Province:");
    labels[2] = new QLabel("Locality:");
    labels[3] = new QLabel("Organization:");
    labels[4] = new QLabel("Organization Unit:");
    labels[5] = new QLabel("Common Name:");

    //BUTTON
    button_gen_csr = new QPushButton("ㅤㅤ&TAP TO GENERATE CERTIFICATE REQUESTㅤㅤ");
    connect(button_gen_csr, &QPushButton::clicked, this, &Widget::on_button_gen_csr_clicked);

    //TEXT FIELD
    text_output_csr = new QTextEdit(this);
    text_output_csr->setReadOnly(true);


    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(header_layout);
    mainLayout->addLayout(cb_encrypt_layout);
    mainLayout->addLayout(cb_hash_layout);

    for(int i = 0; i < NUM_LINES; i++) {
        lines[i] = new QLineEdit(this);
        mainLayout->addWidget(labels[i]);
        mainLayout->addWidget(lines[i]);
    }
    mainLayout->addWidget(button_gen_csr);
    mainLayout->addWidget(text_output_csr);

    widget->setLayout(mainLayout);
    widget->show();
}

Widget::~Widget() {}

void Widget::GenerateCsr() {
    QString result;
    bool is_all_data = true;
    std::string command;

    for(int i = 0; i < NUM_LINES; i++) {
        if(lines[i]->text() == "") {
            is_all_data = false;
            lines[i]->setPlaceholderText("Empty!");
        }
        lines[i]->text() == "" ? labels[i]->setStyleSheet("color: rgb(255, 0, 0)") : labels[i]->setStyleSheet("color: rgb(0, 0, 0)");
    }

    if(!(cb_encrypt->currentIndex() && cb_hashes[cb_encrypt->currentIndex()]->currentIndex()))
        is_all_data = false;
    cb_encrypt->currentIndex() ? cb_encrypt_label->setStyleSheet("color: rgb(0, 0, 0)") : cb_encrypt_label->setStyleSheet("color: rgb(255, 0, 0)");
    cb_hashes[cb_encrypt->currentIndex()]->currentIndex() ? cb_hash_label->setStyleSheet("color: rgb(0, 0, 0)")    : cb_hash_label->setStyleSheet("color: rgb(255, 0, 0)");

    if(lines[0]->text().size() != 2 && lines[0]->text().size() != 0) {
        is_all_data = false;
        lines[0]->clear();
        lines[0]->setPlaceholderText("You need to write 2 letters!");
        labels[0]->setStyleSheet("color: rgb(255, 0, 0)");
    }

    if(!is_all_data) {
        text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
        text_output_csr->setText("ERROR: not all data is entered!");
    }
    else {
        std::string certificate_name = "CSR.csr";
        if(rb_if_new_file->isChecked()) {
            file_name = line_input_file_name->text();
            command = "openssl genpkey -algorithm " + cb_encrypt->currentText().toStdString() + " -pkeyopt paramset:A -out " + file_name.toStdString();
            if(system(command.c_str())) {
                text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
                text_output_csr->setText("ERROR: failed to generate private key!");
            }
        }
        command = "openssl req -x509 -" + cb_hashes[cb_encrypt->currentIndex()]->currentText().toStdString() + " -new -key " + file_name.toStdString() + " -out " + certificate_name + " -subj \"" +
                "/C="  + lines[0]->text().toStdString() +
                "/ST=" + lines[1]->text().toStdString() +
                "/L="  + lines[2]->text().toStdString() +
                "/O="  + lines[3]->text().toStdString() +
                "/OU=" + lines[4]->text().toStdString() +
                "/CN=" + lines[5]->text().toStdString() + "\"";
        if(system(command.c_str())) {
            text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
            text_output_csr->setText("ERROR: failed to generate certificate!");
        }
        else {
            QFile file(QString::fromStdString(certificate_name));
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                result = in.readAll();
                file.close();
            }

            else {
                text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
                text_output_csr->setText("ERROR: failed to open certificate file!");
            }
        }
    }
    text_output_csr->setStyleSheet("color: rgb(0, 0, 0)");
    text_output_csr->setText(result);

}

void Widget::on_rb_if_cur_file_clicked(bool checked) {
    if(checked) {
        line_input_file_name->hide();
        button_open_file->show();
    }
}

void Widget::on_rb_if_new_file_clicked(bool checked) {
    if(checked) {
        line_input_file_name->show();
        button_open_file->hide();
    }
}

void Widget::on_cb_encrypt_activated(int index) {
    cb_hashes[index]->show();
    cb_hashes[(index + 1) % 4]->hide();
    cb_hashes[(index + 2) % 4]->hide();
    cb_hashes[(index + 3) % 4]->hide();
}

void Widget::on_button_open_file_clicked() {
    file_name = QFileDialog::getOpenFileName(this, "Open File", "/home/mukhamed/");
}

void Widget::on_button_gen_csr_clicked() {
    if(rb_if_cur_file->isChecked()) {
        if(file_name.isEmpty()) {
            button_open_file->setStyleSheet("color: rgb(255, 0, 0)");
            text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
            text_output_csr->setText("ERROR: private key file not selected!");
        }

        else {
            button_open_file->setStyleSheet("color: rgb(0, 0, 0)");
            GenerateCsr();
        }
    }
    else {
        if(line_input_file_name->text() == "") {
            line_input_file_name->setPlaceholderText("Empty!");
            text_output_csr->setStyleSheet("color: rgb(255, 0, 0)");
            text_output_csr->setText("ERROR: new file name of private key not entered!");
        }
        else {
            line_input_file_name->setPlaceholderText("");
            GenerateCsr();
        }
    }
}



















