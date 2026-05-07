$filepath = 'c:\Users\MSI\Documents\projetcpp2526-s2-2a14-smart-leather-goods-factory-1-integration\fournisseurs.ui'
$content = [System.IO.File]::ReadAllText($filepath, [System.Text.Encoding]::UTF8)

$sidebar = @"
   <widget class="QGroupBox" name="sidebarGroup">
    <property name="geometry">
     <rect>
      <x>0</x>
      <y>0</y>
      <width>210</width>
      <height>740</height>
     </rect>
    </property>
    <property name="title">
     <string/>
    </property>
    <property name="styleSheet">
     <string notr="true">QGroupBox {
    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2c1a0e, stop:1 #3d2512);
    border: none;
    border-radius: 0px;
}
QPushButton {
    background-color: transparent;
    border: none;
    color: #d4c4b0;
    font-size: 13px;
    font-weight: 600;
    text-align: left;
    padding: 14px 20px;
}
QPushButton:hover {
    background-color: rgba(255,255,255,0.1);
    color: #ffffff;
}
QPushButton#pushButton_fournisseurs_label {
    background-color: rgba(255,200,100,0.15);
    color: #ffffff;
    border-left: 4px solid #c47a2c;
}
QPushButton#pushButton_16 { color: #e0a0a0; }
QPushButton#pushButton_16:hover { background-color: rgba(200,50,50,0.2); }</string>
    </property>
    <widget class="QLabel" name="sidebarLogo">
     <property name="geometry"><rect><x>15</x><y>15</y><width>180</width><height>90</height></rect></property>
     <property name="pixmap"><pixmap resource="logo.qrc">:/Logo.png</pixmap></property>
     <property name="scaledContents"><bool>true</bool></property>
    </widget>
    <widget class="QPushButton" name="pushButton_15">
     <property name="geometry"><rect><x>0</x><y>120</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Employés</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_21">
     <property name="geometry"><rect><x>0</x><y>175</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Produits</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_20">
     <property name="geometry"><rect><x>0</x><y>230</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Commandes</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_fournisseurs_label">
     <property name="geometry"><rect><x>0</x><y>285</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Fournisseurs</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_22">
     <property name="geometry"><rect><x>0</x><y>340</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Matières</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_23">
     <property name="geometry"><rect><x>0</x><y>395</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Machines</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_16">
     <property name="geometry"><rect><x>0</x><y>650</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Déconnexion</string></property>
    </widget>
   </widget>
"@

$needle = '  <widget class="QMenuBar" name="menubar">'
if ($content.Contains($needle)) {
    $content = $content.Replace($needle, $sidebar + "`r`n" + $needle)
    
    # Also shift all <x> values by 210
    $content = [regex]::Replace($content, '(?s)<widget(?! class="QGroupBox" name="sidebarGroup").*?<x>(\d+)</x>', {
        param($m)
        $val = [int]$m.Groups[1].Value
        if ($val -lt 1100) {
            $newVal = $val + 210
            return $m.Value -replace ('<x>' + $val + '</x>'), ('<x>' + $newVal + '</x>')
        }
        return $m.Value
    })

    [System.IO.File]::WriteAllText($filepath, $content, [System.Text.Encoding]::UTF8)
    Write-Host "SUCCESS"
} else {
    Write-Host "ERROR: Needle not found"
}
