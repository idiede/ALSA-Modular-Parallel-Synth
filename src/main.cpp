#include <stdio.h>      
#include <stdlib.h>     
#include <getopt.h>  
#include <string.h>
#include <unistd.h>

#include <QApplication>
#include <QDir>
#include <QString>
#include <QLibraryInfo>
#include <QLocale>
#include <QObject>
#include <QTranslator>

#include "mainwindow.h"
#include "msoptions.h"
#include "config.h"


#define AMSDIR ".alsamodular"

static struct option options[] =
        {{"periodsize", 1, 0, 'b'},
         {"frag", 1, 0, 'f'},   
         {"poly", 1, 0, 'p'},
         {"threads", 1, 0, 't'},
         {"rate", 1, 0, 'r'},   
         {"edge", 1, 0, 'e'},   
         {"help", 0, 0, 'h'},
         {"soundcard", 1, 0, 'c'},
         {"preset", 1, 0, 'l'},
         {"presetpath", 1, 0, 'd'},
         {"nogui", 0, 0, 'n'},
         {"jack", 0, 0, 'J'},
         {"alsa", 0, 0, 'A'},
         {"in", 1, 0, 'i'},
         {"out", 1, 0, 'o'},
         {"name", 1, 0, 'N'},
         {0, 0, 0, 0}};


QTextStream StdOut(stdout);
QTextStream StdErr(stderr);



QString amsRcPath(const QString &synthName)
{
  return QString("%1/" AMSDIR "/%2.cfg").arg(QDir::homePath())
      .arg(synthName);
}

QString amsSynthName(const QString &name, unsigned int index)
{
  return QString("%1_%2").arg(name).arg(index);
}

int makeSynthName(QString &name)
{
    int fd;

    QDir amshome = QDir(QDir::homePath());
    if (!amshome.exists(AMSDIR)) {
        if (!amshome.mkdir(AMSDIR)) {
            qWarning(QObject::tr("Could not create v-amps home "
                        "directory.").toUtf8());
            return -1;
        }
    }

    for (unsigned int index = 1; index <= 9; index++) {
        QString rcPath = amsRcPath(amsSynthName(name, index));
        //StdOut << "Resource file path: " << rcPath << endl;
        fd = open(rcPath.toLatin1().data(), O_CREAT|O_RDWR, 0666);
        if (fd == -1) {
            qWarning(QObject::tr("Failed to open file '%1'")
                    .arg(rcPath).toUtf8());
            return -1;
        }

        struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            close(fd);
            StdOut << "File occupied: " << rcPath << endl;
        } else {
            lock.l_type = F_RDLCK;
            if (fcntl(fd, F_SETLK, &lock) == -1) {
                qWarning(QObject::tr("Ooops in %1 at %2")
                        .arg(__FUNCTION__).arg(__LINE__).toUtf8());
                return -1;
            }
            name = amsSynthName(name, index);
            return fd;
        }
    }
    qWarning(QObject::tr("Client name '%1' occupied.").arg(name).toUtf8());
    return -1;
}


int main(int argc, char *argv[])  
{
  char aboutText[] = MONO_AMPS_LONGNAME " " VERSION " (c)2002-2009";
  /*                 "\nby Matthias Nagorni and Fons Adriaensen\n"
                     "(c)2002-2003 SuSE AG Nuremberg\n"
                     "(c)2003 Fons Adriaensen\n"
		     "Additional programming:\n"
		     "2007 Malte Steiner, Karsten Wiese\n"
		     "2008 Guido Scholz\n";
 */
  QApplication app(argc, argv);

  // translator for Qt library strings
  QTranslator qtTr;
  QLocale loc = QLocale::system();

  if (qtTr.load(QString("qt_") + loc.name(),
              QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
      app.installTranslator(&qtTr);
  else
      qWarning("No Qt translation for locale '%s' found.",
              loc.name().toUtf8().constData());
  
  // translator for ams strings
  QTranslator amsTr;

  if (amsTr.load(QString("ams_") + loc.name(), TRANSLATIONSDIR))
      app.installTranslator(&amsTr);
  else
      qWarning("No " MONO_AMPS_LONGNAME " translation for locale '%s' found.",
              loc.name().toUtf8().constData());
  
  int getopt_return;
  int option_index;
  ModularSynthOptions msoptions;

  msoptions.synthName = "v-amps";
  msoptions.pcmname = DEFAULT_PCMNAME;
  msoptions.frsize = DEFAULT_PERIODSIZE;
  msoptions.fsamp = DEFAULT_RATE;
  msoptions.ncapt = DEFAULT_CAPT_PORTS;
  msoptions.nfrags = DEFAULT_PERIODS;
  msoptions.nplay = DEFAULT_PLAY_PORTS;
  msoptions.poly = 1;
  msoptions.edge = 1.0;
  msoptions.noGui = false;
  msoptions.presetName = "";
  msoptions.presetPath = "";
  msoptions.havePreset = false;
  msoptions.havePresetPath = false;
  msoptions.forceJack = false;
  msoptions.forceAlsa = false;
  msoptions.verbose = 0;

  while ((getopt_return = getopt_long(argc, argv, "hnJjAab:p:t:f:e:c:l:d:r:i:o:vN:",
				      options, &option_index)) >= 0) {
    switch(getopt_return) {
    case 'p': 
        msoptions.poly = atoi(optarg);
        break;
    case 't':
        msoptions.threads = atoi(optarg);
        break;
    case 'b': 
        msoptions.frsize = atoi(optarg);
        break;
    case 'f': 
        msoptions.nfrags = atoi(optarg);
        break;
    case 'e': 
        msoptions.edge = atof(optarg);
        break;
    case 'r': 
        msoptions.fsamp = atoi(optarg);
        break;
    case 'c': 
        msoptions.pcmname = optarg;
        break; 
    case 'l': 
        msoptions.presetName = optarg;
        msoptions.havePreset = true;
        break; 
    case 'd': 
        msoptions.presetPath = optarg;
        msoptions.havePresetPath = true;
        break; 
    case 'n':
        msoptions.noGui = true;
        break;
    case 'J':
    case 'j':
        msoptions.forceJack = true;
        msoptions.forceAlsa = false;
        break;
    case 'A':
    case 'a':
        msoptions.forceJack = false;
        msoptions.forceAlsa = true;
        break;
    case 'i': 
        msoptions.ncapt = atoi(optarg);
        break;
    case 'o': 
        msoptions.nplay = atoi(optarg);
        break;
    case 'v':
      msoptions.verbose++;
      break;
    case 'N':
      msoptions.synthName += optarg;
      break;
    case 'h':
	printf("%s\n\n", aboutText);
	printf("Usage:\v-amps [OPTION]... [PRESETFILE]\n\nOptions:\n");
        printf("  -J, --jack                    Force JACK\n");
        printf("    -i, --in <num>                Number of JACK input ports\n");
        printf("    -o, --out <num>               Number of JACK output ports\n");
        printf("  -A, --alsa                    Force ALSA\n");
        printf("    -c, --soundcard <plug>        Soundcard [hw:0,0]\n");
        printf("    -b, --periodsize <frames>     Periodsize [%d]\n", DEFAULT_PERIODSIZE);
        printf("    -f, --frag <num>              Number of fragments [%d]\n", DEFAULT_PERIODS);
        printf("    -r, --rate <samples/s>        Samplerate [%d]\n", DEFAULT_RATE);
        printf("  -p, --poly <num>              Polyphony [1]\n");
        printf("  -t, --thread <num>            Number of threads [1]\n");
        printf("  -e, --edge <0..10>            VCO Edge [1.0]\n");
        printf("  -l, --preset <file>           Preset file\n");
        printf("  -d, --presetpath <path>       Preset path\n");
        printf("  -n, --nogui                   Start without GUI\n");
        printf("  -v,				verbose\n");
        printf("  -N, --name <name>             ALSASEQ/JACK clientname, windowtitle\n\n");
        exit(EXIT_SUCCESS);
        break;
    }
  }

  if (!msoptions.havePreset && optind < argc){
    msoptions.presetName = argv[optind];
    msoptions.havePreset = true;
  }

  msoptions.rcFd = makeSynthName(msoptions.synthName);
  if (msoptions.rcFd == -1)
      exit(1);

  MainWindow* top = new MainWindow(msoptions);
  Q_CHECK_PTR(top);
  top->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  return app.exec();
}
