Ich benutze i.d.R. den QtCreator mit CMake als build system. Du kannst den Code aber auch kompilieren mit:

    g++ -std=c++20 -O3 -Wall -Wextra -Wpedantic -fopenmp -pthread main.cpp defaultrandgen.cpp swarm.cpp trajectory.cpp -o ./output/Exo8

(unter der Annahme, dass ein Unterordner ./output existiert – ansonsten musst du natürlich den Parameter -o anpassen)

Der Code geht auch davon aus, dass in ./output ein Script "plot.gnuplot" existiert, und dass gnuplot installiert und in der PATH-Variable eingetragen ist. Das Script findest du im Archiv; falls gnuplot bei dir nicht läuft, kannst du auch einfach nur die Textdateien anschauen und selbst irgendwie plotten/zur GIF verarbeiten.
Der Verlet-Alg. bis t=10 läuft hier sogar in unter einer Minute durch. Nennenswert viel Zeit wird von gnuplot verbraucht, das natürlich auch nur single threaded laufen kann.

Die dummy.gif/dummy.out sind (wie du in der main siehst) nur als Ergebnis von Testcode entstanden, mit dem ich geprüft habe, ob mein Filewriter wirklich so funktioniert, wie ich das wollte.

Hoffe, du kommst damit zurecht, ansonsten können wir gern vor/nach der VL nochmal reden.

Viele Grüße,
Stefan
