TEMPLATE = subdirs

SUBDIRS += \
    QLepp2DLib \
    QLepp2DGui

QLepp2DGui.depends = QLepp2DLib
