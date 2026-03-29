Task-1: Am transformat prin calcule numarul de secunde in numarul de ore, minute si secunde de la timestampul 0 iar apoi
am facut mod 24 respectiv 60 pentru ca am nevoie de restul impartirii, adica de ora minutul si secundele din ultima zi

Task-2:Am transformat anii prin calcul efectiv ,iar apoi am aflat numarul de zile de la anul aflat pana la timestampul 
dat, iar apoi am creat un vector in care se afla zilele lunilor anului iar apoi m am folosit de u DAY_COUNTER in care stocam
suma primelor cateva luni pentru a incatdra numarul de zile aflat intr-un interval si pentru a determina luna.Odata aflat intervalul
am scazut din zilele aflate DAY_COUNTER care da zilele ramase

Task-3:Am transformat timestampul in zile pentru ca (din fericire) toate zilele sunt formate din 84600 de secunde, iar apoi
cu ajutorul a 2 vectori (unul pentru an si unul pentru luna) am reusit sa incatrez in intervale zilele aflate si am procedat ca 
la task ul anterior

Task-4:M am folosit de task urile 1 si 3 pentru a atribui la result.date respectiv result.time data si ora de la UTC, iar apoi am 
adunat cu diferenta de ora si am facut verificat daca ora trece de intervalul (0,24), iar apoi am verificat daca zilele trec de intervalul [1,30) sau [1,31) si tot asa pentru a mi da timestampul corect

Task-5:Am facut un proces invers al task ului 4 in sensul ca am calculat suma zilelor tuturor anilor, dupa am 
calculat suma zilelor de la anul curent pana la luna curenta iar am adunat zilele ramase si am facut calcul efectiv pentru a 
obtine timestampul

Task-6:M-am folosit de specificatorii %02u si %+i pentru a printa in formatul specificat

Task-7:Am creat 3 masti care aveau pe pozitiile 1 2 3 4 5, 6 7 8 9 respctiv 10 11 12 13 14 15 15 bitul 1 pentru a putea face "si" logic ,a shift-a la stg(mai putin la zile) si a le stoca intr o vector de tip strcut si dupa le am sortat cu qsort

Task-8 & Task-9: Skill issue

PS: La taskurile unde aveam probleme cu atribuirea de date (cum ar fi unui char sa i atribui valoare unui int) m-am folosit 
de o variabila ajutatoare(ex:*_step_1 sau month_int)