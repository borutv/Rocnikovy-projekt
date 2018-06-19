# Rocnikovy-projekt
Letný semester:

Podarilo sa mi vytvoriť program ktorý mi v realnom čase vráti X, Y a Z súradnice očí. Pri rozpoznávaní tváre a očí som využival knižnicu OpenCV a už ich vytvorené kaskády. Pri tomto projekte hlavne išlo okrem implementácie aj o pochopenie princípu rozpoznavania tváre a oči algoritmom ktoré sa použivajú (Viola-Jones, integral image, Window Sliding).

Viola-Jones

Vlastnosti algoritmu Viola-Jones, ktoré ho robia dobrým detekčným algoritmom, sú:

-Robustnosť

-Spracovávanie v realnom čase

-Iba detekcia tváre (nie jej rozpoznávanie)


Algoritmus má štyri fázy:

-Vybrať funkciu Haar

-Vytvorenie integralného obrázka

-Adaboost Training

-Kaskádové klasifikátory

Všetky ľudské tváre majú podobné vlastnosti. Tieto zákonitosti možno zosúladiť pomocou funkcií Haar.

Niekoľko spoločných vlastností ľudských tvárí:


Oblasť očí je tmavšia ako horná časť tváre.

Oblasť nosa je jasnejšia ako oči.

Zloženie vlastností, ktoré tvoria kompatibilné tvárové vlastnosti:


Umiestnenie a veľkosť: oči, ústa, most nosa

Vlastnosti obdĺžnika:


Hodnota = Σ (pixely v čiernej oblasti) - Σ (pixely v bielej oblasti)
Tri typy: dva, tri, štyri obdĺžniky, Viola & Jones používali dva obdĺžnikové prvky.
Napríklad: rozdiel v jasnosti medzi bielymi a čiernými obdĺžnikmi v určitej oblasti.
Každá funkcia súvisí so špeciálnou polohou v podokne.


Obrazové zobrazenie nazývané integrálny obraz hodnotí obdĺžnikové prvky v konštantnom čase, čo im dáva značnú rýchlostnú výhodu oproti sofistikovanejším alternatívnym funkciám. Keďže obdĺžniková oblasť každého objektu je vždy susedia s aspoň jedným ďalším obdĺžnikom, z toho vyplýva, že akékoľvek dva obdĺžníky možno vypočítať v šiestich referenčných poliach, akékoľvek tri v ôsmych a akékoľvek štyri v deviatich.

______________________________________________________________________


Tento program by sa dal použiť ako jeden z modulov do bakalárskej práce v ktorej riešim pohľadovo závislé stereo zobrazenie. Tým, že mi v realnom čase vracia polohu očí tak dokážem zobrazovaný 3d model natáčať v závislosti od polohy hlavy a tým vytvárať dojem, že to je naozaj pred nami.

Krátky popis programu:

program sa snaží najsť tvár z obrazu ktorý si zoberie z kamery. Potom čo nájde tvár sa snaží nájsť oči a teda ich stred a následne okolo nich opíše kružnicu s nejakym polomerom r co bude nasledne náš default_z dokym nestratime tú tvár. Potom čo úspešne našiel oči a opisal kružnicu mi vypíše do konzoly ich suradnice [X, Y, default_z -radius+default_z].

Výstup:

![alt text](https://raw.githubusercontent.com/borutv/Rocnikovy-projekt/master/Priklad.png)


Výstup v konzole:

eye1 x:751
eye1 y:226
eye1 z:16
eye2 x:624
eye2 y:226
eye2 z:16
