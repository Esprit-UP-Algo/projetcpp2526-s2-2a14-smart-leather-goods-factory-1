$filepath = 'c:\Users\MSI\Documents\projetcpp2526-s2-2a14-smart-leather-goods-factory-1-integration\fournisseurs.ui'
$content = [System.IO.File]::ReadAllText($filepath, [System.Text.Encoding]::UTF8)

$sidebar = @"
   <widget class="QGroupBox" name="sidebarGroup">
    <property name="geometry">
     <rect>
      <x>0</x>
      <y>0</y>
      <width>210</width>
      <height>713</height>
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
    margin-top: 0px;
    padding: 0px;
}
QPushButton {
    background-color: transparent;
    border: none;
    border-radius: 0px;
    color: #d4c4b0;
    font-size: 13px;
    font-weight: 600;
    text-align: left;
    padding: 14px 20px;
}
QPushButton:hover {
    background-color: rgba(255,255,255,0.10);
    color: #ffffff;
}
QPushButton#pushButton_fournisseurs_label {
    background-color: rgba(255,200,100,0.18);
    color: #ffffff;
    border-left: 4px solid #c47a2c;
    font-weight: bold;
}
QPushButton#pushButton_16 { color: #e0a0a0; }
QPushButton#pushButton_16:hover { background-color: rgba(200,50,50,0.2); color: #ff8888; }</string>
    </property>
    <widget class="QLabel" name="sidebarLogo">
     <property name="geometry"><rect><x>15</x><y>10</y><width>180</width><height>90</height></rect></property>
     <property name="text"><string/></property>
     <property name="pixmap"><pixmap resource="logo.qrc">:/Logo.png</pixmap></property>
     <property name="scaledContents"><bool>true</bool></property>
    </widget>
    <widget class="QPushButton" name="pushButton_15">
     <property name="geometry"><rect><x>0</x><y>120</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Employ&#233;s</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_21">
     <property name="geometry"><rect><x>0</x><y>170</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Produits</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_20">
     <property name="geometry"><rect><x>0</x><y>220</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Commandes</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_fournisseurs_label">
     <property name="geometry"><rect><x>0</x><y>270</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Fournisseurs</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_22">
     <property name="geometry"><rect><x>0</x><y>320</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Mati&#232;res</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_23">
     <property name="geometry"><rect><x>0</x><y>370</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>Machines</string></property>
    </widget>
    <widget class="QPushButton" name="pushButton_16">
     <property name="geometry"><rect><x>0</x><y>640</y><width>210</width><height>50</height></rect></property>
     <property name="text"><string>D&#233;connexion</string></property>
    </widget>
   </widget>

"@

$needle = '  <widget class="QMenuBar" name="menubar">'
$idx = $content.IndexOf($needle)

if ($idx -ge 0) {
    $newContent = $content.Substring(0, $idx) + $sidebar + $content.Substring($idx)
    [System.IO.File]::WriteAllText($filepath, $newContent, [System.Text.Encoding]::UTF8)
    Write-Host "SUCCESS: Sidebar inserted at index $idx"
} else {
    Write-Host "ERROR: Could not find QMenuBar insertion point"
    Write-Host "Last 300 chars: $($content.Substring($content.Length - 300))"
}
