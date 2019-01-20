# IrcSocketBufferingParsing

Un bot IRC tout simple.

Le code se compose de la gestion des sockets Windows && UNIX avec l'utilisation de select() pour ne pas rester bloqué sur un recv() et le parseur des lignes IRC qui va bien (http://abcdrfc.free.fr/rfc-vf/rfc1459.html).

J'ai utilisé ce bot irc pour afficher la sortie de mdisass (les 256 premières instructions) sur un canal irc durant l'année 2012. Pour exécuter mdisass, j'utilisais tout simplement popen()

Le code démontre aussi une certaine technique de buffering (voir set_irc_buffer)

    $ gcc main.c parser.c socket.c asprintf.c -o ircbot

output:
line: +:adams.freenode.net 001 SimpleBot :Welcome to the freenode Internet Relay Chat Network SimpleBot+
