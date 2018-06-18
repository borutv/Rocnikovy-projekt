# Rocnikovy-projekt
Letný semester:

Podarilo sa mi vytvoriť program ktorý mi v realnom čase vráti X, Y a Z súradnice očí. Pri rozpoznávaní tváre a očí som využival knižnicu OpenCV a už ich vytvorené kaskády. Pri tomto projekte hlavne išlo okrem implementácie aj o pochopenie princípu rozpoznavania tváre a oči algoritmom ktoré sa použivajú (Viola-Jones, integral image, Window Sliding).

Krátky popis programu:

program sa snaží najsť tvár z obrazu ktorý si zoberie z kamery. Potom čo nájde tvár sa snaží nájsť oči a teda ich stred a následne okolo ich opíše kružnicu s nejakym polomerom r co bude nasledne náš default_z dokym nestratime tú tvár. Potom čo úspešne našiel oči a opisal kružnicu mi vypíše do konzoly ich suradnice [X, Y, default_z -radius+default_z].

Výstup:

![alt text](https://raw.githubusercontent.com/borutv/Rocnikovy-projekt/master/Priklad.png)


Výstup v konzole:

eye1 x:751
eye1 y:226
eye1 z:16
eye2 x:624
eye2 y:226
eye2 z:16
