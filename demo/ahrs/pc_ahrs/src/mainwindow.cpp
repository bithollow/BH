#include "mainwindow.h"
#include "bhsocket.h"

// Constructor of the main window
// Create window properties, menu etc ...
MainWindow::MainWindow(QWidget *parent,int w, int h)
    : QMainWindow(parent)
{ 
    // Set the window size
    this->resize(w,h);
    this->setWindowTitle("Object viewer");

    // Create a layout in the main window
    centralWidget = new QWidget(this);
    gridLayoutWidget = new QWidget(centralWidget);
    gridLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
    gridLayout = new QGridLayout(gridLayoutWidget);

    // Create the openGL display for the map
    Object_GL = new ObjectOpenGL(gridLayoutWidget);
    Object_GL->setObjectName(QString::fromUtf8("ObjectOpenGL"));
    Object_GL->setGeometry(QRect(0, 0, this->width(), this->height()));

    // Insert the Open Gl display into the layout
    gridLayout->addWidget(Object_GL, 0, 0, 1, 1);
    setCentralWidget(centralWidget);

    // Create the menubar
    QMenu *FileMenu = menuBar()->addMenu("&File");
    FileMenu->addSeparator();
    FileMenu->addAction("Quit", qApp, SLOT (quit()), QKeySequence(tr("Ctrl+Q")));

    // Add menu items
    QMenu *ViewMenu = menuBar()->addMenu("&View");
    ViewMenu->addAction("Front view",       Object_GL, SLOT (FrontView()),  QKeySequence(tr("Ctrl+f")));
    ViewMenu->addAction("Rear view",        Object_GL, SLOT (RearView()),   QKeySequence(tr("Ctrl+e")));
    ViewMenu->addAction("Left view",        Object_GL, SLOT (LeftView()),   QKeySequence(tr("Ctrl+l")));
    ViewMenu->addAction("Right view",       Object_GL, SLOT (RightView()),  QKeySequence(tr("Ctrl+r")));
    ViewMenu->addAction("Top view",         Object_GL, SLOT (TopView()),    QKeySequence(tr("Ctrl+t")));
    ViewMenu->addAction("Bottom view",      Object_GL, SLOT (BottomView()), QKeySequence(tr("Ctrl+b")));
    FileMenu->addSeparator();
    ViewMenu->addAction("Isometric",        Object_GL, SLOT (IsometricView()), QKeySequence(tr("Ctrl+i")));
    QMenu *AboutMenu = menuBar()->addMenu("?");
    AboutMenu->addAction("About Convert_STL_2_Cube", this, SLOT (handleAbout()));

    // Timer (used for repainting the GL Window every 50 ms)
    QTimer *timerDisplay = new QTimer();
    timerDisplay->connect(timerDisplay, SIGNAL(timeout()),this, SLOT(onTimer_UpdateDisplay()));
    timerDisplay->start(5);

    // Timer for reading raw data (every 10ms)
    QTimer *timerArduino = new QTimer();
    timerArduino->connect(timerArduino, SIGNAL(timeout()),this, SLOT(onTimer_ReadData()));
    timerArduino->start(5);
}


// Desctructor
MainWindow::~MainWindow()
{}




// On resize event, the items in the window are resized
void MainWindow::resizeEvent(QResizeEvent *)
{
    Object_GL->resize(centralWidget->width(),centralWidget->height());
    gridLayoutWidget->setGeometry(QRect(0, 0, centralWidget->width(), centralWidget->height()));
}

// Timer event : repain the Open Gl window
void MainWindow::onTimer_UpdateDisplay()
{
    Object_GL->updateGL();
//    printf("MainWindow::onTimer_UpdateDisplay\n");
}

#define         RATIO_ACC       (16.0/32767.0)
//#define         RATIO_GYRO      ((2000./32768.)*(M_PI/180.))
#define         RATIO_GYRO    (2000.0/32767.0)
#define         RATIO_MAG       (5855.0/32767.0)

// Timer event : get raw data from Arduino
void MainWindow::onTimer_ReadData()
{

    // Read data from MPU-9250
    char buffer[300] = {0};
    memset(buffer, 0, sizeof(buffer));
    bh_mpu9250.recvFrom(buffer, 300);

    float dt = 0;
    // Convert into floats
    float ax = 0,ay = 0,az = 0;
    float gx = 0,gy = 0,gz = 0;
    float mx = 0,my = 0,mz = 0;
    float roll = 0,pitch = 0,yaw = 0;

    sscanf(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f\n",&ax,&ay,&az,&gx,&gy,&gz,&mx,&my,&mz,&roll,&pitch,&yaw);
    printf("data= %s\n",buffer);
    /*
    ax=0.0932262;
    ay=-0.145508;
    az=0.975586;

    gx= -0.304878;
    gy=0.792683;
    gz=0.670732; 

    mx=35.742188;
    my=40.031250;
    mz=-32.962502;
    roll=-8.569538;
    pitch=-4.321769;
    yaw=-43.814415;
    */
    Object_GL->setAcceleromter(ay,ax,az);
    Object_GL->setGyroscope(gy, gx,gz);
    Object_GL->setMagnetometer(my, mx,mz); 
    Object_GL->setAngles(roll,pitch,yaw);
}

// Open the 'about' dialog box
void MainWindow::handleAbout()
{
    QMessageBox::information(this,"About OpenGL Frame","<H2>OpenGL Frame</H2>2011<BR>Supported by the Cart-O-Matic project (ANR CAROTTE)<BR>University of Angers (France,Europe)<BR>Designed by Philippe Lucidarme <BR> Contact: philippe.lucidarme@univ-angers.fr. ");
}


// Connect to the serial device (Arduino)
bool MainWindow::connect()
{
    // Connect to serial port
  /*
    if (mpu9250.openDevice(DEVICE_NAME,115200)!=1)
    {
        std::cerr << "Error while opening serial device" << std::endl;
        return false;
    }
  */
    // Flush receiver of previously received data
    usleep(10000);
    //mpu9250.flushReceiver();
    return true;
}
