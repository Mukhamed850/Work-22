#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_button_gen_csr_clicked();
    void on_rb_if_cur_file_clicked(bool checked);
    void on_rb_if_new_file_clicked(bool checked);
    void on_cb_encrypt_activated(int index);
    void on_button_open_file_clicked();

private:
    const int NUM_LINES = 6;
    const int NUM_ENCRYPT_VARS = 4;
    QString file_name = "";

    QWidget* widget;
    QWidget* choose_file_widget;

    QVBoxLayout* mainLayout;
    QHBoxLayout* cb_encrypt_layout;
    QHBoxLayout* cb_hash_layout;
    QVBoxLayout* radio_buttons_layout;
    QVBoxLayout* choose_file_layout;
    QHBoxLayout* header_layout;

    QRadioButton* rb_if_cur_file;
    QRadioButton* rb_if_new_file;

    QPushButton* button_open_file;

    QLineEdit* line_input_file_name;

    QLabel* cb_encrypt_label; QComboBox* cb_encrypt;
    QLabel* cb_hash_label;    QComboBox** cb_hashes = new QComboBox*[NUM_ENCRYPT_VARS];

    QLineEdit** lines = new QLineEdit*[NUM_LINES];
    QLabel** labels   = new QLabel*[NUM_LINES];

    QPushButton* button_gen_csr;

    QTextEdit* text_output_csr;

    void GenerateCsr();
};

#endif // WIDGET_H
