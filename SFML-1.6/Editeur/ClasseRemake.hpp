#ifndef CLASSEREMAKE_HPP
#define CLASSEREMAKE_HPP

#include <iostream>
#include <QtGui>

/*************************************************************
toutes ces classes font exactement la même chose que
celle de Qt, saut qu'elle emettent la valeur index
(passé en parametre lors de la construction)
lorsqu'elle sont changer. Cela peu etre partique pour
connaitre la case du tableau dans lequelle elles sont.
**************************************************************/

//Edition d'une simple ligne
class QLineEditIndex : public QLineEdit
{
  Q_OBJECT

  public:

  QLineEditIndex(int index,char* text,QWidget * parent=0) : QLineEdit(parent)
  {
      this->index=index;
      setText(text);
      QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(slotTextChange()));
  };

  public slots:
        void slotTextChange() {emit EmitIndex(index);};

        signals:
    void EmitIndex(int index);

  private:
    int index;

};


//edition multi ligne
class QTextEditIndex : public QTextEdit
{
    Q_OBJECT

    public:

    QTextEditIndex(int index,char* text,QWidget* parent=0) : QTextEdit(parent)
    {
        this->index=index;
        setPlainText(text);
        QObject::connect(this,SIGNAL(textChanged()),this,SLOT(slotTextChange()));
    }

    public slots:
        void slotTextChange() {emit EmitIndex(index);};

        signals:
    void EmitIndex(int index);

  private:
    int index;
};

//boite de choix d'entier
class QSpinBoxIndex : public QSpinBox
{
  Q_OBJECT

  public:

  QSpinBoxIndex(int index,int i,QWidget * parent=0) : QSpinBox(parent)
  {
      this->index=index;
      setValue(i);
      QObject::connect(this,SIGNAL(valueChanged(int)),this,SLOT(slotValueChange()));
  };

  public slots:
        void slotValueChange() {emit EmitIndex(index);};

        signals:
    void EmitIndex(int index);

  private:
    int index;

};

//Combobox basique
class QComboBoxIndex : public QComboBox
{
  Q_OBJECT

  public:

  QComboBoxIndex(int index,QWidget * parent=0) : QComboBox(parent)
  {
      this->index=index;
      QObject::connect(this,SIGNAL(	currentIndexChanged(int)),this,SLOT(slotValueChange()));
  };

  public slots:
        void slotValueChange() {emit EmitIndex(index);};

        signals:
    void EmitIndex(int index);

  private:
    int index;

};

#endif
