#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T12:14:39
#
#-------------------------------------------------

QT       += core gui xml qml declarative quick widgets opengl


TARGET = NeoGemini
TEMPLATE = app

RC_FILE = winres.rc

SOURCE_PATH = $$(QTGAMESRC)

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += util \
    particle \
    particle/emitters \
    particle/initializers \
    particle/operators \
    particle/renderers \
    audio \
    menu \
    camera \
    game \
    game/event \
    game/ai \
    widgets \
    material \
    collision \
    script \
    stats \
    .

#DEFINES += SFML_DYNAMIC
DEFINES += SFML_STATIC

win32 {
    CONFIG(debug, debug|release){
        LIBS += $${SOURCE_PATH}/SFML/sfml-system-s-d.lib
        LIBS += $${SOURCE_PATH}/SFML/sfml-audio-s-d.lib
    } else {
        LIBS += $${SOURCE_PATH}/SFML/sfml-system-s.lib
        LIBS += $${SOURCE_PATH}/SFML/sfml-audio-s.lib
    }

    LIBS += -lAdvapi32
    DEFINES += PLATFORM_WINDOWS

    # doesn't work
    QMAKE_CXXFLAGS += /wd4100

    QMAKE_CXXFLAGS_RELEASE    = -O2 -MD
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -O2 -MD -Zi
    QMAKE_CXXFLAGS_DEBUG      = -Zi -MDd
} else:macx {
    LIBS += -framework SFML -lsfml-system -lsfml-audio
    DEFINES += PLATFORM_OSX
    QMAKE_CXXFLAGS += -Wno-unused-parameter
} else:unix {
    QMAKE_LFLAGS += -Wl,-rpath,$${SOURCE_PATH}/bin
    LIBS += -L$${SOURCE_PATH}/bin -lsfml-audio
    DEFINES += PLATFORM_UNIX
    QMAKE_CXXFLAGS += -Wno-unused-parameter
}

CONFIG(debug, debug|release){
DEFINES += DEBUG
} else {
DEFINES += RELEASE
}


OTHER_FILES += \
    content/* \
    content/particles/* \
    content/sounds/* \
    content/maps/* \
    content/materials/* \
    content/resources/* \
    qml/* \
    bin/* \
    qml/Hud.qml

win32 {
    QMAKE_POST_LINK += xcopy $${SOURCE_PATH}\\qml\\* $${DESTDIR}qml\\* /I /R /Y /S &
    QMAKE_POST_LINK += xcopy $${SOURCE_PATH}\\bin\\* $${DESTDIR}* /I /R /Y /S &
    QMAKE_POST_LINK += xcopy $${SOURCE_PATH}\\content\\* $${DESTDIR}content\\* /I /R /Y /S &
} else:unix {
    QMAKE_POST_LINK += mkdir -p $${DESTDIR}qml ; mkdir -p $${DESTDIR}content ;

    QMAKE_POST_LINK += cp -rf $${SOURCE_PATH}/qml/* $${DESTDIR}./qml ;
    QMAKE_POST_LINK += cp -rf $${SOURCE_PATH}/bin/* $${DESTDIR}./ ;
    QMAKE_POST_LINK += cp -rf $${SOURCE_PATH}/content/* $${DESTDIR}./content
}

QML_IMPORT_PATH += qml

SOURCES += main.cpp\
        mainwindow.cpp \
    util/util.cpp \
    util/globals.cpp \
    simulationlist.cpp \
    particle/particleroot.cpp \
    util/vector2d.cpp \
    particle/particlefunctionfactory.cpp \
    particle/particleprecache.cpp \
    particle/particlesystem.cpp \
    particle/renderers/render_circle.cpp \
    particle/emitters/emitter_continuously.cpp \
    particle/initializers/init_sphere.cpp \
    particle/initializers/init_size_random.cpp \
    particle/operators/op_lifetime_decay.cpp \
    particle/initializers/init_lifetime_random.cpp \
    camera/camera.cpp \
    particle/operators/op_alpha_fade.cpp \
    particle/initializers/init_velocity_random.cpp \
    particle/operators/op_movement.cpp \
    particle/renderers/render_line.cpp \
    particle/operators/op_size_scale.cpp \
    particle/operators/op_length_scale.cpp \
    particle/initializers/init_length_random.cpp \
    audio/audiomanager.cpp \
    game/game.cpp \
    util/precache.cpp \
    game/entity.cpp \
    game/player.cpp \
    widgets/particleview.cpp \
    material/materialprecache.cpp \
    game/layer.cpp \
    game/map.cpp \
    game/layerparticle.cpp \
    game/layerfill.cpp \
    util/config.cpp \
    particle/renderers/render_material.cpp \
    particle/renderers/render_base.cpp \
    material/material.cpp \
    particle/initializers/init_angle_random.cpp \
    particle/initializers/init_angularvelocity_random.cpp \
    material/queuedpainter.cpp \
    util/bytebuffer.cpp \
    material/passthroughpainter.cpp \
    material/parallelrenderer.cpp \
    util/barrier.cpp \
    game/input.cpp \
    widgets/rootview.cpp \
    widgets/menubackground.cpp \
    game/sprite.cpp \
    particle/operators/op_lock_to_system_origin.cpp \
    game/ship.cpp \
    particle/renderers/render_flare.cpp \
    particle/operators/op_set_children_control_point.cpp \
    particle/operators/op_remap_control_point.cpp \
    game/resource.cpp \
    particle/operators/op_gravity.cpp \
    particle/emitters/emitter_instantaneously.cpp \
    particle/operators/op_noise.cpp \
    particle/operators/op_oscillate.cpp \
    game/entityfactory.cpp \
    game/projectile.cpp \
    game/obstacle.cpp \
    collision/collisionmanager.cpp \
    game/enemyship.cpp \
    game/event/scripteventmanager.cpp \
    game/event/scripteventfactory.cpp \
    game/event/abstractscriptevent.cpp \
    game/event/rootevent.cpp \
    game/event/obstacleevent.cpp \
    game/event/continuousevent.cpp \
    game/event/abstractdelayedscriptevent.cpp \
    game/event/instantaneousevent.cpp \
    game/event/abstractentityscriptevent.cpp \
    stats/score.cpp \
    stats/ranking.cpp \
    particle/initializers/init_color_random.cpp \
    game/event/fighterevent.cpp \
    game/ai/aifactory.cpp \
    game/destructible.cpp \
    game/ai/abstractai.cpp \
    game/ai/simpleai.cpp \
    game/ai/fighter1ai.cpp \
    game/ai/fighter2ai.cpp \
    game/ai/fighter3ai.cpp \
    game/laserbeam.cpp \
    game/event/musicevent.cpp \
    game/bossship.cpp \
    game/event/bossevent.cpp \
    game/ai/boss0ai.cpp \
    widgets/gameview.cpp \
    game/event/warningtextevent.cpp \
    game/event/cutsceneevent.cpp \
    stats/events.cpp \
    stats/statistics.cpp \
    stats/plotter.cpp

HEADERS  += mainwindow.h \
    util/util.h \
    util/globals.h \
    simulationlist.h \
    base.h \
    particle/particleroot.h \
    particle/particle.h \
    util/vector2d.h \
    particle/particlebase.h \
    particle/particlesystem.h \
    isimulated.h \
    particle/iparticleinitializer.h \
    particle/iparticleoperator.h \
    particle/iparticlerenderer.h \
    particle/particlefunctionfactory.h \
    particle/iparticlefunction.h \
    particle/particleprecache.h \
    util/platform.h \
    particle/iparticleemitter.h \
    particle/iparticlecontext.h \
    camera/camera.h \
    audio/audiomanager.h \
    main.h \
    game/game.h \
    util/precache.h \
    game/entity.h \
    game/player.h \
    widgets/particleview.h \
    material/materialprecache.h \
    game/layer.h \
    game/map.h \
    game/gamebase.h \
    game/layerparticle.h \
    game/layerfill.h \
    util/macros.h \
    util/config.h \
    particle/renderers/render_base.h \
    material/material.h \
    material/ipainter.h \
    material/queuedpainter.h \
    util/bytebuffer.h \
    material/passthroughpainter.h \
    material/parallelrenderer.h \
    pch.h \
    util/platform_warnings.h \
    util/barrier.h \
    util/blockingqueue.h \
    game/input.h \
    widgets/rootview.h \
    widgets/menubackground.h \
    game/sprite.h \
    game/igamecontext.h \
    material/rendercontext.h \
    material/irenderable.h \
    particle/iparticleparent.h \
    game/ship.h \
    game/resource.h \
    game/entityfactory.h \
    game/gameutil.h \
    game/projectile.h \
    game/obstacle.h \
    collision/icollidable.h \
    collision/collisionmanager.h \
    game/enemyship.h \
    game/event/iscriptevent.h \
    game/event/scripteventmanager.h \
    game/event/scripteventfactory.h \
    game/event/abstractscriptevent.h \
    game/event/scripteventbase.h \
    game/event/abstractdelayedscriptevent.h \
    game/event/abstractentityscriptevent.h \
    stats/score.h \
    stats/ranking.h \
    game/damage.h \
    game/ai/aifactory.h \
    util/factory.h \
    game/destructible.h \
    game/ai/aibase.h \
    game/ai/iai.h \
    game/ai/abstractai.h \
    game/laserbeam.h \
    game/bossship.h \
    widgets/gameview.h \
    game/event/cutsceneevent.h \
    util/keyvalues.h \
    stats/events.h \
    stats/statistics.h \
    stats/plotter.h

FORMS    += mainwindow.ui \
    widgets/rootview.ui

#RESOURCES     = resources.qrc \
#    resources.qrc
