# twdiff
#### Descriere
twdiff este un simplu program scris in C care compara mai multe fisiere dupa atomi(tokens) si afiseaza diferite 
date despre comparatie.

Primul scop al acestui program a fost determinarea surselor plagiate trimise de elevi in timpul unui test pe o
platforma de tipul contest management system.

#### Descriere tehnica
Acest program foloseste un alt fisier care sa descrie sintaxa fisierelor de intrare. Formatul acestui fisier 
este descris [aici](../SYNTAX_FILE.md) iar un exemplu de fisier de sintaxa poate fi gasit [aici](../syntax/c_cpp.syntax.twdiff).
Fiecarui tip de atom din acest fisier ii este atribuit un id unic. 

In etapa de citire a fisierelor de intrare, acest program foloseste tipurile de atomi definite in fisierul de
sintaxa si creaza un vector de id-uri. 

Cand ajunge la compararea propriu-zisa, foloseste algoritmul de determinare a celui mai lung subsir comun (LCS) dintre 
vectorii corespunzatori fisierelor de intrare.

Pentru performanta(original a fost facut sa fie apelat de mii de ori), am adaugat optiunea de a trimite mai multe fisiere
si de a specifica perechile intre care sa se aplice algoritmul.


###### Important
- Daca un atom incepe cu `#`, el si restul liniei pe care acesta se afla va fi ignorata.
- In cazul in care un fisier de intrare nu este scris corect, programul va ignora toate perechile din care face parte

#### Cerinte de sistem
Merge pe orice sistem care are runtime-ul C, dar a fost testat doar pe Linux