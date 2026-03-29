#include "commandes.h"
#include "stats.h"

void commandes::on_pushButton_9_clicked() {
    Stats dlg(this);
    dlg.exec();
}

