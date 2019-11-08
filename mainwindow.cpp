#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <gst/gst.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GstElement *pipeline = gst_pipeline_new(nullptr);
    GstElement *src = gst_element_factory_make("videotestsrc", nullptr);
    GstElement *glupload = gst_element_factory_make("glupload", nullptr);
    // the plugin must be loaded before loading the qml file to register the GstGLVideoItem qml item
    GstElement *sink = gst_element_factory_make("qmlglsink", nullptr);

    assert(src && glupload && sink);

    gst_bin_add_many(GST_BIN(pipeline), src, glupload, sink, nullptr);
    gst_element_link_many(src, glupload, sink, nullptr);

    QUrl qmlSource("qrc:/ui/video.qml");
    ui->quickWidget->setSource(qmlSource);

    QQuickItem *videoItem;
    QQuickWindow *qQuickWindows;

    /* find and set the videoItem on the sink */
    videoItem = ui->quickWidget->rootObject();
    videoItem = videoItem->findChild<QQuickItem *>("videoItem");
    assert(videoItem);
    g_object_set(sink, "widget", videoItem, nullptr);

    qQuickWindows = videoItem->window();
    assert(qQuickWindows);
    qQuickWindows->scheduleRenderJob(new SetPlaying(pipeline),
                                     QQuickWindow::BeforeSynchronizingStage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

SetPlaying::SetPlaying(GstElement *pipeline)
{
    m_pipeline = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)):nullptr;
}

SetPlaying::~SetPlaying()
{
    if (m_pipeline)
        gst_object_unref (m_pipeline);
}

void SetPlaying::run()
{
    if (m_pipeline)
        gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}
