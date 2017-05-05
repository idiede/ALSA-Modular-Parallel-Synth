#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <qwidget.h>
#include <qstring.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <QApplication>
#include <QAbstractScrollArea>
#include <qcolordialog.h>
#include <qsocketnotifier.h>
#include <QFileDialog>
#include <qpen.h>
#include <qstringlist.h>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMenu>
#include <QPaintEvent>
#include <QStringList>
#include <QTextBlock>
#include <QTextEdit>

#include "midislider.h"
#include "intmidislider.h"
#include "midicombobox.h"
#include "midicheckbox.h"
#include "modularsynth.h"
#include "port.h"
#include "midiwidget.h"
#include "guiwidget.h"
#include "midicontroller.h"
#include "textedit.h"

#include "m_vco.h"
#include "m_vca.h"
//#include "m_delay.h"
#include "m_env.h"
//#include "m_ede.h"
#include "m_mcv.h"
#include "m_pcmout.h"
#include "m_pcmin.h"
#include "m_lfo.h"
#include "m_noise.h"
#include "m_seq.h"
#include "m_vcf.h"

#include "prefwidget.h"
#include "config.h"

//removed modules for minmal
//#include "ladspadialog.h"

//#include "m_spectrum.h"

//#include "m_vco2.h"



//#include "m_vcenv.h"
//#include "m_vcenv2.h"
//#include "m_advenv.h"

#include "m_advmcv.h"
//#include "m_scmcv.h"
#include "m_ringmod.h"
#include "m_mix.h"
//#include "m_stereomix.h"


//#include "m_wavout.h"

//#include "m_noise2.h"
//#include "m_ladspa.h"

#include "m_inv.h"
//#include "m_amp.h"
//#include "m_ad.h"
//#include "m_vquant.h"
//#include "m_conv.h"
//#include "m_cvs.h"
//#include "m_vcswitch.h"
//#include "m_slew.h"
#include "m_sh.h"
//#include "m_vcpanning.h"
//#include "m_midiout.h"
//#include "m_scope.h"
//#include "m_vcorgan.h"
//#include "m_dynamicwaves.h"
//#include "m_quantizer.h"
//#include "m_scquantizer.h"
//#include "m_function.h"
//#include "m_vcdoubledecay.h"
//#include "m_mphlfo.h"


static const char COLOREXT[] = ".acs";


SynthData *synthdata;
thread_pool *pool;

ModularSynth::ModularSynth(QWidget* parent, const ModularSynthOptions& mso) 
  : QWidget(parent)
  , pcmname(mso.pcmname)
  , fsamp(mso.fsamp)
  , frsize(mso.frsize)
  , nfrags(mso.nfrags)
  , ncapt(mso.ncapt)
  , nplay(mso.nplay)
  , verbose(mso.verbose)
  , paintFastly(false)
  , _zoomFactor(1.0)
{
  modified = false;
  dragWidget = NULL;
  selectedPort = NULL;
  connectorStyle = CONNECTOR_BEZIER;
  portPopup = new PopupMenu(this);
  aboutWidget = new QMessageBox(parent); 

  synthdata = new SynthData(mso.synthName, mso.poly, mso.edge);
  pool = new thread_pool(mso.threads);
  midiWidget = new MidiWidget(NULL);
  midiWidget->setWindowTitle(tr("AlsaModularSynth Control Center"));
  synthdata->midiWidget = midiWidget;
  
  guiWidget = new GuiWidget(NULL);
  guiWidget->setWindowTitle(tr("AlsaModularSynth Parameter View"));
  synthdata->guiWidget = guiWidget;
  
  prefWidget = new PrefWidget();
  prefWidget->setWindowTitle(tr("AlsaModularSynth Preferences"));
  QObject::connect(prefWidget, SIGNAL(prefChanged()),
                   this, SLOT(refreshColors()));

/*  ladspaDialog = new LadspaDialog();
  QObject::connect(static_cast<QWidget *>(ladspaDialog),
		   SIGNAL(createLadspaModule(int, int, bool, bool)),
                   this, SLOT(newM_ladspa(int, int, bool, bool)));

  setPalette(QPalette(QColor(240, 240, 255), QColor(240, 240, 255)));
  loadingPatch = false;
*/
}

ModularSynth::~ModularSynth()
{
  synthdata->stopPCM();
  synthdata->midiWidget->clearAllClicked();

  guiWidget->close();
  prefWidget->close();
  midiWidget->close();
  delete synthdata;
}

QSize ModularSynth::sizeHint() const
{
    return QSize(std::max(childrenRect().right(),
                ((QWidget*)parent())->width()),
            std::max(childrenRect().bottom(),
                ((QWidget*)parent())->height()));
}

void ModularSynth::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    if (!paintFastly)
        p.setRenderHint(QPainter::Antialiasing);

    for (int l1 = 0; l1 < listModule.count(); ++l1) {
        Module* m = listModule.at(l1);
        
        if (m == NULL)
            continue;

        m->paintCablesToConnectedPorts(p);
    }
}
 
void ModularSynth::mousePressEvent(QMouseEvent *ev)
{
    switch (ev->button()) {
        case Qt::LeftButton:  
            {
                QWidget *pw = childAt(ev->pos());
                if (pw != NULL) {
                    Box* mb = qobject_cast<Box *>(pw);
                    if (mb != 0) {
                        lastMousePos = ev->globalPos();
                        dragWidget = mb;
                        mb->raise();
                        setPaintFastly(true);
                    }
                }
                ev->accept();
                break;
            }

        case Qt::RightButton:
	  {
	    /* there are three different context actions:
	     * 1) show port property menu
	     * 2) show module property dialog
	     * 3) show module list menu
	     **/
	    QWidget *pw = childAt(ev->pos());
	    if (pw) {
	      Port *pp = qobject_cast<Port *>(pw);
	      if (pp) {
		if (pp->isInPort()) {
		  // show port popup menu
		  pp->popupMenuClicked(portPopup->runAt(ev->globalPos()));
		  update();
		}
	      } else {
		Module *mb = qobject_cast<Module *>(pw);
		if (mb) {
		  // show module property window
		  mb->showConfigDialog(ev->globalPos());
		}
	      }
	    } else {
	      // show module list menu
	      newBoxPos = ev->pos();
	      contextMenu->popup(ev->globalPos());
	    }
	    ev->accept();
	  }
	  break;

        default:
            ev->ignore();
            break;
    }
}  

void ModularSynth::mouseMoveEvent(QMouseEvent *ev)
{
    if (!dragWidget)
	return;

    bool crossTopLeft = synthdata->editingFlags.crossTopLeft();
    QPoint delta = ev->globalPos() - lastMousePos;
    lastMousePos = ev->globalPos();
    QPoint newPos = dragWidget->pos() + delta;

    if (!crossTopLeft) {
	/*top and left limit for movement*/
	if (newPos.x() < 0)
		newPos.setX(0);
	if (newPos.y() < 0)
		newPos.setY(0);
    }
    dragWidget->move(newPos);
    if (crossTopLeft) {
	QPoint moveAll;
	if (newPos.x() < 0)
	    moveAll.rx() = - newPos.x();
	if (newPos.y() < 0)
	    moveAll.ry() = - newPos.y();

	QRect chR = childrenRect();
	if (delta.x() > 0 && chR.topLeft().x() > 60) {
	    int x = std::min(delta.x(), chR.topLeft().x() - 60);
	    moveAll.rx() -= x;
	}
	if (delta.y() > 0 && chR.topLeft().y() > 30) {
	    int y = std::min(delta.y(), chR.topLeft().y() - 30);
	    moveAll.ry() -= y;
	}
	moveAllBoxes(moveAll);
    }
    adjustSize();
    update();
    modified = true;
}  

void ModularSynth::mouseReleaseEvent(QMouseEvent *ev)
{
    switch (ev->button()) {
        case Qt::LeftButton:
            if (dragWidget != NULL)
                dragWidget = NULL;
            setPaintFastly(false);
            update();
            ev->accept();
            break;
        default:
            ev->ignore();
            break;
    }
}  

int ModularSynth::go(bool forceJack, bool forceAlsa)
{
    if ((synthdata->seq_handle = open_seq()))
        initSeqNotifier();
    else
        qWarning(QObject::tr("Alsa MIDI wont work!").toUtf8());

    midiWidget->setActiveMidiControllers();

    int err = forceAlsa ? -1 : synthdata->initJack(ncapt, nplay);

    if (err < 0 && !forceJack)
	err = synthdata->initAlsa(pcmname.toLocal8Bit(),
				  fsamp, frsize, nfrags, ncapt, nplay);

    if (err < 0)
	exit(err);

    startSynth();

    return 0;
}

void ModularSynth::displayAbout() {
 
    aboutWidget->about(this, tr("About ") + MONO_AMPS_LONGNAME,
            MONO_AMPS_LONGNAME " " VERSION
            "\nby Matthias Nagorni and Fons Adriaensen\n"
            "(c)2002-2003 SuSE AG Nuremberg\n"
            "(c)2003 Fons Adriaensen\n"
            "(c)2007 Malte Steiner\n"
            "(c)2007-2009 Karsten Wiese\n"
            "(c)2008-2009 Guido Scholz\n\n"
            "Documentation and examples can be found in\n"
            "/usr/share/tamps\n\n"
            "More presets and updates are available from\n"
            "http://alsamodular.sourceforge.net/"
    "\n\nAcknowledgements\n"
    "----------------------\n\n"
    "The VCF Module uses the resonant low-pass filter by Paul Kellett and\n" 
    "the Cookbook formulae for audio EQ biquad filter coefficients by\n"
    "Robert Bristow-Johnson. The experimental 24 dB Lowpass filters have\n" 
    "been taken from http://musicdsp.org/. They are based on the CSound source\n"
    "code, the paper by Stilson/Smith and modifications by Paul Kellett\n" 
    "and Timo Tossavainen. The pink noise conversion formula is by Paul\n" 
    "Kellett and has been taken from http://musicdsp.org/ as well.\n\n"
    "The author is grateful to Takashi Iwai for instructions about ALSA.\n"
    "Klaas Freitag, Helmut Herold, Stefan Hundhammer and Arvin Schnell\n"
    "answered many questions about Qt. Thanks to J�rg Arndt for valuable\n"
    "hints regarding speed optimization. Torsten Rahn has helped to\n" 
    "improve the color scheme of the program. Thanks to Bernhard Kaindl\n"
    "for helpful discussion.\n");
    aboutWidget->raise();
}

void ModularSynth::displayMidiController()
{
  if (!midiWidget->isVisible())
    midiWidget->show();
  else {
    midiWidget->raise();
    midiWidget->activateWindow();
  }
}

void ModularSynth::displayParameterView() {

  guiWidget->show();
  guiWidget->raise();
}

void ModularSynth::displayPreferences() {

  prefWidget->show();
  prefWidget->raise();
}

/*void ModularSynth::displayLadspaPlugins() {
   
  ladspaDialog->show();  
  ladspaDialog->raise();
}*/

int ModularSynth::getSynthDataPoly()
{
    return synthdata->poly;
}

snd_seq_t *ModularSynth::open_seq() {

    snd_seq_t *seq_handle;
    QString qs;

    if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_DUPLEX,
                SND_SEQ_NONBLOCK) < 0) {
        qWarning(QObject::tr("Error opening ALSA sequencer.").toUtf8());
        return NULL;
    }

    snd_seq_set_client_name(seq_handle,
            (synthdata->name + " Midi").toLatin1().constData());

    if (snd_seq_create_simple_port(seq_handle, "mono amps in",
                SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
        qWarning(QObject::tr("Error creating sequencer write port.").toUtf8());
        snd_seq_close(seq_handle);
        return NULL;
    }
    for (int l1 = 0; l1 < 2; ++l1)
        if ((synthdata->midi_out_port[l1] =
                    snd_seq_create_simple_port(seq_handle, "mono amps out",
                        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                        SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
            qWarning(QObject::tr("Error creating sequencer read port.").toUtf8());
            snd_seq_close(seq_handle);
            return NULL;
        }
    return seq_handle;
}

int ModularSynth::initSeqNotifier()
{
    seqNotifier = new QSocketNotifier(synthdata->pipeFd[0],
				      QSocketNotifier::Read, this);
    QObject::connect(seqNotifier, SIGNAL(activated(int)),
		     this, SLOT(midiAction(int)));
    return(0);
}

void ModularSynth::midiAction(int fd)
{
    char pipeIn[16];

    ssize_t pipeRed = read(fd, pipeIn, sizeof(pipeIn));

    if (pipeRed < 0) {
        StdErr << __PRETTY_FUNCTION__ << ": read() " << endl;
        perror(NULL);
        exit(-1);
    }
    if (pipeRed < 1 || pipeRed >= (ssize_t)sizeof(pipeIn))
        StdErr << __PRETTY_FUNCTION__ << ": read() " << pipeRed << " bytes" << endl;
    if (pipeRed == 0)
        return;

    while (pipeRed > 1)
        pipeIn[0] |= pipeIn[--pipeRed];

    MidiControllableBase *mcAble = NULL;
    for (int mCs = synthdata->mcSet.count(); mCs; --mCs) {
        mcAble = synthdata->mcSet.get();
        mcAble->updateMGCs(NULL);
    }
    if (mcAble)
        midiWidget->midiTouched(*mcAble);

    for (int mcKs = synthdata->mckRead.count(); mcKs; --mcKs) {
        MidiControllerKey mcK = synthdata->mckRead.get();
        if (midiWidget->isVisible()) {
            if (mcK.type() == SND_SEQ_EVENT_CONTROLLER ||
                    mcK.type() == SND_SEQ_EVENT_CONTROL14 ||
                    mcK.type() == SND_SEQ_EVENT_PITCHBEND ||
                    (midiWidget->noteControllerEnabled &&
                     (mcK.type() == SND_SEQ_EVENT_NOTEON ||
                      mcK.type() == SND_SEQ_EVENT_NOTEOFF)))
                midiWidget->addMidiController(mcK);
        }
    }

    for (int mcKs = synthdata->mckDump.count(); mcKs; --mcKs) {
	MidiControllerKey mcK = synthdata->mckDump.get();
	switch (mcK.type()) {
	case SND_SEQ_EVENT_SYSEX:
	    StdErr << "SYSEX MIDI events are not supported" << endl;
	    break;
	default:
	    StdErr << "Unsupported MIDI event received (type = " <<
		mcK.type() << ")" << endl;
	    break;
	}
    }
    if (pipeIn[0] & 4)
	guiWidget->setCurrentPresetText();
}

/* redraws complete module area when a module connection is changed*/
void ModularSynth::redrawPortConnections()
{
    update();
    modified = true;
}

void ModularSynth::initNewModule(Module *m)
{
  m->move(newBoxPos);
  m->show();

  QObject::connect(m, SIGNAL(removeModule()),
          this, SLOT(deleteModule()));
  QObject::connect(m, SIGNAL(portSelected(Port*)),
          this, SLOT(portSelected(Port*)));
  QObject::connect(m, SIGNAL(portDisconnected()),
          this, SLOT(redrawPortConnections()));

  listModule.append(m);
  if (!loadingPatch) {
    midiWidget->addModule(m);
  }
  modified = true;
}

void ModularSynth::new_textEdit()
{
  TextEdit *te = new TextEdit(this, "textEdit");
  te->move(newBoxPos);
  te->show();
  QObject::connect(te, SIGNAL(sizeDragged(const QPoint&)),
          this, SLOT(resizeTextEdit(const QPoint&)));
  QObject::connect(te, SIGNAL(removeTextEdit()), this, SLOT(deleteTextEdit()));
  listTextEdit.append(te);
  modified = true;
}

void ModularSynth::new_textEdit(int w, int h) {

  TextEdit *te = new TextEdit(this, "textEdit");
  te->setFixedSize(w, h);
  te->move(newBoxPos);
  te->show();
  QObject::connect(te, SIGNAL(sizeDragged(const QPoint&)),
          this, SLOT(resizeTextEdit(const QPoint&)));
  QObject::connect(te, SIGNAL(removeTextEdit()), this, SLOT(deleteTextEdit()));
  listTextEdit.append(te);
}

void ModularSynth::startSynth()
{
  pthread_mutex_lock(&synthdata->rtMutex);
  synthdata->doSynthesis = true;
  pthread_mutex_unlock(&synthdata->rtMutex);
}

void ModularSynth::stopSynth()
{
  pthread_mutex_lock(&synthdata->rtMutex);
  synthdata->doSynthesis = false;
  pthread_mutex_unlock(&synthdata->rtMutex);
}
//################################################ start add new modules
void ModularSynth::newM_seq(int seqLen) {

    M_seq *m = new M_seq(seqLen, this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_seq_8() {

    newM_seq(8);
}

void ModularSynth::newM_mcv() {

    M_mcv *m = new M_mcv(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_env() {

    M_env *m = new M_env(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vco() {

    M_vco *m = new M_vco(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vca_lin() {

    M_vca *m = new M_vca(false, this);
    if (m != NULL)
        initNewModule(m);
}
void ModularSynth::newM_vca_exp() {

    M_vca *m = new M_vca(true, this);
    if (m != NULL)
        initNewModule(m);
}
void ModularSynth::newM_lfo() {

    M_lfo *m = new M_lfo(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_noise() {

    M_noise *m = new M_noise(this);
    if (m != NULL)
        initNewModule(m);
}

//void ModularSynth::newM_delay() {
//
//    M_delay *m = new M_delay(this);
//    if (m != NULL)
//        initNewModule(m);
//}

void ModularSynth::newM_pcmout()
{
    int k;
    M_pcmout *m;

    k = synthdata->find_play_mod (0);
    if (k >= 0)
    {
        m = new M_pcmout (this, 2 * k);
        if (m != NULL) {
            initNewModule(m);
            synthdata->set_play_mod (k, m);
        }
    }
    else
        qWarning(QObject::tr("All available output ports are in use").toUtf8());
}

void ModularSynth::newM_vcf() {

    M_vcf *m = new M_vcf(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_advmcv() {

    M_advmcv *m = new M_advmcv(this);
    if (m != NULL) {
        synthdata->listM_advmcv.append(m);
        initNewModule(m);
    }
}

void ModularSynth::newM_pcmin()
{
    int k;
    M_pcmin *m;

    k = synthdata->find_capt_mod (0);
    if (k >= 0)
    {
        m = new M_pcmin(this, 2 * k);
        if (m != NULL) {
            initNewModule(m);
            synthdata->set_capt_mod (k, m);
        }
    }
    else
        qWarning(QObject::tr("All available input ports are in use").toUtf8());
}



//##############################   end of minimal v-amps- redo full synth later ####################
/*void ModularSynth::newM_seq_12() {

    newM_seq(12);
}

void ModularSynth::newM_seq_16() {

    newM_seq(16);
}

void ModularSynth::newM_seq_24() {

    newM_seq(24);
}

void ModularSynth::newM_seq_32() {

    newM_seq(32);
}

void ModularSynth::newM_vcorgan(int oscCount) {

    M_vcorgan *m = new M_vcorgan(oscCount, this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vcorgan_4() {

    newM_vcorgan(4);
}

void ModularSynth::newM_vcorgan_6() {

    newM_vcorgan(6);
}

void ModularSynth::newM_vcorgan_8() {

    newM_vcorgan(8);
}

void ModularSynth::newM_dynamicwaves(int oscCount) {

    M_dynamicwaves *m = new M_dynamicwaves(oscCount, this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_dynamicwaves_4() {

    newM_dynamicwaves(4);
}

void ModularSynth::newM_dynamicwaves_6() {

    newM_dynamicwaves(6);
}

void ModularSynth::newM_dynamicwaves_8() {

    newM_dynamicwaves(8);
}*/




/*
void ModularSynth::newM_scmcv() {

    M_scmcv *m = new M_scmcv(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_scmcv(QString *p_scalaName) {

    M_scmcv *m = new M_scmcv(this, p_scalaName);
    if (m != NULL)
        initNewModule(m);
}
*/


/*void ModularSynth::newM_vcenv() {

    M_vcenv *m = new M_vcenv(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vcenv2() {

    M_vcenv2 *m = new M_vcenv2(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vcdoubledecay() {

    M_vcdoubledecay *m = new M_vcdoubledecay(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_vcpanning() {

    M_vcpanning *m = new M_vcpanning(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_advenv() {

    M_advenv *m = new M_advenv(this);
    if (m != NULL)
        initNewModule(m);
}
*/
/*void ModularSynth::newM_vco2() {

    M_vco2 *m = new M_vco2(this);
    if (m != NULL)
        initNewModule(m);
}
*/



/*void ModularSynth::newM_mphlfo() {

    M_mphlfo *m = new M_mphlfo(this);
    if (m != NULL)
        initNewModule(m);
}
*/

/*
void ModularSynth::newM_noise2() {

    M_noise2 *m = new M_noise2(this);
    if (m != NULL)
        initNewModule(m);
}
*/
void ModularSynth::newM_ringmod() {

    M_ringmod *m = new M_ringmod(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_inv() {

    M_inv *m = new M_inv(this);
    if (m != NULL)
        initNewModule(m);
}
/*
void ModularSynth::newM_amp() {

    M_amp *m = new M_amp(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_ad(int outCount) {

    M_ad *m = new M_ad(outCount, this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_ad_2() { 

    newM_ad(2);
}

void ModularSynth::newM_ad_4() { 

    newM_ad(4);
}

void ModularSynth::newM_ad_6() { 

    newM_ad(6);
}

void ModularSynth::newM_vquant() {

    M_vquant *m = new M_vquant(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_conv() {

    M_conv *m = new M_conv(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_cvs() {

    M_cvs *m = new M_cvs(this);
    if (m != NULL)
        initNewModule(m);
}
*/
void ModularSynth::newM_sh() {

    M_sh *m = new M_sh(this);
    if (m != NULL)
        initNewModule(m);
}
/*
void ModularSynth::newM_vcswitch() {

    M_vcswitch *m = new M_vcswitch(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_slew() {

    M_slew *m = new M_slew(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_quantizer() {

    M_quantizer *m = new M_quantizer(this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_scquantizer(QString *p_scalaName) {

    M_scquantizer *m = new M_scquantizer(this, p_scalaName);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_scquantizer() {

    M_scquantizer *m = new M_scquantizer(this);
    if (m != NULL)
        initNewModule(m);
}
*/


void ModularSynth::newM_mix(int in_channels) {

    M_mix *m = new M_mix(in_channels, this);
    if (m != NULL)
        initNewModule(m);
}

/*
void ModularSynth::newM_function(int functionCount) {

    M_function *m = new M_function(functionCount, this);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_function_1() {

    newM_function(1);
}

void ModularSynth::newM_function_2() {

    newM_function(2);
}

void ModularSynth::newM_function_4() {

    newM_function(4);
}

void ModularSynth::newM_stereomix(int in_channels) {

    M_stereomix *m = new M_stereomix(in_channels, this);
    if (m != NULL)
        initNewModule(m);
}
*/
void ModularSynth::newM_mix_2() {

    newM_mix(2);
}

void ModularSynth::newM_mix_4() {

    newM_mix(4);
}

void ModularSynth::newM_mix_8() {

    newM_mix(8);
}
/*
void ModularSynth::newM_stereomix_2() {

    newM_stereomix(2);
}

void ModularSynth::newM_stereomix_4() {

    newM_stereomix(4);
}

void ModularSynth::newM_stereomix_8() {

    newM_stereomix(8);
}

void ModularSynth::newM_ladspa(int p_ladspaDesFuncIndex, int n, bool p_newLadspaPoly, bool p_extCtrlPorts) {

    QString qs;

    qs = synthdata->ladspaLib.at(p_ladspaDesFuncIndex).desc.at(n)->Name;
    M_ladspa *m = new M_ladspa(this, p_ladspaDesFuncIndex, n,
            p_newLadspaPoly, p_extCtrlPorts);
    if (m != NULL)
        initNewModule(m);
}

void ModularSynth::newM_wavout() {

    M_wavout *m = new M_wavout(this);
    if (m != NULL) {
        synthdata->wavoutModuleList.append(m);
        initNewModule(m);
    }
}

void ModularSynth::newM_midiout() {

    M_midiout *m = new M_midiout(this);
    if (m != NULL) {
        synthdata->midioutModuleList.append(m);
        initNewModule(m);
    }
}
*/

/*


void ModularSynth::newM_scope() {

    M_scope *m = new M_scope(this);
    if (m != NULL) {
        synthdata->scopeModuleList.append(m);
        initNewModule(m);
    }
}

void ModularSynth::newM_spectrum() {

    M_spectrum *m = new M_spectrum(this);
    if (m != NULL) {
#ifdef OUTDATED_CODE
        synthdata->spectrumModuleList.append(m);
#endif
        initNewModule(m);
    }
}
*/

//========================================== End of adding module functions

void ModularSynth::resizeTextEdit(const QPoint& pos) {

  int l1;
  TextEdit *te;
  int cx, cy;
  
  for (l1 = 0; l1 < listTextEdit.count(); ++l1) {
    if ((te = listTextEdit.at(l1)) == sender()) {
      cx = pos.x();
      cy = pos.y();
      if ((cx > 200) && (cy > 170)) { 
        te->setFixedSize(cx + 3, cy + 3);
      }
    }
  }
}

// selecting and connecting ports:
void ModularSynth::portSelected(Port* p)
{
    if (p == NULL)
        return;

    if (p->isInPort() && p->hasConnectedPort()) {
        qApp->beep();
    }
    else if (selectedPort == NULL) {
        selectedPort = p;
        p->setHighlighted(true);
    }
    else if (selectedPort == p) {
        p->setHighlighted(false);
        selectedPort = NULL;
    }
    else if (selectedPort->module == p->module) {
        qApp->beep();
    }
    else if (((selectedPort->isInPort()) && (!p->isInPort()))
            || ((p->isInPort()) && (!selectedPort->isInPort()))) {
        selectedPort->connectTo(p);
        p->connectTo(selectedPort);
        p->setHighlighted(false);
        selectedPort->setHighlighted(false);
        selectedPort = NULL;
        redrawPortConnections();
    }
    else
        qApp->beep();
}

void ModularSynth::deleteModule() {

  Module *m;
 
  m = (Module *)sender();
  listModule.removeAll(m);
  m->Delete();
  modified = true;
}

void ModularSynth::deleteTextEdit()
{
  TextEdit *t = dynamic_cast<TextEdit *>(sender());
  listTextEdit.removeAll(t);
  t->deleteLater();
}

void ModularSynth::deleteTextEdit(TextEdit *te) {

  delete(te);
}

bool ModularSynth::clearConfig(bool restart)
{
  int l1;

  bool restartSynth = synthdata->doSynthesis;
  stopSynth();

  for (l1 = 0; l1 < listModule.count(); ++l1)
    listModule.at(l1)->Delete();

  listModule.clear();
  guiWidget->clearGui();
  for (l1 = 0; l1 < listTextEdit.count(); ++l1) {
    deleteTextEdit(listTextEdit.at(l1));
  }
  listTextEdit.clear();
  synthdata->moduleID = 0;
  synthdata->moduleCount = 0;
  if (restartSynth && restart)
    startSynth();

  synthdata->initVoices();
  update();
  return restartSynth;
}

//#################################################### Start persistence
static int Fscanf(FILE *f, const char *format, ...)
{
   va_list argList;
   va_start(argList, format);

   return vfscanf(f, format, argList);
}

static void setColor(FILE *f, QColor &c)
{
  int r, g, b;
  Fscanf(f, "%d", &r);   
  Fscanf(f, "%d", &g);   
  Fscanf(f, "%d", &b);   
  c = QColor(r, g, b);
}

void ModularSynth::loadColors() {

    QString config_fn, qs;
    FILE *f;
    char sc[2048];

    config_fn = QFileDialog::getOpenFileName(this, tr("Load Colors"),
            synthdata->loadPath, tr("AlsaModularSynth Color files") +
            " (*" + COLOREXT + ")");
    if (config_fn.isEmpty())
        return;

    if (!(f = fopen(config_fn.toLatin1().constData(), "r"))) {
        QMessageBox::information( this, "AlsaModularSynth",
                tr("Could not open file."));
    }
    else {
        while (Fscanf(f, "%s", sc) != EOF) {
            qs = QString(sc);
            if (qs.contains("ColorBackground", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorBackground);
            }        
            else if (qs.contains("ColorModuleBackground", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorModuleBackground);
            }        
            else if (qs.contains("ColorModuleBorder", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorModuleBorder);
            }        
            else if (qs.contains("ColorModuleFont", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorModuleFont);
            }        
            else if (qs.contains("ColorJack", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorJack);
            }        
            else if (qs.contains("ColorCable", Qt::CaseInsensitive)) {
                setColor(f, synthdata->colorCable);
            }        
        }      
        updateColors();
        fclose(f);
    }        
}

void ModularSynth::saveColors() {

    FILE *f;
    QString config_fn, qs;

    config_fn = QFileDialog::getSaveFileName(this, tr("Save Colors"),
            synthdata->savePath, tr("AlsaModularSynth Color files") + 
            " (*" + COLOREXT + ")");
    if (config_fn.isEmpty())
        return;

    /*check for file extension*/
    if (!config_fn.endsWith(COLOREXT))
        config_fn.append(COLOREXT);

    if (!(f = fopen(config_fn.toLatin1().constData(), "w"))) { 
        QMessageBox::information( this, "AlsaModularSynth",
                tr("Could not save file."));
    }
    else {
        fprintf(f, "ColorBackground %d %d %d\n",
                synthdata->colorBackground.red(),
                synthdata->colorBackground.green(),
                synthdata->colorBackground.blue());
        fprintf(f, "ColorModuleBackground %d %d %d\n",
                synthdata->colorModuleBackground.red(),
                synthdata->colorModuleBackground.green(),
                synthdata->colorModuleBackground.blue());
        fprintf(f, "ColorModuleBorder %d %d %d\n",
                synthdata->colorModuleBorder.red(),
                synthdata->colorModuleBorder.green(),
                synthdata->colorModuleBorder.blue());
        fprintf(f, "ColorModuleFont %d %d %d\n",
                synthdata->colorModuleFont.red(),
                synthdata->colorModuleFont.green(),
                synthdata->colorModuleFont.blue());
        fprintf(f, "ColorJack %d %d %d\n", synthdata->colorJack.red(),
                synthdata->colorJack.green(), synthdata->colorJack.blue());
        fprintf(f, "ColorCable %d %d %d\n", synthdata->colorCable.red(),
                synthdata->colorCable.green(), synthdata->colorCable.blue());
        fclose(f);
    }
}                             

void ModularSynth::load(QTextStream& ts)
{
  int moduleID = 0;
  int index = 0;
  int midiSign = 1;
  M_typeEnum M_type;
  int value, x, y, w, h, subID1, subID2;
  int index1, index2, moduleID1, moduleID2;
  int type, ch, param, sliderMin, sliderMax;
  int red1, green1, blue1, red2, green2, blue2;
  QString qs, qs2, pluginName, para, scalaName;//ladspaLibName,
  bool isLog,  commentFlag, followConfig;//ladspaLoadErr,
  //int newLadspaPolyFlag = 0;
  int textEditID;
  Module *m;
  int currentProgram;
  QStringList tokens;

  followConfig = midiWidget->followConfig;
  midiWidget->followConfig = false;
  currentProgram = -1;

  bool restartSynth = clearConfig(false);
  //ladspaLoadErr = false;
  commentFlag = false;
  loadingPatch = true;

  while (!ts.atEnd()) {
    qs = ts.readLine();

    if (qs.startsWith("#PARA#", Qt::CaseInsensitive))
      commentFlag = true;

    else if (qs.startsWith("#ARAP#", Qt::CaseInsensitive))
      commentFlag = false;

    // Module <type> <id> <xpos> <ypos> <subid1> <subid2>
    // Module <type> <id> <xpos> <ypos> <scalaname> <->
    // Module <type> <id> <xpos> <ypos> <l_poly> <l_lib> <l_plugin>
    //   0       1    2      3     4       5        6        7
    else if (qs.startsWith("Module", Qt::CaseInsensitive) && !commentFlag) {
        tokens = qs.split(' ');
      M_type = (M_typeEnum)tokens[1].toInt();
      moduleID = tokens[2].toInt();
      newBoxPos.setX(tokens[3].toInt());
      newBoxPos.setY(tokens[4].toInt());


//load what we need
     switch (M_type) {
   /*       case M_type_ladspa:
              newLadspaPolyFlag = tokens[5].toInt();
              ladspaLibName = tokens[6];
              pluginName = qs.section(' ', 7);
              qWarning(tr("Loading LADSPA plugin \"%1\" from library \"%2\".") 
                      .arg(pluginName).arg(ladspaLibName).toUtf8().constData());

              if (!synthdata->getLadspaIDs(ladspaLibName, pluginName,
                          &subID1, &subID2)) {
                  QMessageBox::information(this, "AlsaModularSynth",
                          tr("Could not find LADSPA plugin "
                              "\"%1\" from library \"%2\".")
                          .arg(pluginName).arg(ladspaLibName));
                  ladspaLoadErr = true;
              }
              break;
          case M_type_scquantizer: 
          case M_type_scmcv: 
              scalaName = tokens[5];
              break;*/
          default: 
              subID1 = tokens[5].toInt();
              subID2 = tokens[6].toInt();
              break;
      }

    switch(M_type) {
    //      case M_type_custom:
     //         break;
          case M_type_vco: 
              newM_vco();
              break;
        //  case M_type_vco2:
        //      newM_vco2();
        //      break;

          case M_type_vca: 
             if (subID1)
                 newM_vca_exp();
              else
                  newM_vca_lin();
              break;
          case M_type_vcf: 
              newM_vcf();
              break;
          case M_type_lfo: 
              newM_lfo();
              break;
          //case M_type_mphlfo:
          //    newM_mphlfo();
          //    break;
          case M_type_noise: 
              newM_noise();
              break;
      //    case M_type_noise2:
      //        newM_noise2();
      //        break;
//          case M_type_delay:
//              newM_delay();
//              break;
          case M_type_seq: 
              newM_seq(subID1);
              break;
          case M_type_env: 
              newM_env();
              break;
        /*  case M_type_vcenv:
              newM_vcenv();
              break;
          case M_type_vcenv2: 
              newM_vcenv2();
              break;
          case M_type_vcdoubledecay: 
              newM_vcdoubledecay();
              break;
          case M_type_vcpanning: 
              newM_vcpanning();
              break;
          case M_type_advenv: 
              newM_advenv();
              break;                 */
          case M_type_mcv: 
              newM_mcv();
              break;
          case M_type_advmcv:
              newM_advmcv();
              break;
              /*         case M_type_scmcv:
              newM_scmcv(&scalaName);
              break;
                */
          case M_type_ringmod: 
              newM_ringmod();
              break;

          case M_type_inv: 
              newM_inv();
              break;
               /*
          case M_type_amp: 
              newM_amp();
              break;
          case M_type_vquant: 
              newM_vquant();
              break;
          case M_type_conv: 
              newM_conv();
              break;
              */
          case M_type_sh: 
              newM_sh();
              break;
              /*
          case M_type_vcswitch: 
              newM_vcswitch();
              break;
          case M_type_cvs: 
              newM_cvs();
              break;
          case M_type_slew: 
              newM_slew();
              break;
          case M_type_quantizer: 
              newM_quantizer();
              break;
          case M_type_scquantizer: 
              newM_scquantizer(&scalaName);
              break;
          case M_type_ad: 
              newM_ad(subID1);
              break;*/
          case M_type_mix: 
              newM_mix(subID1);
              break;
          /*case M_type_function:
              newM_function(subID1);
              break;
          case M_type_stereomix: 
              newM_stereomix(subID1);
              break;
          case M_type_vcorgan: 
              newM_vcorgan(subID1);
              break;
          case M_type_dynamicwaves: 
              newM_dynamicwaves(subID1);
              break;
          case M_type_ladspa: 
              if (!ladspaLoadErr) {
                  newM_ladspa(subID1, subID2, newLadspaPolyFlag & 2,
                          newLadspaPolyFlag & 1);
              }
              break; */


          case M_type_pcmout:
          case M_type_jackout:
              newM_pcmout();
              break;
         case M_type_pcmin:
         case M_type_jackin:
              newM_pcmin();
            break;
      /*    case M_type_wavout:
              newM_wavout(); 
              break;
          case M_type_midiout:
              newM_midiout(); 
              break;
          case M_type_scope:
              newM_scope(); 
              break;
          case M_type_spectrum:
              newM_spectrum(); 
              break;
           */
      }

      if (listModule.count() > 0) {
          m = listModule.last();
          if (m != NULL) {
              m->setModuleId(moduleID);
              midiWidget->addModule(m);
          }
      }

      if (synthdata->moduleID <= moduleID) {
          synthdata->moduleID = moduleID + 1;
      }
    }

    // Comment <id> <..> <xpos> <ypos> <width> <height> 
    else if (qs.startsWith("Comment", Qt::CaseInsensitive) && !commentFlag) {
        tokens = qs.split(' ');
        textEditID = tokens[1].toInt(); 
        //textEditID = tokens[2].toInt(); 
        newBoxPos.setX(tokens[3].toInt());
        newBoxPos.setY(tokens[4].toInt());
        w = tokens[5].toInt();
        h = tokens[6].toInt();
        new_textEdit(w, h);
    }
  }

  // end of file reached, we start again looking for other tags
  ts.seek(0);

  while (!ts.atEnd()) {
    qs = ts.readLine();
    tokens = qs.split(' ');

    // Port <idx1> <idx2> <modid11> <modid2>
    if (qs.startsWith("Port", Qt::CaseInsensitive)) {
        index1 = tokens[1].toInt(); 
        index2 = tokens[2].toInt(); 
        moduleID1 = tokens[3].toInt();
        moduleID2 = tokens[4].toInt();

        Module* mod1 = NULL;
        Module* mod2 = NULL;
        Port* inport = NULL;
        Port* outport = NULL;

        mod1 = getModuleWithId(moduleID1);
        if (mod1 == NULL)
            continue;

        mod2 = getModuleWithId(moduleID2);
        if (mod2 == NULL)
            continue;

        inport = mod1->getInPortWithIndex(index1);
        if (inport == NULL)
            continue;

        if (inport->hasConnectedPort()) {
            qWarning(tr("Input port %1 of module %2 is already connected. "
                        "New connection to module %3 ignored.")
                    .arg(index1).arg(moduleID1).arg(moduleID2).toUtf8());
            continue;
        }

        outport = mod2->getOutPortWithIndex(index2);
        if (outport == NULL)
            continue;

        inport->connectTo(outport);
        outport->connectTo(inport);
    }

    // ColorP <idx1> <idx2> <modid11> <modid2> <r1> <g1> <b1> <r2> <g2> <b2>
    else if (qs.startsWith("ColorP", Qt::CaseInsensitive)) {
        index1 = tokens[1].toInt(); 
        index2 = tokens[2].toInt(); 
        moduleID1 = tokens[3].toInt();
        moduleID2 = tokens[4].toInt();
        red1 = tokens[5].toInt(); 
        green1 = tokens[6].toInt(); 
        blue1 = tokens[7].toInt(); 
        red2 = tokens[8].toInt(); 
        green2 = tokens[9].toInt(); 
        blue2 = tokens[10].toInt(); 

        Module* mod1 = NULL;
        Module* mod2 = NULL;
        Port* inport = NULL;
        Port* outport = NULL;

        mod1 = getModuleWithId(moduleID1);
        if (mod1 == NULL)
            continue;

        mod2 = getModuleWithId(moduleID2);
        if (mod2 == NULL)
            continue;

        inport = mod1->getInPortWithIndex(index1);
        if (inport == NULL)
            continue;

        outport = mod2->getOutPortWithIndex(index2);
        if (outport == NULL)
            continue;

        inport->connectTo(outport);
        outport->connectTo(inport);

        inport->jackColor = QColor(red1, green1, blue1);
        inport->cableColor = QColor(red2, green2, blue2);
    }

    // FSlider <modid> <idx> <value> <islog> <s_min> <s_max> <midisign>
    //   0        1      2      3      4        5       6        7
    else if (qs.startsWith("FSlider", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      value = tokens[3].toInt();
      isLog = (tokens[4].toInt() == 1);
      sliderMin = tokens[5].toInt(); 
      sliderMax = tokens[6].toInt(); 
      if (tokens.count() > 6)
          midiSign = tokens[7].toInt(); 

      m = getModuleWithId(moduleID);
      if (m != NULL) {
              MidiSlider* ms = m->configDialog->getMidiSlider(index);
              if (ms != NULL) {
                  MidiControllableFloat &mcAbleF =
                      dynamic_cast<MidiControllableFloat &> (ms->mcAble);
                  mcAbleF.setLog(isLog);
                  mcAbleF.setVal(value, NULL);
                  mcAbleF.setNewMin(sliderMin);
                  mcAbleF.setNewMax(sliderMax);
                  mcAbleF.midiSign = midiSign;
                  /*listModule.at(l1)->configDialog->midiSliderList.at(index)->setLogMode(isLog);
                    listModule.at(l1)->configDialog->midiSliderList.at(index)->updateValue(value);
                    listModule.at(l1)->configDialog->midiSliderList.at(index)->setLogMode(isLog);
                    listModule.at(l1)->configDialog->midiSliderList.at(index)->setNewMin(sliderMin);
                    listModule.at(l1)->configDialog->midiSliderList.at(index)->setNewMax(sliderMax);
                    listModule.at(l1)->configDialog->midiSliderList.at(index)->midiSign = midiSign;*/
          }
      }
    }

    // integer slider
    // ISlider <modid> <idx> <value> <midisign>
    else if (qs.startsWith("ISlider", Qt::CaseInsensitive)) {
        moduleID = tokens[1].toInt(); 
        index = tokens[2].toInt(); 
        value = tokens[3].toInt();
        midiSign = tokens[4].toInt();

        m = getModuleWithId(moduleID);
        if (m != NULL) {
            IntMidiSlider* ims = m->configDialog->getIntMidiSlider(index); 
            if (ims != NULL) {
                MidiControllableBase &mcAble = ims->mcAble;
                mcAble.midiSign = midiSign;
                mcAble.setVal(value, NULL);
                /* listModule.at(l1)->configDialog->intMidiSliderList.at(index)->midiSign = midiSign;
                   listModule.at(l1)->configDialog->intMidiSliderList.at(index)->updateValue((int)value);
                   listModule.at(l1)->configDialog->intMidiSliderList.at(index)->slider->setValue((int)value);*/
            }
        }
    }

    // float slider
    // LSlider <modid> <idx> <value> <midisign>
    else if (qs.startsWith("LSlider", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      value = tokens[3].toInt();
      midiSign = tokens[4].toInt();

      m = getModuleWithId(moduleID);
      if (m != NULL) {
          IntMidiSlider* ims = m->configDialog->getFloatIntMidiSlider(index); 
          if (ims != NULL) {
              MidiControllableBase &mcAble = ims->mcAble;
              mcAble.midiSign = midiSign;
              mcAble.setVal(value, NULL);
              /*listModule.at(l1)->configDialog->floatIntMidiSliderList.at(index)->midiSign = midiSign;
                listModule.at(l1)->configDialog->floatIntMidiSliderList.at(index)->updateValue((int)value);
                listModule.at(l1)->configDialog->floatIntMidiSliderList.at(index)->slider->setValue((int)value);*/
          }
      }
    }

    else if (qs.startsWith("ComboBox", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      value = tokens[3].toInt();
      midiSign = tokens[4].toInt();

      m = getModuleWithId(moduleID);
      if (m != NULL) {
          MidiComboBox* mcb = m->configDialog->getMidiComboBox(index);
          if (mcb != NULL) {
              MidiControllableNames &mcAble =
                  dynamic_cast<MidiControllableNames &>(mcb->mcAble);
              mcAble.midiSign = midiSign;
              mcAble.setVal(value, NULL);
          }
      }
    }

    else if (qs.startsWith("CheckBox", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      value = tokens[3].toInt();
      midiSign = tokens[4].toInt();

      m = getModuleWithId(moduleID);
      if (m != NULL) {
          MidiCheckBox* mcb = m->configDialog->getMidiCheckBox(index);
          if (mcb != NULL) {
              mcb->checkBox->setChecked(value == 1);
              mcb->mcAble.midiSign = midiSign;
          }
      }
    }

    else if (qs.startsWith("Function", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      subID1 = tokens[3].toInt();
      subID2 = tokens[4].toInt();

      m = getModuleWithId(moduleID);
      if (m != NULL) {
          Function* fnc = m->configDialog->getFunction(index);
          if (fnc != NULL)
              fnc->setPointCount(subID2);
      }
    }

    else if (qs.startsWith("Point", Qt::CaseInsensitive)) {
      moduleID = tokens[1].toInt(); 
      index = tokens[2].toInt(); 
      subID1 = tokens[3].toInt();
      subID2 = tokens[4].toInt();
      x = tokens[5].toInt();
      y = tokens[6].toInt();

      m = getModuleWithId(moduleID);
      if (m != NULL) {
          Function* fnc = m->configDialog->getFunction(index);
          if (fnc != NULL)
              fnc->setPoint(subID1, subID2, x, y);
      }
    }

    else if (qs.contains("MIDI", Qt::CaseInsensitive)) {
        moduleID = tokens[1].toInt(); 
        index = tokens[2].toInt(); 
        type = tokens[3].toInt();
        ch = tokens[4].toInt();
        param = tokens[5].toInt();

        MidiControllerKey mck(type, ch, param);
        midiWidget->addMidiController(mck);
        // MidiControllerKey const midiController =
        //    new MidiController(type, ch, param);
        // if (!midiWidget->midiControllerList.contains(midiController)) {
        //    *midiWidget->midiController(MidiController(type, ch, param));
        // } else {
        //  QList<MidiController*>::iterator midiIndex =
        //     midiWidget->midiControllerList.find(midiController);
        //  delete(midiController);
        //  midiController = *midiIndex;
        //  }
        if (qs.startsWith("FSMIDI", Qt::CaseInsensitive)) {
            m = getModuleWithId(moduleID);
            if (m != NULL) {
                MidiSlider* ms = m->configDialog->getMidiSlider(index);
                if (ms != NULL)
                    ms->mcAble.connectToController(mck);
            }
        }
        else if (qs.startsWith("ISMIDI", Qt::CaseInsensitive)) {
            m = getModuleWithId(moduleID);
            if (m != NULL) {
                IntMidiSlider* ims = m->configDialog->getIntMidiSlider(index);
                if (ims != NULL)
                    ims->mcAble.connectToController(mck);
            }
        }
        else if (qs.startsWith("LSMIDI", Qt::CaseInsensitive)) {
            m = getModuleWithId(moduleID);
            if (m != NULL) {
                IntMidiSlider* ims = 
                    m->configDialog->getFloatIntMidiSlider(index);
                if (ims != NULL)
                    ims->mcAble.connectToController(mck);
            }
        }
        else if (qs.startsWith("CMIDI", Qt::CaseInsensitive)) {
            m = getModuleWithId(moduleID);
            if (m != NULL) {
                MidiComboBox* mcb = m->configDialog->getMidiComboBox(index);
                if (mcb != NULL)
                    mcb->mcAble.connectToController(mck);
            }
        }
        else if (qs.startsWith("TMIDI", Qt::CaseInsensitive)) {
            m = getModuleWithId(moduleID);
            if (m != NULL) {
                MidiCheckBox* mcb = m->configDialog->getMidiCheckBox(index);
                if (mcb != NULL)
                    mcb->mcAble.connectToController(mck);
            }
        }
        else
            qWarning(tr("Unknown MIDI controller tag found: %1")
                    .arg(qs).toUtf8());
    }

    // #PARA# <te_id> <..> <line_idx>
    else if (qs.startsWith("#PARA#", Qt::CaseInsensitive)) {
        textEditID = tokens[1].toInt(); 
        //textEditID = tokens[2].toInt(); 
        index = tokens[3].toInt();
        qs = ts.readLine();

        if (!qs.startsWith("#ARAP#", Qt::CaseInsensitive)) {
            para = qs + ' ';
        } else {
            para = " ";
        }
        while (!qs.startsWith("#ARAP#", Qt::CaseInsensitive)) {
            qs = ts.readLine();

            if (!qs.startsWith("#ARAP#", Qt::CaseInsensitive)) {
                para.append(qs + " "); 
            }
        }
        TextEdit* te = getTextEditAt(textEditID);
        if (te != NULL)
            te->textEdit->append(para);
    }

    // Instrument tags
    // Tab "TabName"
    else if (qs.startsWith("Tab", Qt::CaseInsensitive)) {
        QRegExp rx("\"([^\"]+)\"");
        int pos = rx.indexIn(qs);
        if (pos != -1)
            guiWidget->addTab(rx.cap(1));
        else
            qWarning(QObject::tr("No title for tab '%1' found.")
                    .arg(qs).toUtf8());
    }

    // Frame <"FrameName"> <frame number>
    else if (qs.startsWith("Frame", Qt::CaseInsensitive)) {
        QRegExp rx("\"([^\"]+)\"");
        int pos = rx.indexIn(qs);
        if (pos != -1) {
            pos += rx.matchedLength();
            QString number = qs.mid(pos);
            number = number.trimmed();
            guiWidget->setTab(number.toInt());
            guiWidget->addFrame(rx.cap(1));
        }
        else
            qWarning(QObject::tr("No data for frame '%1' found.")
                    .arg(qs).toUtf8());
    }

    // Parameter <"ParamName"> <modid> <index>
    // Parameter <"ParamName"> <modid> <index> <s_min> <s_max> <islog>
    else if (qs.startsWith("Parameter", Qt::CaseInsensitive)) {
        QRegExp rx("\"([^\"]+)\"");
        int pos = rx.indexIn(qs);
        if (pos != -1) {
            pos += rx.matchedLength();
            QString numbers = qs.mid(pos);
            numbers = numbers.trimmed();
            tokens = numbers.split(' ');
            if (tokens.isEmpty()) {
                qWarning(QObject::tr("No parameter values found.").toUtf8());
                continue;
            }

            qs = rx.cap(1);
            moduleID = tokens[0].toInt();
            index = tokens[1].toInt();
        }
        else {
            qWarning(QObject::tr("No parameter name '%1' found.")
                    .arg(qs).toUtf8());
            continue;
        }
        m = getModuleWithId(moduleID);
        if (m != NULL) {
            MidiControllableBase* mcb = m->getMidiControlableBase(index);
            if (mcb != NULL) {
                guiWidget->addParameter(mcb, qs);

                // check for additional parameters of float sliders
                if (tokens.count() == 5) {

                    MidiControllableFloat *mcAbleF =
                        dynamic_cast<MidiControllableFloat *>(mcb);

                    if (mcAbleF != NULL) {
                        sliderMin = tokens[2].toInt();
                        sliderMax = tokens[3].toInt();
                        isLog = (tokens[4].toInt() == 1);

                        mcAbleF->setNewMin(sliderMin);
                        mcAbleF->setNewMax(sliderMax);
                        mcAbleF->setLog(isLog);

                        /*listModule.at(l1)->configDialog->midiGUIcomponentList.at(index)->componentType == GUIcomponentType_slider)*/
                        //!! ((MidiSlider *)guiWidget->parameterList.last())->setNewMin(sliderMin);
                        // ((MidiSlider *)guiWidget->parameterList.last())->setNewMax(sliderMax);
                        // ((MidiSlider *)guiWidget->parameterList.last())->setLogMode(isLog);
                    }
                    else
                        qWarning("MIDI controllable at index %d for "
                                "parameter '%s' is no float parameter.",
                                index, qs.toUtf8().constData());
                }
                else
                    qWarning("Parameter list too short for '%s' (index = %d).",
                            qs.toUtf8().constData(), index);
            }
            else
                qWarning("No MIDI controllable at index %d found.",
                        index);
        }
        else
            qWarning("No module with ID %d for parameter %s found.",
                    moduleID, qs.toUtf8().constData());
    }

    else if (qs.startsWith("Program", Qt::CaseInsensitive)) {

        index = tokens[1].toInt(); 
        value = tokens[2].toInt();

        if (index != currentProgram) {
            currentProgram = index;
            guiWidget->setPresetCount(currentProgram + 1);
        }
        guiWidget->presetList[currentProgram].append(value);
    }
    
    else if (qs.startsWith("PresetName", Qt::CaseInsensitive)) {
        QRegExp rx("\"([^\"]+)\"");
        int pos = rx.indexIn(qs);
        if (pos != -1)
            qs = rx.cap(1);
        else {
            qWarning(QObject::tr("No name for preset '%1' found.")
                    .arg(qs).toUtf8());
            continue;
        }

      qs2.sprintf("%3d", guiWidget->presetNameList.count());
      guiWidget->presetNameList.append(qs2+qs);
    }
    /*For debugging only, 'Module' and 'Comment' will also apear
    else
        qWarning(tr("Unknown tag found: %1").arg(qs).toUtf8());
    */
  } // end while loop

  if (guiWidget->presetCount)
    guiWidget->setCurrentPreset(0);

  loadingPatch = false;

  adjustSize();
  update();
  midiWidget->setActiveMidiControllers();
  if (restartSynth)
    startSynth();
  if (verbose) {
    StdOut << "synthdata->periodsize = " << synthdata->periodsize << endl;
    StdOut << "synthdata->cyclesize = " << synthdata->cyclesize << endl;
    StdOut << "Module::portmemAllocated = " << Module::portmemAllocated << endl;
  }
  midiWidget->followConfig = followConfig;
  guiWidget->refreshGui();
  modified = false;
}

void ModularSynth::save(QTextStream& ts)
{
    int l1, l2;
    int offX = 0, offY = 0;
    QString qs;

    // adjust modules x position
    if (childrenRect().left() > 100)
      offX = childrenRect().left() - 100;

    // adjust modules y position
    if (childrenRect().top() > 66)
      offY = childrenRect().top() - 66;
    
    // save module parameters
    for (l1 = 0; l1 < listModule.count(); ++l1) {
      ts << "Module "
          << (int)listModule.at(l1)->M_type << ' '
          << listModule.at(l1)->moduleID << ' '
          << listModule.at(l1)->x() - offX << ' '
          << listModule.at(l1)->y() - offY << ' ';

      switch(listModule.at(l1)->M_type)
      {
     //   case M_type_custom:
       //   break;
        case M_type_vca: 
          ts << (int)((M_vca *)listModule.at(l1))->expMode << " 0" << endl;
          break;
   /*     case M_type_ad:
        case M_type_function: 
          ts << listModule.at(l1)->outPortCount << " 0" << endl;
          break;*/
        case M_type_mix: 
          ts << ((M_mix *)listModule.at(l1))->in_channels << " 0" << endl;
          break;
      /*  case M_type_stereomix:
          ts << ((M_stereomix *)listModule.at(l1))->in_channels << " 0" << endl;
          break;
        case M_type_vcorgan: 
          ts << ((M_vcorgan *)listModule.at(l1))->oscCount << " 0" << endl;
          break;
        case M_type_dynamicwaves: 
          ts << ((M_dynamicwaves *)listModule.at(l1))->oscCount << " 0" << endl;
          break;
          */
        case M_type_seq: 
          ts << ((M_seq *)listModule.at(l1))->seqLen << " 0" << endl;
          break;
  /*      case M_type_ladspa:
          ts << 2 * (int)((M_ladspa *)listModule.at(l1))->isPoly
                  + (int)((M_ladspa *)listModule.at(l1))->hasExtCtrlPorts
             << ' '
             << synthdata->ladspaLib.at(((M_ladspa *)listModule.at(l1))
                         ->ladspaDesFuncIndex).name
             << ' '
             << ((M_ladspa *)listModule.at(l1))->pluginName << endl;
          break;
        case M_type_scquantizer: 
          qs = ((M_scquantizer *)listModule.at(l1))->sclname;
          if (qs.contains('/')) {
            qs = qs.mid(qs.lastIndexOf('/') + 1);             
          }
          ts << qs << endl;
          break;
        case M_type_scmcv: 
          qs = ((M_scmcv *)listModule.at(l1))->sclname;
          if (qs.contains('/')) {
            qs = qs.mid(qs.lastIndexOf('/') + 1);             
          }
          ts << qs << endl;
          break;
          */
        default:
          ts << "0 0" << endl;
          break; 
      }
      listModule.at(l1)->save(ts);
    }

    // save comment window content
    for (l1 = 0; l1 < listTextEdit.count(); ++l1) {
        TextEdit *te = listTextEdit.at(l1);
        if (te != NULL) {
            ts << "Comment " 
                << te->textEditID << ' '
                << l1 << ' '
                << te->x() - offX << ' '
                << te->y() - offY << ' '
                << te->width() << ' '
                << te->height() << endl;

            QTextDocument *tD = te->textEdit->document();
            QTextBlock tB = tD->begin();
            for (l2 = 0; l2 < tD->blockCount(); ++l2, tB = tB.next()) {
                ts << "#PARA# " << te->textEditID << ' '
                    << l1 << ' ' << l2 << endl;
                ts << tB.text() << endl; 
                ts << "#ARAP#" << endl; 
            }
        }
    }

    // save MIDI controller window configuration
    guiWidget->save(ts);

    modified = false;
}
//====================================================== End persistence

void ModularSynth::allVoicesOff()
{
  synthdata->allNotesOff();
}

void ModularSynth::loadPreferences(QString& line)
{
    prefWidget->loadPref(line);
    refreshColors();
}


void ModularSynth::savePreferences(QTextStream& ts)
{
    prefWidget->savePref(ts);
}


void ModularSynth::showContextMenu(const QPoint& pos) {
  
    contextMenu->popup(mapToGlobal(pos));
}


void ModularSynth::refreshColors() {

  for (int l1 = 0; l1 < listModule.count(); ++l1) {
      Module* m = listModule[l1];
      if (m != NULL)
          m->getColors();
  }      
  prefWidget->refreshColors();

  QPalette p = palette();
  p.setColor(backgroundRole(), synthdata->colorBackground);
  setAutoFillBackground(true);
  setPalette(p);
  update();
}


void ModularSynth::updateColors()
{
  synthdata->colorPortFont1 = synthdata->colorModuleFont;
  synthdata->colorPortFont2 = QColor(255, 240, 140);
  prefWidget->recallColors();
  refreshColors();
}

void ModularSynth::moveAllBoxes(const QPoint &delta)
{
  modified = true;

  if (delta.isNull())
    return;

  int i;
  for (i = 0; i < listModule.count(); ++i) {
    Box *b = listModule[i];
    b->move(b->pos() + delta);
  }
  for (i = 0; i < listTextEdit.count(); ++i) {
    Box *b = listTextEdit[i];
    b->move(b->pos() + delta);
  }
}

bool ModularSynth::isModified()
{
   return modified;
}

QString ModularSynth::getLoadPath()
{
    return synthdata->loadPath;
}

void ModularSynth::setLoadPath(const QString& sp)
{
    synthdata->loadPath = sp;
}

QString ModularSynth::getSavePath()
{
    return synthdata->savePath;
}

void ModularSynth::setSavePath(const QString& sp)
{
    synthdata->savePath = sp;
}

Module* ModularSynth::getModuleWithId(int id)
{
    Module* m = NULL;

    for (int i = 0; i < listModule.count(); ++i) {
        m = listModule.at(i);
        if (m != NULL && m->hasModuleId(id)) {
            break;
        }
    }
    if (m == NULL)
        qWarning("No module with id %d found.", id);

    return m;
}

TextEdit* ModularSynth::getTextEditAt(int idx)
{
    TextEdit* te = NULL;

    if ((idx + 1) > listTextEdit.count())
        qWarning("TextEdit index out of range (value = %d)", idx);
    else
        te = listTextEdit.at(idx);

    return te;
}

