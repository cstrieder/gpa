gpa++ - 26 janeiro 2010

Para instalar:

chmod +x install.sh

./install.sh

gpa++ sera instalado na raiz do diretorio do usuario. ex: /home/cristiano/gpa++/bin/

Para executar gpa++:

Modo texto:

./gpa++ file.dat

file.dat e um arquivo ASCII contendo uma matrix de valores

ex:

1 2 3
4 5 6
7 8 9

Modo grafico:

Para este e preciso que JAVA esteja instalado.

'java -jar gpa++GUI.jar' pode ser utilizada para executar gpa++ em um ambiente de janelas.

Resultados:

O resultado obtido por gpa++ e salvo em uma pasta com nome 'file_GPAC_OUT' no mesmo diretorio do arquivo analisado file.dat.


gpa++:

gpa++ foi por escrito em C++ em ambiente Linux Slackware 13 com a IDE Netbeans 6.8, por Cristiano Strieder.

gpa++ utiliza 3angle para fazer a triangualacao de Delaunay. 3angle foi criado a partir do codigo de 'Triangle - A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator', Jonathan Richard Shewchuk. Obtido no site http://www.cs.cmu.edu/~quake/triangle.html em meados de 2009.


gpa++GUI:

gpa++GUI foi escrita em JAVA e e completamente independente do gpa++ que roda em modo texto. O intuito de gpa++GUI e oferecer uma maneira facilidata de acesso ao gpa++.