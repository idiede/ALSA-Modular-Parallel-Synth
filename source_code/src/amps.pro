message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_SETTINGS])
message(Examples: $$[QT_INSTALL_EXAMPLES])
message(Demonstrations: $$[QT_INSTALL_DEMOS])
######################################################################
#
######################################################################

TEMPLATE = app
TARGET = 

CONFIG += link_pkgconfig
PKGCONFIG += alsa
# lash not yet used
# PKGCONFIG += lash-1.0

DEPENDPATH += .
INCLUDEPATH += .
LIBS += -lclalsadrv -ljack

#LADSPA_PATH = $$(LADSPA_PATH)
#isEmpty( LADSPA_PATH ) {
#LADSPA_PATH = "/usr/lib/ladspa:/usr/local/lib/ladspa"
#}

PLATFORM = $$system(uname -i)
OBJECTS_DIR = BUILD_$${PLATFORM}
DESTDIR = $$OBJECTS_DIR
MOC_DIR = MOC

INSTALL_PATH = "/usr"
PIXMAPS_PATH = "$$INSTALL_PATH/share/amps/pixmaps/"
QMAKE_CXXFLAGS += -DPIXMAPS_PATH=\\\"$$PIXMAPS_PATH\\\"

message($$PLATFORM)
message($$OBJECTS_DIR)

# QMAKE_CXXFLAGS tweeks. uncomment, tweak, post your favorite :-)
#
# base
# QMAKE_CXXFLAGS += -DLADSPA_PATH=\\\"$$LADSPA_PATH\\\"
#
# x86_64 fedora
QMAKE_CXXFLAGS += -DLADSPA_PATH=\\\"/usr/lib64/ladspa\\\"
#
# i386 best bet
# QMAKE_CXXFLAGS += -msse2 -mfpmath=sse -ffast-math -DLADSPA_PATH=\\\"$$LADSPA_PATH\\\"
#
# bad code ?
# QMAKE_CXXFLAGS += -mtune=k8 -ffast-math -DLADSPA_PATH=\\\"$$LADSPA_PATH\\\"
#
# end QMAKE_CXXFLAGS tweeks

QT += opengl

# Input
HEADERS += box.h \
           canvas.h \
           canvasfunction.h \
           configdialog.h \
           envelope.h \
           function.h \
           guiwidget.h \
           intmidislider.h \
           m_advmcv.h \
           #m_delay.h \
         
           m_env.h \
    
           m_fsin.h \
           m_lfo.h \
           m_mcv.h \
           
           m_noise.h \
          
           m_pcmout.h \
           m_ringmod.h \
          
           m_seq.h \
           m_sh.h \
           m_vca.h \
           
           m_vcf.h \
           m_vco.h \
           m_inv.h \
           main.h \
           mainwindow.h \
           mced.h \
           midicheckbox.h \
           midicombobox.h \
           midicontrollable.h \
           midicontroller.h \
           midicontrollerlist.h \
           midiguicomponent.h \
           midipushbutton.h \
           midisliderbase.h \
           midislider.h \
           midiwidget.h \
           modularsynth.h \
           module.h \
           msoptions.h \
           
           port.h \
           port_popup.h \
           prefwidget.h \
           ringbuffer.h \
          
           synthdata.h \
           textedit.h


### removed for minimal
           #ladspadialog.h \
           #m_ad.h \
           #m_advenv.h \
        
           #m_amp.h \
           #m_conv.h \
           #m_cvs.h \
           #m_dynamicwaves.h \
           #m_function.h \
          
           #m_ladspa.h \
           #m_midiout.h \
           #m_mix.h \
           #m_mphlfo.h \
           #m_noise2.h \
           #m_pcmin.h \
           #m_quantizer.h \
          
           #m_scmcv.h \
           #m_scope.h \
           #m_scquantizer.h \
           
           #m_slew.h \
           #m_spectrum.h \
           #m_stereomix.h \
           #m_vcdoubledecay.h \
           #m_vcenv.h \
           #m_vcenv2.h \
           #m_vco2.h \
           #m_vcorgan.h \
           #m_vcpanning.h \
           #m_vcswitch.h \
           #m_vquant.h \
           #m_wavout.h \
           #multi_envelope.h \
           #scopescreen.h \
           
           
SOURCES += box.cpp \
           canvas.cpp \
           canvasfunction.cpp \
           configdialog.cpp \
           envelope.cpp \
           function.cpp \
           guiwidget.cpp \
           intmidislider.cpp \
           m_advmcv.cpp \
           m_delay.cpp \
        
           m_env.cpp \
 
           m_fsin.cpp \
           m_lfo.cpp \
           m_mcv.cpp \
          
           m_noise.cpp \
         
           m_pcmout.cpp \
           m_ringmod.cpp \
           m_seq.cpp \
           m_sh.cpp \
           m_vca.cpp \
          
           m_vcf.cpp \
           m_vco.cpp \
           m_inv.cpp \
           main.cpp \
           mainwindow.cpp \
           mced.cpp \
           midicheckbox.cpp \
           midicombobox.cpp \
           midicontrollable.cpp \
           midicontroller.cpp \
           midicontrollerlist.cpp \
           midiguicomponent.cpp \
           midipushbutton.cpp \
           midislider.cpp \
           midisliderbase.cpp \
           midiwidget.cpp \
           modularsynth.cpp \
           module.cpp \
          
           port.cpp \
           prefwidget.cpp \
          
           synthdata.cpp \
           textedit.cpp
           
           
 ## removed for minimal amps         
           #ladspadialog.cpp \
           #m_ad.cpp \
           #m_advenv.cpp \
         
           #m_amp.cpp \
           #m_conv.cpp \
           #m_cvs.cpp \
           #m_dynamicwaves.cpp \
           #m_function.cpp \
          
           #m_ladspa.cpp \
           #m_midiout.cpp \
           #m_mix.cpp \
           #m_mphlfo.cpp \
           #m_noise2.cpp \
           #m_pcmin.cpp \
           #m_quantizer.cpp \
        
           #m_scmcv.cpp \
           #m_scope.cpp \
           #m_scquantizer.cpp \
          
           #m_slew.cpp \
           #m_spectrum.cpp \
           #m_stereomix.cpp \
           #m_vcdoubledecay.cpp \
           #m_vcenv.cpp \
           #m_vcenv2.cpp \
           #m_vco2.cpp \
           #m_vcorgan.cpp \
           #m_vcpanning.cpp \
           #m_vcswitch.cpp \
           #m_vquant.cpp \
           #m_wavout.cpp \
           #multi_envelope.cpp \
           #scopescreen.cpp \

TRANSLATIONS = \
	translations/amps_de.ts
