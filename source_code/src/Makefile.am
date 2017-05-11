# Makefile.am for v-amps

bin_PROGRAMS = vamps

# list of all generated (and not distributed) sources
nodist_vamps_SOURCES = \
	box_moc.cpp \
	canvas_moc.cpp \
	colorwidget_moc.cpp \
	configdialog_moc.cpp \
	function_moc.cpp \
	guiwidget_moc.cpp \
	m_advmcv_moc.cpp \
	m_env_moc.cpp \
    m_lfo_moc.cpp \
	m_mcv_moc.cpp \
	m_mix_moc.cpp \
	m_noise_moc.cpp \
	m_pcmout_moc.cpp \
	m_pcmin_moc.cpp \
	m_ringmod_moc.cpp \
	m_seq_moc.cpp \
	m_sh_moc.cpp \
	m_vca_moc.cpp \
	m_vcf_moc.cpp \
	m_vco_moc.cpp \
	m_fsin_moc.cpp \
	m_inv_moc.cpp \
	mainwindow_moc.cpp \
	midicheckbox_moc.cpp \
	midicombobox_moc.cpp \
	midicontrollable_moc.cpp \
	midiguicomponent_moc.cpp \
	midipushbutton_moc.cpp \
	midisliderbase_moc.cpp \
	midiwidget_moc.cpp \
	modularsynth_moc.cpp \
	module_moc.cpp \
	port_moc.cpp \
	port_popup_moc.cpp \
	prefwidget_moc.cpp \
	textedit_moc.cpp
	
# list of all c++ source files
vamps_SOURCES = \
	box.cpp \
	canvas.cpp \
	canvasfunction.cpp \
	colorwidget.cpp \
	configdialog.cpp \
	envelope.cpp \
	function.cpp \
	guiwidget.cpp \
	intmidislider.cpp \
	m_advmcv.cpp \
	m_env.cpp \
	m_lfo.cpp \
	m_mcv.cpp \
	m_mix.cpp \
	m_noise.cpp \
	m_pcmout.cpp \
	m_pcmin.cpp \
	m_ringmod.cpp \
	m_seq.cpp \
	m_sh.cpp \
	m_vca.cpp \
	m_vcf.cpp \
	m_vco.cpp \
	m_fsin.cpp \
	m_inv.cpp \
	main.cpp \
	mainwindow.cpp \
	mced.cpp \
	midicheckbox.cpp \
	midicombobox.cpp \
	midicontrollable.cpp \
	midicontroller.cpp \
	midiguicomponent.cpp \
	midipushbutton.cpp \
	midislider.cpp \
	midisliderbase.cpp \
	midiwidget.cpp \
	modularsynth.cpp \
	module.cpp \
	port.cpp \
	port_popup.cpp \
	prefwidget.cpp \
	synthdata.cpp \
	textedit.cpp \
	notelist.cpp \
	getclocktimer.cpp \
	voice.cpp
	
# list of all header files
noinst_HEADERS = \
	box.h \
	canvas.h \
	canvasfunction.h \
	colorwidget.h \
	configdialog.h \
	envelope.h \
	function.h \
	guiwidget.h \
	intmidislider.h \
    m_advmcv.h \
	m_env.h \
	m_lfo.h \
	m_mcv.h \
	m_mix.h \
	m_noise.h \
	m_pcmout.h \
	m_pcmin.h \
	m_ringmod.h \
	m_seq.h \
	m_sh.h \
	m_vca.h \
	m_vcf.h \
	m_vco.h \
	m_fsin.h \
	m_inv.h \
	macros.h \
	main.h \
	mainwindow.h \
	mced.h \
	midicheckbox.h \
	midicombobox.h \
	midicontrollable.h \
	midicontroller.h \
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
	textedit.h \
	notelist.h \
	join_threads.hpp \
	getclocktimer.h \
	thread_pool.hpp \
	thread_safe_queue.hpp \
	spinlock_mutex.hpp \
	voice.h
	
translationsdir = $(pkgdatadir)/translations
translations = \
	translations/amps_de.ts \
	translations/amps_fr.ts

translations_DATA = \
	translations/amps_de.qm \
	translations/amps_fr.qm

# misc files which are distributed but not installed
EXTRA_DIST = amps.pro $(translations)

# all generated files to be removed by "make clean"
CLEANFILES = $(nodist_vamps_SOURCES) $(translations_DATA)

AM_LDFLAGS = @QT_LIBS@
AM_CXXFLAGS = @QT_CXXFLAGS@ @VAMPS_CXXFLAGS@
DEFS = -D_REENTRANT -DLADSPA_PATH=\"$(LADSPA_PATH)\" -DTRANSLATIONSDIR=\"$(translationsdir)\" @DEFS@

# rule for moc-file generation
SUFFIXES = _moc.cpp

.h_moc.cpp: $(vamps_SOURCES)
	$(MOC) $< -o $@

# rule for translation file updates
$(translations): $(vamps_SOURCES)
	        $(LUPDATE) -verbose $(vamps_SOURCES) -ts $@

# rule to generate binary translation files
.ts.qm: $(translations)
	$(LRELEASE) -verbose $< -qm $@
