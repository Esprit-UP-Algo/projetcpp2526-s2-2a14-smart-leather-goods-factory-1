#include "simplepayserver.h"
#include <QNetworkInterface>
#include <QHostAddress>

// ─────────────────────────────────────────────────────────────────────────────
SimplePayServer::SimplePayServer(const QString &ref,
                                 const QString &amount,
                                 const QString &clientName,
                                 QObject *parent)
    : QTcpServer(parent), m_ref(ref), m_amount(amount), m_clientName(clientName)
{
    connect(this, &QTcpServer::newConnection, this, &SimplePayServer::handleConnection);
}

bool SimplePayServer::startListening()
{
    // Essaie d'abord des ports fixes connus (plus facile à autoriser dans le pare-feu Windows).
    // Si tous sont occupés, laisse l'OS choisir avec le port 0.
    const QList<quint16> candidates = {8080, 8181, 8282, 8383, 9090};
    for (quint16 port : candidates) {
        if (listen(QHostAddress::Any, port))
            return true;
        close(); // libère avant de réessayer
    }
    return listen(QHostAddress::Any, 0); // fallback : port aléatoire
}

QString SimplePayServer::url() const
{
    return QString("http://%1:%2").arg(localIp()).arg(serverPort());
}

// ── Serve a page for every incoming connection ───────────────────────────────
void SimplePayServer::handleConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    if (!socket) return;

    // Wait until we have the request (or 2 seconds max)
    socket->waitForReadyRead(2000);
    socket->readAll(); // discard the request — we always serve the same page

    QByteArray body = buildPage().toUtf8();

    QString header = QString(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %1\r\n"
        "Connection: close\r\n"
        "\r\n").arg(body.size());

    socket->write(header.toUtf8());
    socket->write(body);
    socket->flush();
    socket->disconnectFromHost();
    socket->deleteLater();
}

// ── Build the payment page HTML ───────────────────────────────────────────────
QString SimplePayServer::buildPage() const
{
    // CSS
    QString css =
        "* { box-sizing:border-box; margin:0; padding:0; }"
        "body { min-height:100vh;"
        "  background:linear-gradient(135deg,#2c1a0e,#5b2f1d,#8b4513);"
        "  display:flex; align-items:center; justify-content:center;"
        "  font-family:'Segoe UI',Arial,sans-serif; padding:20px; }"
        ".card { background:linear-gradient(160deg,#fffaf5,#f4e8d8);"
        "  border-radius:22px; box-shadow:0 20px 60px rgba(0,0,0,.5);"
        "  max-width:420px; width:100%; overflow:hidden; }"
        ".hdr { background:linear-gradient(135deg,#4a2717,#8b4513);"
        "  padding:24px 28px 18px; color:#fffaf5; }"
        ".brand { font-size:20px; font-weight:800; }"
        ".brand b { color:#f0c060; }"
        ".ref-badge { display:inline-block; margin-top:10px;"
        "  background:rgba(255,255,255,.18); border-radius:20px;"
        "  padding:3px 14px; font-size:13px; }"
        ".bdy { padding:24px 28px 28px; }"
        ".amtbox { background:linear-gradient(135deg,#6b3e26,#4a2717);"
        "  border-radius:12px; padding:16px 20px; color:#fffaf5; margin-bottom:20px; }"
        ".amtlbl { font-size:11px; opacity:.7; margin-bottom:4px; }"
        ".amtval { font-size:30px; font-weight:800; color:#f0c060; }"
        ".cli { font-size:13px; opacity:.8; margin-top:4px; }"
        ".sec { font-size:11px; font-weight:700; color:#8b4513;"
        "  letter-spacing:1.5px; text-transform:uppercase; margin:18px 0 10px; }"
        ".fld { margin-bottom:14px; }"
        "label { display:block; font-size:12px; font-weight:700;"
        "  color:#5b2f1d; margin-bottom:5px; }"
        "input { width:100%; padding:11px 13px;"
        "  border:2px solid #d4c0b0; border-radius:9px;"
        "  font-size:15px; color:#2c1a0e; background:#fffaf5; outline:none; }"
        "input:focus { border-color:#8b4513; }"
        ".row { display:grid; grid-template-columns:1fr 1fr; gap:10px; }"
        ".btn { width:100%; padding:14px;"
        "  background:linear-gradient(135deg,#c4890a,#8b4513);"
        "  color:#fff; border:none; border-radius:11px;"
        "  font-size:15px; font-weight:800; cursor:pointer; margin-top:6px; }"
        ".note { text-align:center; font-size:11px; color:#8b6b55; margin-top:12px; }"
        ".err { color:#c0392b; font-size:11px; margin-top:4px; display:none; }"
        "#ok { display:none; text-align:center; padding:30px 20px; }"
        ".ok-icon { font-size:60px; margin-bottom:12px; }"
        ".ok-ttl { font-size:22px; font-weight:800; color:#2c6b2f; }"
        ".ok-txt { color:#5b3a29; font-size:13px; margin-top:10px; line-height:1.6; }";

    // JavaScript
    QString js =
        "function fmtCard(e){"
        "  var v=e.value.replace(/\\D/g,'').substring(0,16);"
        "  e.value=v.replace(/(.{4})/g,'$1 ').trim();}"
        "function fmtExp(e){"
        "  var v=e.value.replace(/\\D/g,'');"
        "  if(v.length>=3) v=v.substring(0,2)+'/'+v.substring(2,4);"
        "  e.value=v;}"
        "function luhn(n){"
        "  var s=0,a=false;"
        "  for(var i=n.length-1;i>=0;i--){"
        "    var d=+n[i];if(a){d*=2;if(d>9)d-=9;}s+=d;a=!a;}"
        "  return s%10===0;}"
        "function pay(){"
        "  var c=document.getElementById('cn').value.replace(/\\s/g,'');"
        "  var er=document.getElementById('er');"
        "  if(c.length<12||!luhn(c)){er.style.display='block';return;}"
        "  er.style.display='none';"
        "  var x=document.getElementById('exp').value;"
        "  var v=document.getElementById('cvv').value;"
        "  var h=document.getElementById('holder').value;"
        "  if(!x||v.length<3||h.length<2){alert('Remplissez tous les champs.');return;}"
        "  document.getElementById('form').style.display='none';"
        "  document.getElementById('ok').style.display='block';"
        "  document.getElementById('last4').textContent=c.slice(-4);}";

    // HTML body
    QString html;
    html += "<!DOCTYPE html><html lang=\"fr\"><head>";
    html += "<meta charset=\"utf-8\"/>";
    html += "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"/>";
    html += "<title>Paiement SmartLeather</title>";
    html += "<style>" + css + "</style></head><body>";
    html += "<div class=\"card\">";
    // Header
    html += "<div class=\"hdr\">";
    html += "<div class=\"brand\">Smart<b>Leather</b></div>";
    html += "<div class=\"ref-badge\">Commande : " + m_ref + "</div>";
    html += "</div>";
    // Body
    html += "<div class=\"bdy\">";
    // Form screen
    html += "<div id=\"form\">";
    html += "<div class=\"amtbox\">";
    html += "<div class=\"amtlbl\">Montant &agrave; r&eacute;gler</div>";
    html += "<div class=\"amtval\">" + m_amount + " TND</div>";
    html += "<div class=\"cli\">" + m_clientName + "</div>";
    html += "</div>";
    html += "<div class=\"sec\">Informations de carte</div>";
    html += "<div class=\"fld\"><label>Titulaire</label>";
    html += "<input id=\"holder\" placeholder=\"Mohamed Ben Ali\"/></div>";
    html += "<div class=\"fld\"><label>Num&eacute;ro de carte</label>";
    html += "<input id=\"cn\" placeholder=\"1234 5678 9012 3456\" maxlength=\"19\" oninput=\"fmtCard(this)\"/>";
    html += "<div class=\"err\" id=\"er\">Num&eacute;ro invalide</div></div>";
    html += "<div class=\"row\">";
    html += "<div class=\"fld\"><label>Expiration</label>";
    html += "<input id=\"exp\" placeholder=\"MM/AA\" maxlength=\"5\" oninput=\"fmtExp(this)\"/></div>";
    html += "<div class=\"fld\"><label>CVV</label>";
    html += "<input id=\"cvv\" placeholder=\"123\" maxlength=\"4\" type=\"password\"/></div>";
    html += "</div>";
    html += "<button class=\"btn\" onclick=\"pay()\">Payer " + m_amount + " TND</button>";
    html += "<div class=\"note\">Paiement simul&eacute; - SmartLeather ERP</div>";
    html += "</div>"; // #form
    // Success screen
    html += "<div id=\"ok\">";
    html += "<div class=\"ok-icon\">&#10004;</div>";
    html += "<div class=\"ok-ttl\">Paiement confirm&eacute; !</div>";
    html += "<div class=\"ok-txt\">";
    html += "Commande <strong>" + m_ref + "</strong><br/>";
    html += "Montant : <strong>" + m_amount + " TND</strong><br/><br/>";
    html += "Carte se terminant par &bull;&bull;&bull;&bull; <strong id=\"last4\"></strong><br/><br/>";
    html += "Merci - SmartLeather";
    html += "</div></div>"; // #ok
    html += "</div></div>"; // .bdy .card
    html += "<script>" + js + "</script>";
    html += "</body></html>";

    return html;
}

// ── Retourne toutes les IPs locales non-loopback disponibles ─────────────────
QStringList SimplePayServer::allLocalIps()
{
    QStringList wifi, others;
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
        if (!iface.flags().testFlag(QNetworkInterface::IsUp))      continue;
        if (!iface.flags().testFlag(QNetworkInterface::IsRunning)) continue;
        if ( iface.flags().testFlag(QNetworkInterface::IsLoopBack)) continue;

        const QString name = iface.humanReadableName().toLower();
        const bool isWifi  = name.contains("wi-fi")    || name.contains("wifi")
                          || name.contains("wlan")     || name.contains("wireless")
                          || name.contains("802.11");

        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            QHostAddress addr = entry.ip();
            if (addr.protocol() != QAbstractSocket::IPv4Protocol) continue;
            const QString s = addr.toString();
            // Exclure loopback et link-local (169.254.x.x)
            if (s == "127.0.0.1" || s.startsWith("169.254."))   continue;
            if (isWifi) wifi.prepend(s);   // Wi-Fi en priorité
            else        others.append(s);
        }
    }
    return wifi + others;
}

// ── Choisit la meilleure IP locale (préférence Wi-Fi) ─────────────────────────
QString SimplePayServer::localIp()
{
    const QStringList all = allLocalIps();
    return all.isEmpty() ? QStringLiteral("127.0.0.1") : all.first();
}
