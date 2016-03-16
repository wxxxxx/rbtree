#include <QCoreApplication>
#include "rbtree-define.h"
#include <QTextCodec>
#include <QtCore>
#include <QByteArray>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);

    int a[]= {10, 40, 30, 60, 90, 70, 20, 50, 80};
    int check_insert = 0;
    int check_remove = 0;
    int i;
    int ilen = (sizeof(a)) / (sizeof(a[0])) ;
    RBTree<int>* tree = new RBTree<int>();

    cout<< "== init-array:"<<'\t';
    for(i = 0; i < ilen ; i++)
        cout<< a[i] << " ";
    cout<< endl;

    for(i = 0; i < ilen ; i++)
    {
        tree->insert(a[i]);
        if(check_insert)
        {
            cout<< "== add a[i]:" <<'\t'<< a[i] << endl;
            cout<< "== all-information for tree:" << endl;
            tree->print();
            cout<<endl;
        }
    }

    cout<< "== preOrder: "<<'\t';
    tree->preOrder();
    cout<<endl;
    cout<< "== inOrder: "<<'\t';
    tree->inOrder();
    cout<<endl;
    cout<< "== postOrder: "<<'\t';
    tree->postOrder();
    cout<<endl;

    cout<< "== min value: " << tree->minimum() << endl;
    cout<< "== max value: " << tree->maximum() << endl;
    cout<< "== all-information for tree: " << endl;
    tree->print();

    if(check_remove)
    {
        for(i = 0 ; i < ilen ; i++)
        {
            tree->remove(a[i]);

            cout<< "== 删除节点: " << a[i] <<endl;
            cout<< "== 树的详细信息: " << endl;
            tree->print();
            cout<< endl;
        }
    }

    tree->destroy();

    return app.exec();
}

