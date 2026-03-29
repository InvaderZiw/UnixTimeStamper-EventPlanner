README
Descriere generală

În această temă am implementat mai multe funcții și programe în limbajul C pentru lucrul cu Unix Timestamps, conversii între timestamp și dată/oră, prelucrare de date împachetate pe biți și planificarea unui eveniment între mai multe persoane aflate în fusuri orare diferite.

Pentru rezolvare nu am folosit funcții din time.h sau alte biblioteci care fac automat conversii de timp, ci am implementat manual calculele necesare.

Task 1 - Convert Unix Timestamp to Time

Pentru acest task am transformat timestamp-ul primit în numărul total de secunde scurse de la momentul de referință și am determinat ora, minutul și secunda corespunzătoare ultimei zile.

Mai exact:

am calculat numărul total de ore, minute și secunde;
am folosit operațiile modulo pentru a obține:
ora în intervalul [0, 23],
minutul în intervalul [0, 59],
secunda în intervalul [0, 59].

Astfel am extras doar componenta de timp asociată timestamp-ului.

Task 2 - Convert Unix Timestamp to Date Without Leap Years

Pentru acest task am ignorat anii bisecți și am lucrat doar cu ani de 365 de zile.

Am procedat astfel:

am transformat timestamp-ul în număr total de zile;
am determinat anul prin împărțiri și scăderi succesive;
după aflarea anului, am calculat câte zile au mai rămas în anul curent;
am folosit un vector care conține numărul de zile din fiecare lună;
cu ajutorul unei variabile de tip contor am determinat luna în care se află ziua respectivă;
după identificarea lunii, am calculat ziua exactă din lună.
Task 3 - Convert Unix Timestamp to Date

La acest task am extins logica de la taskul anterior, de data aceasta luând în considerare și anii bisecți.

Am procedat astfel:

am transformat timestamp-ul în număr total de zile;
am determinat anul ținând cont dacă este sau nu bisect;
am folosit doi vectori pentru lunile unui an normal și ale unui an bisect;
după determinarea anului, am identificat luna și ziua prin încadrarea numărului de zile rămase în intervalele corespunzătoare lunilor.

În acest mod am obținut data corectă pentru orice timestamp valid din intervalul cerut.

Task 4 - Convert Unix Timestamp to DateTimeTZ

Pentru acest task am pornit de la rezultatele obținute la taskurile 1 și 3, adică data și ora în UTC.

Apoi:

am aplicat diferența de fus orar (utc_hour_difference);
am verificat dacă ora rezultată iese din intervalul [0, 23];
dacă trecea în ziua următoare sau în ziua anterioară, am ajustat corespunzător data;
la modificarea datei am ținut cont de numărul de zile din lună și de anii bisecți.

Rezultatul final este o structură care conține:

data,
ora,
timezone-ul asociat.
Task 5 - Convert DateTimeTZ to Unix Timestamp

Pentru acest task am realizat operația inversă față de taskul 4.

Am procedat astfel:

am calculat numărul total de zile dintre 1 ianuarie 1970 și data primită;
am adunat zilele anilor compleți;
am adunat zilele lunilor complete din anul curent;
am adăugat ziua curentă;
am transformat totul în secunde și am adăugat ora, minutul și secunda;
la final am compensat diferența de timezone pentru a obține timestamp-ul în UTC.

Astfel am reconstruit timestamp-ul asociat unei structuri TDateTimeTZ.

Task 6 - Print DateTimeTZ

Pentru afișare am folosit formatul cerut în enunț.

Am folosit:

un vector de șiruri de caractere pentru numele lunilor;
specificatorul %02u pentru a afișa ora, minutul și secunda mereu pe două cifre;
specificatorul %+d sau %+i pentru afișarea diferenței față de UTC cu semn.

În acest mod, afișarea respectă exact cerința din enunț.

Task 7 - Basic Network Packing

La acest task am extras ziua, luna și anul dintr-un unsigned int în care aceste informații erau memorate pe biți.

Am construit măști pentru:

zi,
lună,
an.

După aceea:

am aplicat operația AND logic între număr și fiecare mască;
am deplasat la dreapta biții pentru lună și an astfel încât valorile să ajungă în forma normală;
am stocat rezultatele într-un vector de structuri TDate;
am sortat vectorul cronologic folosind qsort;
am afișat datele în formatul cerut.
Task 8 - Complex Network Packing

La acest task am extins logica de la taskul 7 pentru cazul în care datele sunt împachetate continuu pe 15 biți, fără să mai fie aliniate separat câte una într-un unsigned int.

Am procedat astfel:

am calculat câte unsigned int sunt necesare pentru a citi toate cele N date;
pentru fiecare dată am determinat bitul de început din fluxul total de biți;
am verificat dacă cei 15 biți ai datei se află integral într-un singur unsigned int sau sunt împărțiți între două;
am extras biții necesari și am reconstruit valoarea de 15 biți corespunzătoare unei date;
apoi am decodat ziua, luna și anul în același mod ca la taskul 7.

În plus, am tratat și partea de validare:

pentru fiecare unsigned int de date am calculat paritatea, adică numărul_de_biți_1 % 2;
am citit biții de control și am verificat dacă aceștia coincid cu paritatea calculată;
dacă un unsigned int era corupt, toate datele care foloseau biți din acel unsigned int au fost ignorate;
doar datele valide au fost reținute, sortate și afișate.

Acest task a necesitat atenție specială la manipularea biților și la cazurile în care o dată este împărțită între două valori consecutive.

Task 9 - Multi-timezone Event Planner

La acest task am implementat un planificator de evenimente pentru mai multe persoane aflate în timezone-uri diferite.

Pentru fiecare persoană:

am citit timezone-ul și intervalele în care aceasta este liberă;
am transformat fiecare început de interval în Unix Timestamp, folosind funcțiile implementate anterior;
am transformat fiecare interval într-un interval în UTC de forma [start, end).

După aceea:

am sortat intervalele fiecărei persoane;
am unit intervalele suprapuse sau lipite, pentru a obține o reprezentare mai curată și mai ușor de verificat;
am construit o listă de momente candidate de start, folosind începuturile intervalelor de disponibilitate.

Pentru fiecare moment candidat:

am verificat câte persoane pot participa pe toată durata evenimentului;
o persoană este considerată validă doar dacă are un interval care acoperă complet intervalul evenimentului;
am ales primul moment pentru care numărul de participanți valizi este cel puțin F.

La final:

am sortat persoanele alfabetic;
pentru fiecare persoană am verificat dacă poate participa;
dacă putea participa, am convertit timestamp-ul evenimentului în timezone-ul ei și l-am afișat;
altfel am afișat invalid.

Dacă nu exista niciun moment valid, programul afișa imposibil.

Observații de implementare

În mai multe locuri am folosit variabile ajutătoare pentru a evita probleme de conversie între tipuri de date, de exemplu între int, unsigned int și unsigned char. Această abordare m-a ajutat să evit erori la atribuire și să păstrez codul mai clar.

De asemenea, pentru sortări am folosit funcția qsort, iar pentru alocarea structurilor și vectorilor necesari am folosit memorie dinamică.
