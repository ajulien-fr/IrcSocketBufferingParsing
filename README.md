# IrcSocketBufferingParsing

Un bot IRC tout simple.

Le code se compose de la gestion des sockets Windows && UNIX avec l'utilisation de select() et le parseur qui va bien.

J'ai utilisé ce bot irc pour afficher la sortie de mdisass (les 256 premières instructions) sur un canal irc dans les années 2012. Pour exécuter mdisass, j'utilisais tout simplement popen()

Le code démontre aussi une certaine technique de buffering (voir set_irc_buffer)
