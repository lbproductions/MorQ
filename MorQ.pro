cache()

TEMPLATE = subdirs

CONFIG      +=  ordered
SUBDIRS     =   QPersistence \
                QSerienJunkies \
                QuunRar \
                Server


QPersistence.subdir           =   lib/QPersistence

QSerienJunkies.subdir       =   lib/QSerienJunkies

QuunRar.subdir              =   lib/QuunRar

Server.subdir               =   Server
Server.depends              =   QPersistence \
                                QSerienJunkies \
                                QuunRar
