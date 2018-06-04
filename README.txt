Demon synchronizujący dwa katalogi (na przykład A i B)

Poprawne użycie programu:

 (1) demon <ścieżka źródłowa> <ścieżka docelowa> [opcje]
 

Jeżeli chcesz uruchomić demona i rozpocząć synchronizację, użyj rozwiązania (1).
Jeżeli chcesz wymusić synchronizację lub zatrzymać uruchomionego demona, użyj rozwiązania (2).
Jeżeli chcesz wyświetlić pomoc wpisz 'demon -h'

Opis opcji stosowanych w (1) przypadku użycia:

 -R : kopiowanie rekursywne

 -s <liczba>[jednostka] : czas uśpienia demona, jednostki (domyślnie sekundy):
  s : sekundy
  m : minuty
  h : godziny

 -t <liczba>[jednostka] : pliki tego rozmiaru i większe będą kopiowane za pomocą mmap, jednostki (domyślnie bajty):
  k : kilobajty 
  m : megabajty
  g : gigabajty

Zabicie programu: 
	kill -SUGUSR1 nr_pid
Instalacja
Wymagania:
* make
* gcc

Aby zainstalować program należy wykonać następujące komendy:
- make
- sudo make install

Przykładowe użycie programu :
	./program <kat. źródłowy> <kat. docelowy> -R -s30s -t50k
- program uruchomiony z synchronizacją podkatalogów (-R), wybudzany co 30s (-s30s), próg wiekości pliku do mmapowania 50kb (-t50b)
AUTORZY
Projekt wykonali Adam Buńkowski, Marcin Bacławski ©2017
